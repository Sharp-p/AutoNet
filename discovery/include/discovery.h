//
// Created by f3m on 04/04/26.
//

#pragma once

#ifdef __cplusplus
extern "C" {
#endif


#include <pthread.h>
#include <stdint.h>
#include <autonet_types.h>

typedef struct ds_in
{
    pthread_mutex_t *net_mutex;
    pthread_cond_t *net_cond;
    int (*net_broadcast)(uint8_t *buf, size_t len);
    uint8_t *buffer[RECV_BUFFER_SIZE];
    size_t buffer_size[RECV_BUFFER_SIZE];
    uint8_t buffer_idx;
    pthread_t thread;
    bool stop;
} ds_in_t;

int init_discovery(ds_in_t *in, ds_out_t *out);
void destroy_discovery();

#ifdef __cplusplus
}
#endif