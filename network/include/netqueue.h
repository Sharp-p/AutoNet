//
// Created by f3m on 12/03/26.
//

#pragma once

#ifdef __cplusplus
extern "C" {
#endif


#include <stdint.h>
#include <stdlib.h>

typedef struct Netqueue
{
    uint8_t *buf[QUEUE_SIZE];
    size_t len[QUEUE_SIZE];
    uint8_t index_w;
    uint8_t index_r;
} Netqueue_t;
typedef struct Queuelist
{
    Netqueue_t discovery;
} Queuelist_t;


void destroyQueuelist(Queuelist_t *queuelist);

int pushQueue(Netqueue_t *queue, uint8_t *buf, size_t len);
int popQueue(Netqueue_t *queue, uint8_t **buf, size_t *len);


#ifdef __cplusplus
}
#endif