#ifndef STEPS_H
#define STEPS_H
#include "sensor.h"
#include "control.h"

#define USBDEBUG 1

int init_sensors(int);
int get_sensor_data(int);

#endif //STEPS_H