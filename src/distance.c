#include <stdlib.h>
#include <stdio.h>
#include <pigpio.h>
#include <inttypes.h>
#include "distance.h"

int main(){
  uint32_t startT;

  uint32_t times[2];

  if (gpioInitialise() < 0)
  {
    perror("[ERROR]Couldn't initialize GPIO interface.");
    exit(1);
  }
  // From the GPIO 16 pin we are going to read (ECHO 3V)
  gpioSetMode(ECHO, PI_INPUT);
  // From the GPIO 5 pin we are going to send a TTL pulse (TRIG)
  gpioSetMode(TRIG, PI_OUTPUT);
  
  printf("GPIO %d level is: %d", ECHO, gpioRead(ECHO));
  // Sends a 10us pulse to the TRIG
  gpioTrigger(TRIG, 10, 1);
  startT = gpioTick();

  gpioSetAlertFuncEx(ECHO, flightTime, &times);
  gpioSleep(PI_TIME_RELATIVE, 3, 0);

  printf("Tempo di volo: %d\n", times[0]-times[1]);
  gpioTerminate();
}


void flightTime(int gpio, int level, uint32_t tick, void *userData) {
  uint32_t time = 0;
  uint32_t startT = 0;
  uint32_t * times = *((uint32_t**)userData);
  startT = *((uint32_t*)userData);
  printf("%d\n %d\n", startT, tick);

  if (level == 1){
    //times[0] = tick;
    if (startT < tick) {
      time = tick - startT;
      printf("Tempo di volo: %d\n", time);
    }
    else {
      time = (4294967295 - startT) + tick;
      printf("Tempo di volo else: %d\n", time);
    }
  } else {
    //times[1] = tick;
  }
}
