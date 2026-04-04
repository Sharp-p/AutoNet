//
// Created by f3m on 04/04/26.
//

#include <discovery.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

discovery_attr_t *attr = nullptr;
discovery_prv_attr_t *prv_attr = nullptr;

void *discovery_loop(void *a)
{
    return EXIT_SUCCESS;
}


int init_discovery(discovery_prv_attr_t *prv_attributes, discovery_attr_t *attributes)
{
    attr = attributes;
    prv_attr = prv_attributes;

    prv_attr->buffer_idx = 0;
    prv_attr->stop = false;

    const int err = pthread_create(&prv_attr->thread, nullptr, discovery_loop, nullptr);
    if (err != 0)
    {
        fprintf(stderr, "pthread_create failed: %s\n", strerror(err));
        return EXIT_FAILURE;
    }
    printf("[DISCOVERY] Discovery initialized!\n");
    return EXIT_SUCCESS;
}
void destroy_discovery()
{
    prv_attr->stop = true;
    pthread_join(prv_attr->thread, nullptr);
}
