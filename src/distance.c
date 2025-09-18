#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <pigpio.h>
#include <inttypes.h>
#include "distance.h"
#include <stdbool.h>



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
    const uint32_t rotationT = gpioTick();
    // TODO: handle the end and start of the program in a better way (it should end after a 360° turn)
    // ends execution after ROTATION_TIME seconds
    while (gpioTick() - rotationT < 2000000)
    {
	printf("Dif time: %d\n", gpioTick() - rotationT);
	gpioWrite(TRIG, 0);

	printf("Valore TRIG: %d\n", gpioRead(TRIG));
	printf("Valore ECHO: %d\n", gpioRead(ECHO));
        // sends an impulse on the TRIG
        if (gpioTrigger(TRIG, 10, 1)) {
		printf("Trigger error\n");
		break;
	}
	

        // waits that it sends the 8 cycle ultrasonic burst
        while (gpioRead(ECHO) == 0)
        {
		printf("Pin ECHO \(%d\) is LOW\n", ECHO);
	}
	printf("Burst sent, starting measurment\n");
        // measures the time it receives the signal for
        const uint32_t startT = gpioTick();
        uint32_t endT = gpioTick();
	bool timeout = false;
	while (gpioRead(ECHO) == 1)
        {
		if (endT - startT > 12080) { 
			timeout = true;
			break;
		}
		endT = gpioTick();
        }
	if (timeout) {
		// printf("Timeout raggiunto\n");
		continue; 
	}
        const uint32_t pulseT = endT - startT;

        const float distance = (float) ((float) pulseT / 2 * 0.034);
        
        printf("Tempo di volo: %duS\nDistanza: %f\n", pulseT, distance);
        gpioSleep(PI_TIME_RELATIVE, 0, 60000);
    }
    gpioTerminate();
}
