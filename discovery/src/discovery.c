//
// Created by f3m on 04/04/26.
//

#include <discovery.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define DISCOVERY_MAX_NEIGHBORS 64U
#define DISCOVERY_DEFAULT_CONVERGENCE_K 3U
#define DISCOVERY_DEFAULT_MIN_TX_INTERVAL_US 100000U
#define DISCOVERY_DEFAULT_MAX_TX_INTERVAL_US 500000U
#define DISCOVERY_PACKET_TAG 0x44534356U

#ifndef DISCOVERY_NODE_ID
#define DISCOVERY_NODE_ID 1ULL
#endif

typedef struct discovery_packet
{
    uint32_t tag;
    uint64_t my_id;
    uint64_t last_id;
} discovery_packet_t;

typedef enum discovery_state
{
    DISCOVERY_STATE_ACTIVE,
    DISCOVERY_STATE_PASSIVE,
    DISCOVERY_STATE_TERMINATED
} discovery_state_t;

typedef enum discovery_event
{
    DISCOVERY_EVENT_RX,
    DISCOVERY_EVENT_TX_TIMER,
    DISCOVERY_EVENT_STOP
} discovery_event_t;

typedef struct discovery_neighbor
{
    uint64_t node_id;
    uint64_t first_seen_us;
    uint64_t last_seen_us;
    uint32_t rx_count;
} discovery_neighbor_t;

typedef struct discovery_runtime
{
    discovery_state_t state;
    uint64_t node_id;
    uint64_t last_seen_id;
    uint32_t convergence_k;
    uint32_t min_tx_interval_us;
    uint32_t max_tx_interval_us;
    uint32_t tx_count;
    uint32_t unchanged_tx_count;
    uint64_t last_table_change_us;
    struct timespec next_tx_at;
    unsigned int rng_state;
    discovery_neighbor_t neighbors[DISCOVERY_MAX_NEIGHBORS];
    size_t neighbor_count;
} discovery_runtime_t;

ds_out_t *ds_out = nullptr;
ds_in_t *ds_in = nullptr;

static uint64_t discovery_monotonic_us()
{
    struct timespec now = {0};
    clock_gettime(CLOCK_MONOTONIC, &now);
    return (uint64_t)now.tv_sec * 1000000U + (uint64_t)now.tv_nsec / 1000U;
}

static void discovery_add_us(struct timespec *ts, const uint32_t us)
{
    ts->tv_sec += us / 1000000U;
    ts->tv_nsec += (long)(us % 1000000U) * 1000L;
    if (ts->tv_nsec >= 1000000000L)
    {
        ts->tv_sec += 1;
        ts->tv_nsec -= 1000000000L;
    }
}

static uint32_t discovery_random_interval_us(discovery_runtime_t *runtime)
{
    const uint32_t min_us = runtime->min_tx_interval_us;
    const uint32_t max_us = runtime->max_tx_interval_us;

    return min_us + rand_r(&runtime->rng_state) % (max_us - min_us + 1U);
}

static void discovery_schedule_next_tx(discovery_runtime_t *runtime)
{
    clock_gettime(CLOCK_REALTIME, &runtime->next_tx_at);
    discovery_add_us(&runtime->next_tx_at, discovery_random_interval_us(runtime));
}

static int discovery_send_packet(discovery_runtime_t *runtime)
{
    const discovery_packet_t packet = {
        .tag = DISCOVERY_PACKET_TAG,
        .my_id = runtime->node_id,
        .last_id = runtime->last_seen_id,
    };

    const int err = ds_in->net_broadcast((uint8_t *)&packet, sizeof(packet));
    if (err == EXIT_SUCCESS)
        runtime->tx_count++;

    return err;
}

static void discovery_check_termination(discovery_runtime_t *runtime)
{
    if (runtime->state == DISCOVERY_STATE_TERMINATED)
        return;

    if (runtime->unchanged_tx_count >= runtime->convergence_k)
    {
        runtime->state = DISCOVERY_STATE_TERMINATED;
        printf("[DISCOVERY] Iteration converged: neighbors=%zu, tx_count=%u\n",
               runtime->neighbor_count,
               runtime->tx_count);
    }
}

