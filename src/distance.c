#include <stdlib.h>
#include <stdio.h>
#include <pigpio.h>
#include "distance.h"

void main(){
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



  gpioTerminate();
  
}
