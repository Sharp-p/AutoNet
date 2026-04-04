//
// Created by f3m on 04/04/26.
//

#include <stdlib.h>
#include <discovery.h>
#include <netlib.h>

typedef struct autonet_attr
{
    discovery_prv_attr_t discovery_prv_attr;
    discovery_attr_t discovery_attr;
}autonet_attr_t;



void cb(uint8_t *data, size_t length);

int main()
{
    autonet_attr_t attr = {};
    pthread_mutex_t net_mutex = PTHREAD_MUTEX_INITIALIZER;

    net_set_callback(cb);
    net_init();
    net_start_recv_loop();

    attr.discovery_prv_attr.net_mutex = &net_mutex;
    attr.discovery_prv_attr.net_broadcast = net_broadcast;

    init_discovery(&attr.discovery_prv_attr, &attr.discovery_attr);





    destroy_discovery();
    net_destroy();
    pthread_mutex_destroy(&net_mutex);
    return EXIT_FAILURE;
}