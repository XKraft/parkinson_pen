#include"pid.h"

void pid_init(PID_t* pid, float k_p, float k_i)
{
    pid->p = 0;
    pid->i = 0;
    pid->d = 0;
    pid->k_i = k_i;
    pid->k_p = k_p;
    pid->last_p = 0;
}

void pid_update(PID_t* pid, float data)
{
    pid->last_p = pid->p;

    pid->p = data;
    pid->i = pid->p - pid->last_p;
}

float pid_output(PID_t* pid)
{
    return pid->k_p * pid->p + pid->k_i * pid->i;
}