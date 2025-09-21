#ifndef SENSOR_H
#define SENSOR_H

#define TRIG 25
#define ECHO 27
#define ROTATION_TIME 5
#define MAX_MEAS_US 36000

float* flightTime(int* len);

typedef struct Node
{
    float val;
    struct Node* next;
} Node;

#endif
