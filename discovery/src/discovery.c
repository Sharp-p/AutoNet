//
// Created by f3m on 04/04/26.
//

#include <discovery.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

ds_out_t *ds_out = nullptr;
ds_in_t *ds_in = nullptr;

void *discovery_loop(void *args)
{
    return EXIT_SUCCESS;
}


int init_discovery(ds_in_t *in, ds_out_t *out)
{
    ds_out = out;
    ds_in = in;

    ds_in->buffer_idx = 0;
    ds_in->stop = false;

    const int err = pthread_create(&ds_in->thread, nullptr, discovery_loop, nullptr);
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
    ds_in->stop = true;
    pthread_join(ds_in->thread, nullptr);
}