static void discovery_handle_tx_timer(discovery_runtime_t *runtime)
{
    if (runtime->state == DISCOVERY_STATE_ACTIVE)
    {
        if (discovery_send_packet(runtime) != EXIT_SUCCESS)
        {
            fprintf(stderr, "[DISCOVERY] Failed to broadcast discovery packet\n");
            discovery_schedule_next_tx(runtime);
            return;
        }
    }

    runtime->unchanged_tx_count++;
    discovery_schedule_next_tx(runtime);
}

static bool discovery_has_rx_locked()
{
    return ds_in->rx_count > 0U;
}

static bool discovery_pop_rx(ds_rx_packet_t *packet)
{
    bool found = false;

    memset(packet, 0, sizeof(*packet));
    pthread_mutex_lock(&ds_in->rx_mutex);

    if (ds_in->rx_count > 0U)
    {
        *packet = ds_in->rx_queue[ds_in->rx_read_idx];
        memset(&ds_in->rx_queue[ds_in->rx_read_idx], 0, sizeof(ds_in->rx_queue[ds_in->rx_read_idx]));

        ds_in->rx_read_idx = (uint8_t)((ds_in->rx_read_idx + 1U) % RECV_BUFFER_SIZE);
        ds_in->rx_count--;
        found = true;
    }

    pthread_mutex_unlock(&ds_in->rx_mutex);
    return found;
}

static discovery_event_t discovery_wait_event(const discovery_runtime_t *runtime)
{
    discovery_event_t event = DISCOVERY_EVENT_TX_TIMER;

    pthread_mutex_lock(&ds_in->rx_mutex);

    while (!ds_in->stop && !discovery_has_rx_locked())
    {
        const int err = pthread_cond_timedwait(
            &ds_in->rx_cond,
            &ds_in->rx_mutex,
            &runtime->next_tx_at);

        if (err == ETIMEDOUT)
            break;

        if (err != 0)
        {
            fprintf(stderr, "[DISCOVERY] pthread_cond_timedwait failed: %s\n", strerror(err));
            break;
        }
    }

    if (ds_in->stop)
        event = DISCOVERY_EVENT_STOP;
    else if (discovery_has_rx_locked())
        event = DISCOVERY_EVENT_RX;

    pthread_mutex_unlock(&ds_in->rx_mutex);
    return event;
}

static discovery_neighbor_t *discovery_find_neighbor(discovery_runtime_t *runtime, const uint64_t node_id)
{
    for (size_t i = 0; i < runtime->neighbor_count; i++)
    {
        if (runtime->neighbors[i].node_id == node_id)
            return &runtime->neighbors[i];
    }

    return nullptr;
}

static bool discovery_update_neighbor(discovery_runtime_t *runtime, const uint64_t node_id)
{
    const uint64_t now = discovery_monotonic_us();
    discovery_neighbor_t *neighbor = discovery_find_neighbor(runtime, node_id);

    if (neighbor != nullptr)
    {
        neighbor->last_seen_us = now;
        neighbor->rx_count++;
        return false;
    }

    if (runtime->neighbor_count >= DISCOVERY_MAX_NEIGHBORS)
    {
        fprintf(stderr, "[DISCOVERY] Neighbor table full, dropping node_id=%lu\n", node_id);
        return false;
    }

    neighbor = &runtime->neighbors[runtime->neighbor_count++];
    neighbor->node_id = node_id;
    neighbor->first_seen_us = now;
    neighbor->last_seen_us = now;
    neighbor->rx_count = 1;
    runtime->last_table_change_us = now;

    return true;
}

static void discovery_process_packet(discovery_runtime_t *runtime, const ds_rx_packet_t *rx_packet)
{
    if (rx_packet->data == nullptr || rx_packet->len < sizeof(discovery_packet_t))
        return;

    const discovery_packet_t *packet = (const discovery_packet_t *)rx_packet->data;

    if (packet->tag != DISCOVERY_PACKET_TAG)
        return;

    if (packet->my_id == runtime->node_id)
        return;

    const bool table_changed = discovery_update_neighbor(runtime, packet->my_id);
    runtime->last_seen_id = packet->my_id;
    runtime->unchanged_tx_count = 0;

    if (packet->last_id == runtime->node_id)
        runtime->state = DISCOVERY_STATE_PASSIVE;

    if (table_changed)
    {
        printf("[DISCOVERY] Discovered node_id=%lu, neighbors=%zu\n",
               packet->my_id,
               runtime->neighbor_count);
    }
}

