#include "MyUart.h"
#include "atk_ms901m.h"

#define UART1_ID uart0
#define UART2_ID uart1
#define BAUD_RATE 115200

#define UART_TX_PIN_0 0
#define UART_RX_PIN_1 1
#define UART_TX_PIN_4 4
#define UART_RX_PIN_5 5

#define BUF_MAXLEN 20
uint8_t szbuf[BUF_MAXLEN];

//初始化串口
void Uart_Init()
{
    uart_init(UART1_ID, BAUD_RATE);
    gpio_set_function(UART_TX_PIN_0, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN_1, GPIO_FUNC_UART);

    uart_set_fifo_enabled(UART1_ID, true);

    uart_init(UART2_ID, BAUD_RATE);
    gpio_set_function(UART_TX_PIN_4, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN_5, GPIO_FUNC_UART);

    uart_set_fifo_enabled(UART2_ID, true);
}

//串口发送函数
// buf：缓冲区
// buflen：发送缓冲区长度
void Uart_Attitude_Data_Send()
{
    uart_write_blocking(UART1_ID, szbuf, 15);
}

//将姿态角数据打包
//数据包定义：FE FD 32bit_pitch 32bit_roll 32bit_yaw FF
//buf：缓冲区， len：缓冲区最大长度
//返回数据包长度
void Attitude_Data_Pack(float pitch, float roll, float yaw)
{

    szbuf[0] = 0xFE;
    szbuf[1] = 0xFD;

    szbuf[2] = ((uint32_t)pitch & 0b11111111000000000000000000000000)>> 24;
    szbuf[3] = ((uint32_t)pitch & 0b00000000111111110000000000000000)>> 16;
    szbuf[4] = ((uint32_t)pitch & 0b00000000000000001111111100000000)>> 8;
    szbuf[5] = ((uint32_t)pitch & 0b00000000000000000000000011111111);

    szbuf[6] = ((uint32_t)roll & 0b11111111000000000000000000000000)>> 24;
    szbuf[7] = ((uint32_t)roll & 0b00000000111111110000000000000000)>> 16;
    szbuf[8] = ((uint32_t)roll & 0b00000000000000001111111100000000)>> 8;
    szbuf[9] = ((uint32_t)roll & 0b00000000000000000000000011111111);

    szbuf[10] = ((uint32_t)yaw & 0b11111111000000000000000000000000)>> 24;
    szbuf[11] = ((uint32_t)yaw & 0b00000000111111110000000000000000)>> 16;
    szbuf[12] = ((uint32_t)yaw & 0b00000000000000001111111100000000)>> 8;
    szbuf[13] = ((uint32_t)yaw & 0b00000000000000000000000011111111);

    szbuf[14] = 0xFF;

}

//通过串口发送姿态角数据包
//结果测试上述通过消息包的方式不可行，存在未知bug，故放弃
void Send_Attitude_Date(float pitch, float roll, float yaw)
{
    printf("S%fA%fB%f\n", pitch, roll, yaw);
}

bool MPU9050_GET_DATA(float* pitch, float* roll, float* yaw)
{
    imu_data = uart_getc(UART2_ID);
    // uart_putc(UART1_ID, imu_data);
    data_phase();
    if(UART2_Process_Flag == Process_run)
    {
        if(ProcessIMU_FrameBuf())
        {
            *pitch = Attitude.pitch;
            *roll = Attitude.roll;
            *yaw = Attitude.yaw;
            return true;
        }
    }
    return false;
}