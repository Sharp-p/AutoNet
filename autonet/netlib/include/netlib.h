
#pragma once

#include <stdint.h>
#include <stdlib.h>

int net_init();
void net_destroy();

int net_send(uint8_t *buf, size_t len, uint8_t dst[6]);
int net_broadcast(uint8_t *buf, size_t len);

int net_start_recv_loop();
int net_stop_recv_loop();

void net_set_callback(void (*cb)(uint8_t *, size_t));