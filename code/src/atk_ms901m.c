#include "atk_ms901m.h"
#include <string.h>
#include <stdio.h>

/* ATK-MS901M UART通讯帧头 */
#define HEAD_L 0x55        /* 通用低位帧头 */
#define HEAD_UPLOAD_H 0x55 /* 高位主动上传帧头 */
#define FRAME_ID 0x01      /* 姿态角数据id */
#define HEAD_ACK_H 0xAF    /* 高位应答帧头 */

atk_ms901m_frame_t IMU_frame; // 数据包解析结构体
atk_ms901m_attitude_data_t Attitude = {
    .roll = 0.0,
    .pitch = 0.0,
    .yaw = 0.0,
};
atk_ms901m_handle_state_t hd_state = wait_for_head_l;
// extern UART_HandleTypeDef huart5;
// extern DMA_HandleTypeDef hdma_uart5_rx;

uint8_t imu_data = 0;
// uint8_t UART2_Frame_Flag = Receive_stop; // 判断是否正在接收
uint8_t UART2_Process_Flag = Process_stop;
// uint8_t IMU_DMA_receive_flag = 0;
// uint8_t imu_buf[FRAME_DATA_MAX_SIZE + 5] = {0}; // 接收缓冲区
uint8_t imu_cnt = 0;                            // 接收计数器
// uint8_t temp1 = 0;
// uint8_t temp2 = 0;
uint8_t imu_checksum = 0;

/*重置数据包解析结构体*/
void Clear_Frame()
{
    IMU_frame.head_l = 0;
    IMU_frame.head_h = 0;
    IMU_frame.id = 0;
    IMU_frame.len = FRAME_DATA_MAX_SIZE;
    memset(IMU_frame.dat, 0, sizeof(IMU_frame.dat));
    IMU_frame.check_sum = 0;

    imu_cnt = 0;
    // memset(imu_buf, 0, sizeof(imu_buf));
    // temp1 = 0;
    // temp2 = 0;
    imu_checksum = 0;
    UART2_Process_Flag = Process_stop;
}

// /**
//  * @brief: 检查校验和是否正确
//  */
// uint8_t IsChecksum_OK(const uint8_t *Buf, uint8_t idea_checksum)
// {
//     uint8_t checksum_temp1 = 0;
//     uint8_t i = 0;
//     while (i < imu_cnt - 1) // 不包含最后一字节
//     {
//         checksum_temp1 += Buf[i];
//         i++;
//     }
//     if (checksum_temp1 == idea_checksum)
//         return HAL_OK;
//     else
//         return HAL_ERROR;
// }

void output(const uint8_t *buf)
{
    size_t len = strlen((char *)buf);
    for (int i = 0; i < len; i++)
    {
        printf("%x ", buf[i]);
    }
}

/**
 * @brief: 数据帧解包
 * @attention: 解包完成后需要手动开启串口中断接收
 */
uint8_t ProcessIMU_FrameBuf()
{
    if(IMU_frame.id == FRAME_ID_ATTITUDE)
    {
        Attitude.roll = (float)((int16_t)(IMU_frame.dat[1] << 8) | IMU_frame.dat[0]) / 32768.0f * 180;
        Attitude.pitch = (float)((int16_t)(IMU_frame.dat[3] << 8) | IMU_frame.dat[2]) / 32768.0f * 180;
        Attitude.yaw = (float)((int16_t)(IMU_frame.dat[5] << 8) | IMU_frame.dat[4]) / 32768.0f * 180;
        Clear_Frame();
        return 1;
    }
    Clear_Frame();
    return 0;
}

// void IMU_IDLECallback(UART_HandleTypeDef *huart)
// {
//     /***************处理UART5接收空闲中断****************/
//     if ((huart->Instance == UART5) && (__HAL_UART_GET_FLAG(&huart5, UART_FLAG_IDLE) != RESET))
//     {
//         __HAL_UART_CLEAR_IDLEFLAG(&huart5);                                // 清除空闲中断标志位
//         HAL_UART_DMAStop(&huart5);                                         // 停止DMA传输
//         imu_cnt = sizeof(imu_buf) - __HAL_DMA_GET_COUNTER(&hdma_uart5_rx); // 获取接收数据的长度
//         ProcessIMU_FrameBuf();                                             // 处理收到的消息包
//         HAL_UART_Receive_DMA(&huart5, imu_buf, sizeof(imu_buf));           // 开启下一次DMA接收
//     }
// }

