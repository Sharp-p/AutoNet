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
	pthread_mutex_t *net_mutex;
	pthread_cond_t *net_cond;

    // TODO: lidar priv attr
}ld_in_t;

int init_lidar_data(ld_in_t *prv_attributes, ld_out_t *attributes);
void destroy_lidar_data();

#ifdef __cplusplus
}
#endif