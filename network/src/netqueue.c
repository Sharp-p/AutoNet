//
// Created by f3m on 12/03/26.
//

#include <netqueue.h>
#include <stdio.h>
#include <string.h>


void destroyQueueList(Queuelist_t *queuelist)
{
    for (int i = 0; i < QUEUE_SIZE; ++i)
    {
        if (queuelist->discovery.buf[i] != NULL)
            free(queuelist->discovery.buf[i]);
    }
    memset(queuelist->discovery.buf, 0, sizeof(queuelist->discovery.buf));
    memset(queuelist->discovery.len, 0, sizeof(queuelist->discovery.buf));
    queuelist->discovery.index_r = 0;
    queuelist->discovery.index_w = 0;
}


int pushQueue(Netqueue_t *queue, uint8_t *buf, const size_t len)
{
    if ((queue->index_w + 1) % QUEUE_SIZE == queue->index_r)
    {
        fprintf(stderr, "Queue full, dropping\n");
        return EXIT_FAILURE;
    }

    queue->buf[queue->index_w] = buf;
    queue->len[queue->index_w] = len;
    queue->index_w = (queue->index_w + 1) % QUEUE_SIZE;

    return EXIT_SUCCESS;
}
int popQueue(Netqueue_t *queue, uint8_t **buf, size_t *len)
{
    if (queue->len[queue->index_r] == 0)
        return EXIT_FAILURE; //Empty

    *buf = queue->buf[queue->index_r];
    *len = queue->len[queue->index_r];
    queue->index_r = (queue->index_r + 1) % QUEUE_SIZE;

    queue->buf[queue->index_r] = nullptr;
    queue->len[queue->index_r] = 0;
    return EXIT_SUCCESS;
}
