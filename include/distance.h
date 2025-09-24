#ifndef SENSOR_H
#define SENSOR_H

#define TRIG 25
#define ECHO 27
#define MEASUREMENT_TIME 25
#define MAX_MEAS_US 36000
#define CHANGE_THRESHOLD 2.0

float* flightTime(int* len);

typedef struct Node
{
    float val;
    struct Node* next;
} Node;

#endif
