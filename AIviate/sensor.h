#ifndef SENSOR_H
#define SENSOR_H

#define DEBUG 1

#include "mbed.h"



#define accel_w 0xA6
#define ACCEL_X 0x32 // x: 0x32,0x33 y: 0x34,0x35 z: 0x36,0x37 Little Endian!!! (x = 0x33<<8|0x22). 2's complement 16bit
#define ACCEL_POWER_CTL 0x2D
#define gyro_w 0xD2
#define GYRO_X 0x28 // x: 0x28,0x29 y: 0x2A,0x2B z: 0x2C,0x2D Little Endian! (x = 0x28<<8|0x22). 2's complement 16bit
#define GYRO_CTRL_REG1 0x20
#define compass_w 0x3C
#define compass_r 0x3D
#define barometer_w 0xEE
#define barometer_r 0xEF

struct sensor
{
    int16_t ax, ay, az;
    int16_t gx, gy, gz;
    int16_t gx0, gy0, gz0;
    char raw_data[6];
};

struct config
{
    int frequency;
    int accel_resolution;
};


char set_i2c_pointer(char addr, char reg);   

int read(char addr, char reg, char *buf, int n);

int write(char addr, char reg, char *buf, int n);

int read_accelerometer(struct sensor* s);
int accelerometer_standby();
int accelerometer_measure();

int read_gyro(struct sensor* s);
int gyro_turnon();
int gyro_turnoff();

int read_compass(void);
int read_barometer(void);

int config_accelerometer(void);
int config_gyro();
int config_compass(void);
int config_barometer(void);

int config_gy80(struct config *c);

#endif //SENSOR_H