//
// Created by f3m on 04/04/26.
//

#pragma once

#ifdef __cplusplus
extern "C" {
#endif


#include <autonet_types.h>
#include <pthread.h>

typedef struct clock_sync_prv_attr
{
    pthread_mutex_t *net_mutex;
    pthread_cond_t *net_cond;
    discovery_attr_t *disc_attr;
    pthread_t thread;
    bool stop;
} clock_sync_prv_attr_t;


int init_clock_sync(clock_sync_prv_attr_t *priv_attributes, clock_sync_attr_t *attributes);
void destroy_clock_sync();


#ifdef __cplusplus
}
#endif