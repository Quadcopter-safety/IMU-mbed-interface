#include "sensor.h"


extern Serial pc;
I2C i2c(p28, p27);

char set_i2c_pointer(char addr, char reg)
{
    if (i2c.write(addr) == 0)
    {
        if (DEBUG)
            pc.printf("Could not write device address (set_i2c_pointer)\r\n");
        return 0;
    }
    if (i2c.write(reg) == 0)
    {
        if (DEBUG)
            pc.printf("Could not write reg address (set_i2c_pointer)\r\n");
        return 0;
    }
    return 1;
}   

int read(char addr, char reg, char *buf, int n)
{
    i2c.start();
    if (set_i2c_pointer(addr, reg) == 0)
    {
        if (DEBUG)
            pc.printf("Could not set i2c pointer (read)\r\n");
        return 0;
    }
    if (i2c.read(addr|1, buf, n, true) != 0)
    {
        if (DEBUG)
            pc.printf("Could not execute read sequence (read)\r\n");
        return 0;
    }
    i2c.stop();
    return n;
}

int write(char addr, char reg, char *buf, int n)
{
    i2c.start();
    if (set_i2c_pointer(addr, reg) == 0)
    {
        if (DEBUG)
            pc.printf("Could not set i2c pointer (write)\r\n");
        return 0;
    }
    for (int i=0; i<n; i++)
    {
        if (i2c.write(buf[i]) == 0)
        {   
            i2c.stop();
            if (DEBUG)
                pc.printf("Only sent %i/%i bytes (write)\r\n", i, n);
            return i;
        }
    }
    i2c.stop();
    return n;
    
}

int read_accelerometer(struct sensor* s)
{
    int ret = read(accel_w, ACCEL_X, s->raw_data, 6);
    if (ret == 0)
    {
        pc.printf("Error, could not read (read_accelerometer)\r\n");
        return 0;
    }
    int16_t axlsb = (int16_t) s->raw_data[0];
    int16_t axmsb = (int16_t) s->raw_data[1];
    int16_t aylsb = (int16_t) s->raw_data[2];
    int16_t aymsb = (int16_t) s->raw_data[3];
    int16_t azlsb = (int16_t) s->raw_data[4];
    int16_t azmsb = (int16_t) s->raw_data[5];

    s->ax = ((axmsb << 8) + axlsb);
    s->ay = ((aymsb << 8) + aylsb);
    s->az = ((azmsb << 8) + azlsb);
    return 1;
}

// disable accelerometer to save power
int accelerometer_standby()
{
    char power_ctl;
    int ret = read(accel_w, ACCEL_POWER_CTL, &power_ctl, 1);
    if (ret == 0)
    {
        if (DEBUG)
            pc.printf("Error putting accelerometer in standby (accelerometer_standby)\r\n");
        return 0;
    }
    power_ctl &= 0xF7 ;
    ret = write(accel_w, ACCEL_POWER_CTL, &power_ctl, 1);
    if (ret == 0)
    {
        if (DEBUG)
            pc.printf("Error putting accelerometer in standby (accelerometer_standby)\r\n");
        return 0;
    }
    return 1;
}

// enable accelerometer for measurements
int accelerometer_measure()
{
    char power_ctl;
    int ret = read(accel_w, ACCEL_POWER_CTL, &power_ctl, 1);
    if (ret == 0)
    {
        if (DEBUG)
            pc.printf("Error putting accelerometer in measure mode (accelerometer_measure)\r\n");
        return 0;
    }
    power_ctl |= 0x8 ;
    ret = write(accel_w, ACCEL_POWER_CTL, &power_ctl, 1);
    if (ret == 0)
    {
        if (DEBUG)
            pc.printf("Error putting accelerometer in measure mode (accelerometer_measure)\r\n");
        return 0;
    }    
    return 1;
}

int gyro_turnon()
{
    char power_ctl;
    int ret = read(gyro_w, GYRO_CTRL_REG1, &power_ctl, 1);
    if (ret == 0)
    {
        if (DEBUG)
            pc.printf("Error turning on gyro (gyro_turnon)\r\n");
        return 0;
    }
    power_ctl |= 0x8 ;
    ret = write(gyro_w, GYRO_CTRL_REG1, &power_ctl, 1);
    if (ret == 0)
    {
        if (DEBUG)
            pc.printf("Error turning on gyro (gyro_turnon)\r\n");
        return 0;
    }    
    return 1;
}

int gyro_turnoff()
{
    char power_ctl;
    int ret = read(gyro_w, GYRO_CTRL_REG1, &power_ctl, 1);
    if (ret == 0)
    {
        if (DEBUG)
            pc.printf("Error turning off gyro (gyro_turnoff)\r\n");
        return 0;
    }
    power_ctl &= 0xF7 ;
    ret = write(gyro_w, GYRO_CTRL_REG1, &power_ctl, 1);
    if (ret == 0)
    {
        if (DEBUG)
            pc.printf("Error turning off gyro (gyro_turnoff)\r\n");
        return 0;
    }
    return 1;
}

int read_gyro(struct sensor* s)
{
    int ret = read(gyro_w, GYRO_X, s->raw_data, 6);
    if (ret == 0)
    {
        pc.printf("Error, could not read (read_gyro)\r\n");
        return 0;
    }
    int16_t gxlsb = (int16_t) s->raw_data[0];
    int16_t gxmsb = (int16_t) s->raw_data[1];
    int16_t gylsb = (int16_t) s->raw_data[2];
    int16_t gymsb = (int16_t) s->raw_data[3];
    int16_t gzlsb = (int16_t) s->raw_data[4];
    int16_t gzmsb = (int16_t) s->raw_data[5];

    s->gx = ((gxmsb << 8) + gxlsb);
    s->gy = ((gymsb << 8) + gylsb);
    s->gz = ((gzmsb << 8) + gzlsb);
    return 1;
}
int read_compass(void){return 0;}
int read_barometer(void){return 0;}

int config_accelerometer(void)
{
    // take accelerometer out of standby mode
    int ret = accelerometer_measure();
    if (ret == 0)
    {
        if (DEBUG)
            pc.printf("Error starting up accelerometer\r\n");
        return 0;
    }
    return 8;
}
int config_gyro()
{
    // turn on the gyro via i2c
    int ret = gyro_turnon();
    if (ret == 0)
    {
        if (DEBUG)
            pc.printf("Error starting up gyro\r\n");
        return 0;
    }
    return 4;
}
int config_compass(void){return 2;}
int config_barometer(void){return 1;}

int config_gy80(struct config *c)
{
    // return value is a 4-bit number: AGCB, indicating
    // the return values of accel, gyro, compass, and barometer
    i2c.frequency(c->frequency);
    int ret = config_accelerometer();
    ret |= config_gyro();
    ret |= config_compass();
    ret |= config_barometer();
    return ret;
}