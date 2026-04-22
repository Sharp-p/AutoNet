//
// Created by lab on 16/04/26.
//

#include <lidar_data.hpp>
#include <sl_lidar.h>
#include <cstring>

ld_out *ld_out = nullptr;
ld_in_t *ld_in = nullptr;

void *lidar_loop(void *args)
{
    return EXIT_SUCCESS;
}


int init_discovery(ld_in_t *in, ld_out_t *out)
{
    ld_out = out;
    ld_in = in;

    ld_in->stop = false;

    if (const int err = pthread_create(&ld_in->thread, nullptr, lidar_loop, nullptr); err != 0)
    {
        fprintf(stderr, "pthread_create failed: %s\n", strerror(err));
        return EXIT_FAILURE;
    }
    printf("[DISCOVERY] Discovery initialized!\n");
    return EXIT_SUCCESS;
}
void destroy_discovery()
{
    ld_in->stop = true;
    pthread_join(ld_in->thread, nullptr);
}