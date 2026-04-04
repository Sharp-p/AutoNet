//
// Created by f3m on 04/04/26.
//

#pragma once

#ifdef __cplusplus
extern "C" {
#endif


#include <pthread.h>
#include <stdint.h>

typedef struct discovery_attr
{
    //TODO: i risultati
}discovery_attr_t;
typedef struct discovery_prv_attr
{
    pthread_mutex_t *net_mutex;
    int (*net_broadcast)(uint8_t *buf, size_t len);
    uint8_t *buffer[RECV_BUFFER_SIZE];
    size_t buffer_size[RECV_BUFFER_SIZE];
    uint8_t buffer_idx;
    pthread_t thread;
    bool stop;
} discovery_prv_attr_t;

int init_discovery(discovery_prv_attr_t *prv_attributes, discovery_attr_t *attributes);
void destroy_discovery();

#ifdef __cplusplus
}
#endif