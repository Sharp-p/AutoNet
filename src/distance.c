#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <pigpio.h>
#include <inttypes.h>
#include "distance.h"


int main (int argc, char *argv[])
{
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
    int i = 0;
    // TODO: handle the end and start of the program in a better way (it should end after a 360° turn)
    while (i++ < 1000000)
    {
        // sends an impulse on the TRIG
        gpioTrigger(TRIG, 10, 1);

        // waits that it receives the impulse back
        while (gpioRead(ECHO) == 0)
        {}
        // measures the time it receives the signal for
        const uint32_t startT = gpioTick();
        while (gpioRead(ECHO) == 1)
        {
        }
        const uint32_t endT = gpioTick();

        const uint32_t pulseT = endT - startT;

        const float distance = (float) ((float) pulseT / 2 * 0.034);
        /*gpioSetAlertFuncEx(ECHO, flightTime, &start);*/
        /*gpioSleep(PI_TIME_RELATIVE, 3, 0);*/

        printf("Tempo di volo: %duS\nDistanza: %f\n", pulseT, distance);
        gpioSleep(PI_TIME_RELATIVE, 0, 60000);
    }
    gpioTerminate();
}
