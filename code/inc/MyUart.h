#include "hardware/uart.h"
#include "pico/stdlib.h"
#include "stdio.h"

void Uart_Init();
void Send_Attitude_Date(float pitch, float roll, float yaw);