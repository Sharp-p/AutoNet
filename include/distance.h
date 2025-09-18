#ifndef SENSOR_H
#define SENSOR_H

#define TRIG 25
#define ECHO 27
#define ROTATION_TIME 2

void flightTime(int gpio, int level, uint32_t tick, void* userData);


#endif
