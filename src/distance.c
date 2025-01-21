#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <pigpio.h>
#include <inttypes.h>
#include "distance.h"

int main(){
  uint32_t startT, endT, pulseT, micros;
  micros = 60;
  float distance;
  uint32_t times[2];

  struct timespec start;
  clock_gettime(CLOCK_MONOTONIC, &start);

  if (gpioInitialise() < 0)
  {
    perror("[ERROR]Couldn't initialize GPIO interface.");
    exit(1);
  }
 
  // From the GPIO 27 pin we are going to read (ECHO 3V)
  gpioSetMode(ECHO, PI_INPUT);
  // From the GPIO 25 pin we are going to send a TTL pulse (TRIG)
  gpioSetMode(TRIG, PI_OUTPUT);
  
  printf("GPIO %d level is: %d\n", ECHO, gpioRead(ECHO));
  // Sends a 10us pulse to the TRIG
while (1) {
  gpioTrigger(TRIG, 10, 1);

  while (gpioRead(ECHO) == 0) {}
  startT = gpioTick();
  while (gpioRead(ECHO) == 1) {}
  endT = gpioTick();
  
  pulseT = endT - startT;

  distance = (pulseT / 2) * 0.034;
  /*gpioSetAlertFuncEx(ECHO, flightTime, &start);*/
  /*gpioSleep(PI_TIME_RELATIVE, 3, 0);*/

  printf("Tempo di volo: %duS\nDistanza: %f\n", pulseT, distance);
  gpioSleep(PI_TIME_RELATIVE, 0, 60000);
}
  gpioTerminate();
}


/*void flightTime(int gpio, int level, uint32_t tick, void *userData) {*/
/*  uint32_t time = 0;*/
  /* uint32_t startT = 0; */
/*  //uint32_t * times = *((uint32_t**)userData);*/
  /* startT = *((uint32_t*)userData); */
  /* printf("%d\n %d\n", startT, tick); */
/*  struct timespec start = *((struct timespec * )userData);*/
/**/
/**/
/*  if (level == 1){*/
/*    //times[0] = tick;*/
/*    clock_gettime(CLOCK_MONOTONIC, &start);   */
/*  /*  if (startT < tick) {*/
/*      time = tick - startT;*/
/*      printf("Tempo di volo: %d\n", time);*/
/*    }*/
/*    else {*/
/*      time = (4294967295 - startT) + tick;*/
/*      printf("Tempo di volo else: %d\n", time);*/
/*    } */
/*  } else {*/
/*    //times[1] = tick;*/
/*    struct timespec end;*/
/*    clock_gettime(CLOCK_MONOTONIC, &end);*/
/*    printf("%ld.%ld\n", end.tv_sec - start.tv_sec, end.tv_nsec-start.tv_nsec);*/
/**/
/*  }*/
/*}*/
