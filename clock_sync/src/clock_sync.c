//
// Created by f3m on 04/04/26.
//

#include <clock_sync.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


cs_out_t *cs_out = nullptr;
cs_in_t *cs_in = nullptr;


void *clock_sync_loop(void *args)
{

}


int init_clock_sync(cs_in_t *in, cs_out_t *out)
{
    cs_out = out;
    cs_in = in;

    cs_in->stop = false;
    const int err = pthread_create(&cs_in->thread, nullptr, clock_sync_loop, nullptr);
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
    cs_in->stop = true;
    pthread_join(cs_in->thread, nullptr);
}