#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <pigpio.h>
#include <inttypes.h>
#include "distance.h"
#include <stdbool.h>




float* flightTime(int* len)
{
	Node* head = NULL;
	Node* tail = NULL;

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
    const uint32_t rotationT = gpioTick();
    // TODO: handle the end and start of the program in a better way (it should end after a 360° turn)
    // ends execution after ROTATION_TIME seconds
    while (gpioTick() - rotationT < ROTATION_TIME * 1000000)
    {
		gpioWrite(TRIG, 0);
		gpioDelay(5);

		//printf("Valore TRIG: %d\n", gpioRead(TRIG));
		//printf("Valore ECHO: %d\n", gpioRead(ECHO));
	        // sends an impulse on the TRIG
	    if (gpioTrigger(TRIG, 10, 1)) {
			printf("Trigger error\n");
			break;
		}
	
		// time variable for timeouts and measurements
		uint32_t startT = gpioTick();
		uint32_t endT = gpioTick();
		bool timeout = false;

		// waits that it sends the 8 cycle ultrasonic burst
		while (gpioRead(ECHO) == 0)
        {
			if (endT - startT > MAX_MEAS_US) {
				timeout = true;
				break;
			}
			endT = gpioTick();
			//printf("Pin ECHO (%d) is LOW\n", ECHO);
		}
		if (timeout) {
			printf("[WARNING]Timeout on ECHO's rising edge.\n");
			continue;
		}

		//printf("Burst sent, starting measurment\n");
	    
		// measures the time it receives the signal for
	    startT = gpioTick();
	    endT = gpioTick();
		timeout = false;
		
		while (gpioRead(ECHO) == 1)
        {
			//printf("Pin ECHO (%d) is HIGH\n", ECHO);
			if (endT - startT > MAX_MEAS_US) {
				timeout = true;
				break;
			}
			endT = gpioTick();
        }

		float distance;
		if (timeout) {
			printf("[WARNING]Timeout on ECHO's falling edge.\n");
			distance = -1;
		}
		else {
        	const uint32_t pulseT = endT - startT;

        	distance = (float) ((float) pulseT / 2 * 0.034);
		}

    	Node* temp = malloc(sizeof(Node));
    	if (!temp) {
    		perror("[ERROR]Couldn't allocate memory for node.");
    		exit(1);
    	}
    	temp->val = distance;
    	temp->next = NULL;

    	if (tail) {
    		tail->next = temp;
    	} else {
    		head = temp;
    	}
    	tail = temp;
		printf("[INFO]Measurement %d complete!\n", *len + 1);
    	(*len)++;

        //printf("Tempo di volo: %duS\nDistanza: %f\n", pulseT, distance);
        gpioSleep(PI_TIME_RELATIVE, 0, 60000);
    }
    gpioTerminate();

	// creation of the array
	float* measures = malloc((*len) * sizeof(float));
	if (!measures) {
		perror("[ERROR]Couldn't allocate memory for measures.");
		exit(1);
	}

	// population of the array and deallocation of the linked list
	Node* curr = head;
	for (int i = 0; i < *len; i++) {
		measures[i] = curr->val;
		Node* temp = curr;
		curr = curr->next;
		free(temp);
	}
	
	return measures;
}
