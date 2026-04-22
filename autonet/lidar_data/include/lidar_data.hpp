//
// Created by tom on 16/04/26.
//

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <pthread.h>

typedef struct lidar_data_prv_attr
{
    // TODO: lidar priv attr
	pthread_mutex_t *net_mutex;
	pthread_cond_t *net_cond;
	pthread_t thread;
	bool stop;

	void *lidar_driver;
	void *channel;

	void *raw_nodes_buffer;
	size_t raw_nodes_count;

	bool is_connected;
	bool is_motor_running;
}lidar_data_prv_attr_t;

int init_lidar_data(lidar_data_prv_attr_t *prv_attributes, lidar_data_t *attributes);
void destroy_lidar_data();

#ifdef __cplusplus
}
#endif