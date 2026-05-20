//
// Created by f3m on 04/04/26.
//

#pragma once

#ifdef __cplusplus
extern "C" {
#endif


#include <pthread.h>
#include <stddef.h>
#include <stdint.h>
#include <autonet_types.h>

typedef struct ds_rx_packet
{
    uint8_t *data;
    size_t len;
} ds_rx_packet_t;

typedef struct ds_in
{
    uint64_t node_id;
    uint32_t convergence_k;
    uint32_t min_tx_interval_us;
    uint32_t max_tx_interval_us;
    pthread_mutex_t *net_mutex;
    pthread_cond_t *net_cond;
    int (*net_broadcast)(uint8_t *buf, size_t len);
    pthread_mutex_t rx_mutex;
    pthread_cond_t rx_cond;
    ds_rx_packet_t rx_queue[RECV_BUFFER_SIZE];
    uint8_t rx_read_idx;
    uint8_t rx_write_idx;
    uint8_t rx_count;
    pthread_t thread;
    bool stop;
} ds_in_t;

int init_discovery(ds_in_t *in, ds_out_t *out);
void destroy_discovery();

#ifdef __cplusplus
}
#endif
