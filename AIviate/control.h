#ifndef CONTROL_H
#define CONTROL_H

#define OUTPUT_SIZE 512
#define MAX_SID_LEN 32

char* get_output(int);
void init();
void schedule();
int schedule_proc(char *sid, int (*funcptr)(int));

typedef enum
{
    READY,
    BLOCKED,
    ZOMBIE,
    EMPTY
} proc_stat;

typedef struct
{
    // process status
    unsigned int status;
    // if status == BLOCKED, wait till block_pid's status is ZOMBIE
    unsigned int block_pid;
    // function. each "process" is a function with a number of properties and storage space
    int (*start)(int);
    char sid[MAX_SID_LEN];
    char output[OUTPUT_SIZE];
} process;

#endif //CONTROL_H