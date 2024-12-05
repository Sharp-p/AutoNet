#include <stdlib.h>
#include <stdio.h>
#include <pigpio.h>

void main(){
  if (gpioInitialise() < 0)
  {
    perror("[ERROR]Couldn't initialize GPIO interface.");
    exit(1);
  }
  // From the GPIO 16 pin we are going to read (ECHO 3V)
  gpioSetMode(16, PI_INPUT);
  // From the GPIO 5 pin we are going to send a TTL pulse (TRIG)
  gpioSetMode(5, PI_OUTPUT);



  gpioTerminate();
  
}
