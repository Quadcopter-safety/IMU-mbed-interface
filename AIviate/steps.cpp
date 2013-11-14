#include "steps.h"

Serial pc(USBTX, USBRX);

// in the future, change get_sensor_data to append the sensor data to a rolling list 
int get_sensor_data(int pid)
{
    struct sensor s;
    
    // retrieve process memory pointer
    char* mem = get_output(pid);
    int gx=0, gy=0, gz=0;
    if (sscanf(mem, "gx0 %i gy0 %i gz0 %i;\r\n", &gx, &gy, &gz) != 3)
    {
        // probably first time running process, retrieve data from init_sensors and store in current process output buffer
        
    }
    else
    {
        s.gx0 = gx;
        s.gy0 = gy;
        s.gz0 = gz;    
    }
    

    if (read_accelerometer(&s) == 0)
    {
        if (USBDEBUG)
            pc.printf("Error in get_sensor_data while reading from accel!\r\n");
        return 1;
    }
    if (read_gyro(&s) == 0)
    {
        if (USBDEBUG)
            pc.printf("Error in get_sensor_data while reading from gyro!\r\n");
        return 1;
    }
    // updated output format for matlab 
    if (USBDEBUG)
        //pc.printf("%i,%i,%i\r\n", s.ax, s.ay, s.az);
        pc.printf("%i,%i,%i,%i,%i,%i\r\n", s.ax, s.ay, s.az, s.gx, s.gy, s.gz);

     
    //if (USBDEBUG)
     //   pc.printf("Ax: %i Ay: %i Az: %i Gx: %i Gy: %i Gz: %i\r\n", s.ax, s.ay, s.az, s.gx, s.gy, s.gz);
    return 1;
}

int init_sensors(int pid)
{
    // create config struct
    struct config c;
    
    // set configurations
    c.frequency = 10000;
    
    // pass configuration struct to configuration routine
    int ret = config_gy80(&c);
    if (ret == 0)
    {
        if (USBDEBUG)
            pc.printf("Error configuring sensors\r\n");
    }
    
    // accumulations of gyro values (for zeroing)
    int gx = 0, gy = 0, gz = 0;
    struct sensor s;
    int numzerotrials = 10;
    for (int i=0; i<numzerotrials; i++)
    {
        if (read_gyro(&s) == 0)
        {
            if (USBDEBUG)
                pc.printf("Error in collecting zero-level data from gyro (init_sensors)\r\n");
            return 1;
        }
        gx += s.gx;
        gy += s.gy;
        gz += s.gz;  
    }
    
    gx /= numzerotrials;
    gy /= numzerotrials;
    gz /= numzerotrials;
    
    char * output = get_output(pid);
    sprintf(output, "gx0 %i gy0 %i gz0 %i;\r\n", gx, gy, gz);
    
    schedule_proc("RETDATA", &get_sensor_data);
    return 0;
}
