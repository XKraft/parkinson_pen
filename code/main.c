#include <stdio.h>
#include <string.h>
#include "mpu6050_i2c.h"
#include "inv_mpu.h"
#include "MyUart.h"
#include "kalman.h"
#include "pid.h"

int main() {
    stdio_init_all();
    
    //初始化串口
    Uart_Init();

    kalman_filter_t pitch_filter;
    kalman_filter_t roll_filter;
    kalman_filter_t yaw_filter;

    PID_t pitch_pid;
    PID_t roll_pid;
    PID_t yaw_pid;

    kaiman_filter_init(&pitch_filter, 0.01, 0.0002, 0.02);
    kaiman_filter_init(&roll_filter, 0.01, 0.0002, 0.02);
    kaiman_filter_init(&yaw_filter, 0.01, 0.0002, 0.02);

    pid_init(&pitch_pid, 20, 0);
    pid_init(&roll_pid, 20, 0);
    pid_init(&yaw_pid, 20, 0);

    // while(MPU6050_DMP_Init());

    float pitch, yaw, roll;

    while (1) {
        
        // MPU6050_DMP_Get_Data(&pitch, &roll, &yaw);
        // printf("pitch:%f, roll:%f, yaw:%f\n", pitch, roll, yaw);
        if(MPU9050_GET_DATA(&pitch, &roll, &yaw))
        {
            kalman_filter_run(&pitch_filter, pitch);
            kalman_filter_run(&roll_filter, roll);
            kalman_filter_run(&yaw_filter, yaw);

            pid_update(&pitch_pid, pitch - pitch_filter.x_now);
            pid_update(&roll_pid, roll - roll_filter.x_now);
            pid_update(&yaw_pid, yaw - yaw_filter.x_now);

            Send_Attitude_Date(pitch, pitch_filter.x_now, pid_output(&pitch_pid));
        }
            
        // sleep_ms(10);
    }
}