static void discovery_process_rx(discovery_runtime_t *runtime)
{
    ds_rx_packet_t rx_packet = {0};

    while (discovery_pop_rx(&rx_packet))
    {
        discovery_process_packet(runtime, &rx_packet);
        free(rx_packet.data);
        memset(&rx_packet, 0, sizeof(rx_packet));
    }
}

static int discovery_runtime_init(discovery_runtime_t *runtime, const ds_in_t *in)
{
    if (in == nullptr || in->net_broadcast == nullptr)
        return EXIT_FAILURE;

    memset(runtime, 0, sizeof(*runtime));

    runtime->state = DISCOVERY_STATE_ACTIVE;
    runtime->node_id = DISCOVERY_NODE_ID;
    runtime->convergence_k = DISCOVERY_DEFAULT_CONVERGENCE_K;
    runtime->min_tx_interval_us = DISCOVERY_DEFAULT_MIN_TX_INTERVAL_US;
    runtime->max_tx_interval_us = DISCOVERY_DEFAULT_MAX_TX_INTERVAL_US;
    runtime->last_table_change_us = discovery_monotonic_us();
    runtime->rng_state = (unsigned int)runtime->node_id ^ (unsigned int)time(nullptr);
    discovery_schedule_next_tx(runtime);

    return EXIT_SUCCESS;
}

void *discovery_loop(void *args)
{
    (void)args;

    uint64_t iteration = 0;

    while (!ds_in->stop)
    {
        discovery_runtime_t runtime = {0};
        if (discovery_runtime_init(&runtime, ds_in) != EXIT_SUCCESS)
        {
            fprintf(stderr, "[DISCOVERY] Runtime init failed\n");
            return nullptr;
        }

        iteration++;
        printf("[DISCOVERY] Iteration %lu initialized: node_id=%lu, k=%u, tx_interval=%u-%u us\n",
               iteration,
               runtime.node_id,
               runtime.convergence_k,
               runtime.min_tx_interval_us,
               runtime.max_tx_interval_us);

        while (!ds_in->stop && runtime.state != DISCOVERY_STATE_TERMINATED)
        {
            const discovery_event_t event = discovery_wait_event(&runtime);

            switch (event)
            {
                case DISCOVERY_EVENT_RX:
                    discovery_process_rx(&runtime);
                    break;
                case DISCOVERY_EVENT_TX_TIMER:
                    discovery_handle_tx_timer(&runtime);
                    break;
                case DISCOVERY_EVENT_STOP:
                    break;
            }

            discovery_check_termination(&runtime);
        }
    }

    return nullptr;
}


int init_discovery(ds_in_t *in, ds_out_t *out)
{
    ds_out = out;
    ds_in = in;

    memset(ds_in->rx_queue, 0, sizeof(ds_in->rx_queue));
    pthread_mutex_init(&ds_in->rx_mutex, nullptr);
    pthread_cond_init(&ds_in->rx_cond, nullptr);
    ds_in->rx_read_idx = 0;
    ds_in->rx_write_idx = 0;
    ds_in->rx_count = 0;
    ds_in->stop = false;

    const int err = pthread_create(&ds_in->thread, nullptr, discovery_loop, nullptr);
    if (err != 0)
    {
        fprintf(stderr, "pthread_create failed: %s\n", strerror(err));
        return EXIT_FAILURE;
    }
    printf("[DISCOVERY] Discovery initialized!\n");
    return EXIT_SUCCESS;
}
void destroy_discovery()
{
    ds_in->stop = true;
    pthread_cond_broadcast(&ds_in->rx_cond);
    pthread_join(ds_in->thread, nullptr);
    pthread_cond_destroy(&ds_in->rx_cond);
    pthread_mutex_destroy(&ds_in->rx_mutex);
}
