//
// Created by f3m on 04/04/26.
//

#include <cstdlib>
#include <netlib.h>
#include <discovery.h>
#include <clock_sync.h>
#include <lidar_data.hpp>

typedef struct autonet_ctx
{
    ds_in_t ds_in;
    ds_out_t ds_out;
    cs_in_t cs_in;
    cs_out_t cs_out;
	ld_in_t ld_in;
	ld_out_t ls_out;
}autonet_ctx_t;



void cb(uint8_t *data, size_t length)
{}

int main()
{
    autonet_ctx_t ctx = {};

    //init del network
    pthread_mutex_t net_mutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_cond_t net_cond = PTHREAD_COND_INITIALIZER;
    net_set_callback(cb);
    net_init();
    net_start_recv_loop();

    //init del discovery
    ctx.ds_in.net_mutex = &net_mutex;
    ctx.ds_in.net_cond = &net_cond;
    ctx.ds_in.net_broadcast = net_broadcast;
    init_discovery(&ctx.ds_in, &ctx.ds_out);

    //init del clock sync
    ctx.cs_in.net_mutex = &net_mutex;
    ctx.cs_in.net_cond = &net_cond;
    ctx.cs_in.disc_attr = &ctx.ds_out;
    init_clock_sync(&ctx.cs_in, &ctx.cs_out);

    //init del lidar
    init_lidar_data(&ctx.ld_in, &ctx.ls_out);





    destroy_lidar_data();
    destroy_clock_sync();
    destroy_discovery();
    net_destroy();
    pthread_mutex_destroy(&net_mutex);
    pthread_cond_destroy(&net_cond);
    return EXIT_FAILURE;
}