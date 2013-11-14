#include "control.h"
#include "sensor.h"
#include "steps.h"
#include "mbed.h"
#include "IMUfilter.h"

#define MAXPROC 15
IMUfilter imuFilter(0.1, 10);


process procs[MAXPROC] = {0};
Serial pc3(USBTX, USBRX);
LocalFileSystem local("local");
int main()
{

    init();
    pc3.baud(115200);
    accelerometer_measure();
    gyro_turnon();
    sensor ter;
    int a;
    while (true)
    {
        schedule();
        a =  read_accelerometer(&ter);
        a = read_gyro(&ter);
        
          wait(1);
        //Gyro and accelerometer values must be converted to rad/s and m/s/s
        //before being passed to the filter.
     /* imuFilter.updateFilter(ter.gx,
                               ter.gy,
                               ter.gz,
                               ter.ax,
                               ter.ay,
                               ter.az);
        imuFilter.computeEuler();
        printf("%f, %f, %f\n", imuFilter.getRoll(), imuFilter.getPitch(), imuFilter.getYaw());
    
    */
    
       
    }
;
}

void init()
{
    // initialize i2c sensors
    schedule_proc("INITSENSE", &init_sensors);
    // set initial processes
    procs[0].status = READY;
    procs[0].start = &get_sensor_data;
    return;
}

char* get_output(int pid)
{
    return procs[pid].output;
}

void schedule()
{
    for (int i=0; i<MAXPROC; i++)
    {
        process proc = procs[i];
        if(proc.status == READY)
        {
            int ret = proc.start(i);
            // if the process returns 0, it means don't run again
            if (ret == 0)
            {
                // set proc.status to ZOMBIE
                // ZOMBIE means process
                // is no longer active, but
                // it's output buffer is still
                // needed.
                proc.status = ZOMBIE;
            }
            return;
        }
     }
}

int schedule_proc(char *sid, int (*funcptr)(int))
{
    for (int i=0; i<MAXPROC; i++)
    {
        process proc = procs[i];
        if(proc.status == EMPTY)
        {
            proc.status = READY;
            proc.start = funcptr;
            strncpy(proc.sid, sid, MAX_SID_LEN-1);
            // null terminate proc.sid
            proc.sid[MAX_SID_LEN-1] = 0;
            return i;
         }
     }
     // if no open processes, return -1
     return -1;
}