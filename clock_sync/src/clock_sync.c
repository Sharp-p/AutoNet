//
// Created by f3m on 04/04/26.
//

#include <clock_sync.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


clock_sync_attr_t *attr = nullptr;
clock_sync_prv_attr_t *prv_attr = nullptr;


void *clock_sync_loop(void *args)
{

}


int init_clock_sync(clock_sync_prv_attr_t *priv_attributes, clock_sync_attr_t *attributes)
{
    attr = attributes;
    prv_attr = priv_attributes;

    prv_attr->stop = false;
    const int err = pthread_create(&prv_attr->thread, nullptr, clock_sync_loop, nullptr);
    if (err != 0)
    {
        fprintf(stderr, "pthread_create failed: %s\n", strerror(err));
        return EXIT_FAILURE;
    }
    printf("[CLOCK SYNC] Clock sync initialized!\n");
    return EXIT_SUCCESS;
}
void destroy_clock_sync()
{
    prv_attr->stop = true;
    pthread_join(prv_attr->thread, nullptr);
}