//
// Created by f3m on 04/04/26.
//

#include <cstdlib>
#include <netlib.h>
#include <discovery.h>
#include <clock_sync.h>
#include <lidar_data.hpp>

typedef struct autonet_attr
{
    ds_in_t discovery_prv_attr;
    ds_out_t discovery_attr;
    cs_in_t clock_sync_prv_attr;
    cs_out_t clock_sync_attr;
	ld_in_t lidar_data_prv_attr;
	ld_out_t lidar_data_attr;
}autonet_attr_t;



void cb(uint8_t *data, size_t length)
{}

int main()
{
    autonet_attr_t attr = {};
    pthread_mutex_t net_mutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_cond_t net_cond = PTHREAD_COND_INITIALIZER;

    net_set_callback(cb);
    net_init();
    net_start_recv_loop();

    attr.discovery_prv_attr.net_mutex = &net_mutex;
    attr.discovery_prv_attr.net_cond = &net_cond;
    attr.discovery_prv_attr.net_broadcast = net_broadcast;

    init_discovery(&attr.discovery_prv_attr, &attr.discovery_attr);

    attr.clock_sync_prv_attr.net_mutex = &net_mutex;
    attr.clock_sync_prv_attr.net_cond = &net_cond;
    attr.clock_sync_prv_attr.disc_attr = &attr.discovery_attr;

    init_clock_sync(&attr.clock_sync_prv_attr, &attr.clock_sync_attr);





    destroy_clock_sync();
    destroy_discovery();
    net_destroy();
    pthread_mutex_destroy(&net_mutex);
    pthread_cond_destroy(&net_cond);
    return EXIT_FAILURE;
}