#include <stdio.h>
#include <string.h>
#include "mpu6050_i2c.h"
#include "inv_mpu.h"
#include "MyUart.h"

int main() {
    stdio_init_all();
    
    //初始化串口
    Uart_Init();

    // while(MPU6050_DMP_Init());

    float pitch, yaw, roll;

    while (1) {
        
        // MPU6050_DMP_Get_Data(&pitch, &roll, &yaw);
        // printf("pitch:%f, roll:%f, yaw:%f\n", pitch, roll, yaw);
        if(MPU9050_GET_DATA(&pitch, &roll, &yaw))
            Send_Attitude_Date(pitch, roll, yaw);
        // sleep_ms(10);
    }
}