#ifndef _PID_
#define _PID_

typedef struct PID_t
{
    float p;
    float i;
    float d;
    float last_p;
    float k_p;
    float k_i;
} PID_t;

void pid_init(PID_t* pid, float k_p, float k_i);
void pid_update(PID_t* pid, float data);
float pid_output(PID_t* pid);

#endif