//
// Created by f3m on 04/04/26.
//

#pragma once

#ifdef __cplusplus
extern "C" {
#endif


#include <autonet_types.h>
#include <pthread.h>

typedef struct cs_in
{
    pthread_mutex_t *net_mutex;
    pthread_cond_t *net_cond;
    ds_out_t *disc_attr;
    pthread_t thread;
    bool stop;
} cs_in_t;


int init_clock_sync(cs_in_t *in, cs_out_t *out);
void destroy_clock_sync();


#ifdef __cplusplus
}
#endif