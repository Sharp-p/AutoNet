#include <stdlib.h>
#include <stdio.h>
#include <pigpio.h>
#include <inttypes.h>
#include "distance.h"

void main(){
  uint32_t startT;

  if (gpioInitialise() < 0)
  {
    perror("[ERROR]Couldn't initialize GPIO interface.");
    exit(1);
  }
  // From the GPIO 16 pin we are going to read (ECHO 3V)
  gpioSetMode(ECHO, PI_INPUT);
  // From the GPIO 5 pin we are going to send a TTL pulse (TRIG)
  gpioSetMode(TRIG, PI_OUTPUT);

  // Sends a 10us pulse to the TRIG
  gpioTrigger(TRIG, 10, 1);
  startT = gpioTick();

  gpioSetAlertFuncEx(ECHO, flightTime, &startT);

  gpioTerminate();
}


void* flightTime(int gpio, int level, uint32_t tick, uint32_t *startT) {
  uint32_t time = 0;

  if (*startT < tick) {
    time = tick - *startT;
    printf("Tempo di volo: " PRIu32 "\n", time);
  }
  else {
    time = (4294967295 - *startT) + tick;
    printf("Tempo di volo: " PRIu32 "\n", time);
  }
}