// /**
//  * @brief: 串口中断回调函数
//  * @attention: 在中断中需要执行的较多，波特率应该适中
//  * @attention: 若数据正在处理，回调函数中不会自动开启下一次中断接收，需要在处理完成后手动开启
//  */
// void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
// {
//     if (huart->Instance == UART5 && UART5_Process_Flag == Process_stop)
//     /*数据未在处理，可以接收进缓存区*/
//     {
//         if (imu_data == HEAD_L && temp1 != HEAD_L && UART5_Frame_Flag == Receive_stop)
//         /*收到嫌疑帧头*/
//         {
//             temp1 = imu_data;
//         }
//         else if (imu_data == HEAD_UPLOAD_H && temp1 == HEAD_L && UART5_Frame_Flag == Receive_stop)
//         /*收到帧头高位部分*/
//         {
//             temp2 = imu_data;
//         }
//         else if (imu_data == FRAME_ID && temp2 == HEAD_UPLOAD_H && UART5_Frame_Flag == Receive_stop)
//         /*收到数据帧id*/
//         {
//             imu_buf[imu_cnt++] = temp1;
//             imu_buf[imu_cnt++] = temp2;
//             imu_buf[imu_cnt++] = imu_data;
//             imu_checksum += imu_buf[0];
//             imu_checksum += imu_buf[1];
//             imu_checksum += imu_buf[2];
//             UART5_Frame_Flag = Receive_run; // 标识正在接收
//             // 放入数据包解析结构体
//             IMU_frame.head_l = HEAD_L;
//             IMU_frame.head_h = HEAD_UPLOAD_H;
//             IMU_frame.id = FRAME_ID;
//         }
//         else if (UART5_Frame_Flag == Receive_run)
//         /*将后续数据放入缓存区，并检测帧尾*/
//         {
//             imu_buf[imu_cnt++] = imu_data;
//             if (imu_cnt == 4)
//             /*数据长度*/
//             {
//                 IMU_frame.len = imu_data;
//                 imu_checksum += imu_data;
//             }
//             else if (imu_data == imu_checksum && imu_cnt == IMU_frame.len + 5)
//             /*检测到帧尾且长度正常，结束该帧接收*/
//             {
//                 IMU_frame.check_sum = imu_data;
//                 UART5_Frame_Flag = Receive_stop;
//                 UART5_Process_Flag = Process_run; // 拉去处理
//             }
//             else if (imu_cnt > 11)
//             /*发生异常，结束并标记*/
//             {
//                 IMU_frame.check_sum = 0;
//                 UART5_Frame_Flag = Receive_stop;
//                 UART5_Process_Flag = Process_run; // 拉去处理
//             }
//             else
//             /*正常接收进数据段*/
//             {
//                 IMU_frame.dat[imu_cnt - 5] = imu_data;
//                 imu_checksum += imu_data;
//             }
//         }
//         HAL_UART_Receive_IT(&huart5, &imu_data, 1); // 重新开启中断接收
//     }
// }

void data_phase()
{
    switch (hd_state)
    {
    case wait_for_head_l:
        if(imu_data == HEAD_L)
            hd_state = wait_for_head_h;
            break;
    case wait_for_head_h:
        if(imu_data == HEAD_UPLOAD_H)
            hd_state = wait_for_id;
        else
            hd_state = wait_for_head_l;
        break;
    case wait_for_id:
        imu_checksum += HEAD_L;
        imu_checksum += HEAD_UPLOAD_H;
        imu_checksum += imu_data;
        // UART2_Frame_Flag = Receive_run; // 标识正在接收
        // 放入数据包解析结构体
        IMU_frame.head_l = HEAD_L;
        IMU_frame.head_h = HEAD_UPLOAD_H;
        IMU_frame.id = imu_data;
        hd_state = wait_for_len;
        break;
    case wait_for_len:
        IMU_frame.len = imu_data;
        imu_checksum += imu_data;
        hd_state = wait_for_dat;
        break;
    case wait_for_dat:
        IMU_frame.dat[imu_cnt++] = imu_data;
        imu_checksum += imu_data;
        if(imu_cnt >= IMU_frame.len)
            hd_state = wait_for_sum;
        break;
    case wait_for_sum:
        if(imu_checksum == imu_data)
        {
            IMU_frame.check_sum = imu_data;
            UART2_Process_Flag = Process_run; // 拉去处理
            hd_state = wait_for_head_l;
        }
        else
        {
            hd_state = wait_for_head_l;
            Clear_Frame();
            UART2_Process_Flag = Process_stop;
        }
        break;
    default:
        hd_state = wait_for_head_l;
        Clear_Frame();
        break;
    }
}