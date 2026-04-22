//
// Created by tom on 16/04/26.
//

#pragma once

#include <pthread.h>
#include <autonet_types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ld_in
{
	bool stop;
    pthread_t thread;

    // TODO: lidar priv attr
}ld_in_t;

int init_lidar_data(ld_in_t *in, ld_out_t *out);
void destroy_lidar_data();

#ifdef __cplusplus
}
#endif