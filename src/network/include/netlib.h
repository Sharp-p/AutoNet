
#pragma once

#include <stdint.h>
#include <stdlib.h>

int netInit();
void netDestroy();

int netSend(uint8_t *buf, size_t len, uint8_t dst[6]);
int netBroadcast(uint8_t *buf, size_t len);

int netStartRecvLoop();
int netStopRecvLoop();

void setCallback(void (*cb)(uint8_t *, size_t));