#include "atk_ms901m.h"
#include <string.h>
#include <stdio.h>

/* ATK-MS901M UARTͨѶ֡ͷ */
#define HEAD_L 0x55        /* ͨ�õ�λ֡ͷ */
#define HEAD_UPLOAD_H 0x55 /* ��λ�����ϴ�֡ͷ */
#define FRAME_ID 0x01      /* ��̬������id */
#define HEAD_ACK_H 0xAF    /* ��λӦ��֡ͷ */

atk_ms901m_frame_t IMU_frame; // ���ݰ������ṹ��
atk_ms901m_attitude_data_t Attitude = {
    .roll = 0.0,
    .pitch = 0.0,
    .yaw = 0.0,
};
atk_ms901m_handle_state_t hd_state = wait_for_head_l;
// extern UART_HandleTypeDef huart5;
// extern DMA_HandleTypeDef hdma_uart5_rx;

uint8_t imu_data = 0;
// uint8_t UART2_Frame_Flag = Receive_stop; // �ж��Ƿ����ڽ���
uint8_t UART2_Process_Flag = Process_stop;
// uint8_t IMU_DMA_receive_flag = 0;
// uint8_t imu_buf[FRAME_DATA_MAX_SIZE + 5] = {0}; // ���ջ�����
uint8_t imu_cnt = 0;                            // ���ռ�����
// uint8_t temp1 = 0;
// uint8_t temp2 = 0;
uint8_t imu_checksum = 0;

/*�������ݰ������ṹ��*/
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
//  * @brief: ���У����Ƿ���ȷ
//  */
// uint8_t IsChecksum_OK(const uint8_t *Buf, uint8_t idea_checksum)
// {
//     uint8_t checksum_temp1 = 0;
//     uint8_t i = 0;
//     while (i < imu_cnt - 1) // ���������һ�ֽ�
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
 * @brief: ����֡���
 * @attention: �����ɺ���Ҫ�ֶ����������жϽ���
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
//     /***************����UART5���տ����ж�****************/
//     if ((huart->Instance == UART5) && (__HAL_UART_GET_FLAG(&huart5, UART_FLAG_IDLE) != RESET))
//     {
//         __HAL_UART_CLEAR_IDLEFLAG(&huart5);                                // ��������жϱ�־λ
//         HAL_UART_DMAStop(&huart5);                                         // ֹͣDMA����
//         imu_cnt = sizeof(imu_buf) - __HAL_DMA_GET_COUNTER(&hdma_uart5_rx); // ��ȡ�������ݵĳ���
//         ProcessIMU_FrameBuf();                                             // �����յ�����Ϣ��
//         HAL_UART_Receive_DMA(&huart5, imu_buf, sizeof(imu_buf));           // ������һ��DMA����
//     }
// }

// /**
//  * @brief: �����жϻص�����
//  * @attention: ���ж�����Ҫִ�еĽ϶࣬������Ӧ������
//  * @attention: ���������ڴ����ص������в����Զ�������һ���жϽ��գ���Ҫ�ڴ�����ɺ��ֶ�����
//  */
// void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
// {
//     if (huart->Instance == UART5 && UART5_Process_Flag == Process_stop)
//     /*����δ�ڴ������Խ��ս�������*/
//     {
//         if (imu_data == HEAD_L && temp1 != HEAD_L && UART5_Frame_Flag == Receive_stop)
//         /*�յ�����֡ͷ*/
//         {
//             temp1 = imu_data;
//         }
//         else if (imu_data == HEAD_UPLOAD_H && temp1 == HEAD_L && UART5_Frame_Flag == Receive_stop)
//         /*�յ�֡ͷ��λ����*/
//         {
//             temp2 = imu_data;
//         }
//         else if (imu_data == FRAME_ID && temp2 == HEAD_UPLOAD_H && UART5_Frame_Flag == Receive_stop)
//         /*�յ�����֡id*/
//         {
//             imu_buf[imu_cnt++] = temp1;
//             imu_buf[imu_cnt++] = temp2;
//             imu_buf[imu_cnt++] = imu_data;
//             imu_checksum += imu_buf[0];
//             imu_checksum += imu_buf[1];
//             imu_checksum += imu_buf[2];
//             UART5_Frame_Flag = Receive_run; // ��ʶ���ڽ���
//             // �������ݰ������ṹ��
//             IMU_frame.head_l = HEAD_L;
//             IMU_frame.head_h = HEAD_UPLOAD_H;
//             IMU_frame.id = FRAME_ID;
//         }
//         else if (UART5_Frame_Flag == Receive_run)
//         /*���������ݷ��뻺�����������֡β*/
//         {
//             imu_buf[imu_cnt++] = imu_data;
//             if (imu_cnt == 4)
//             /*���ݳ���*/
//             {
//                 IMU_frame.len = imu_data;
//                 imu_checksum += imu_data;
//             }
//             else if (imu_data == imu_checksum && imu_cnt == IMU_frame.len + 5)
//             /*��⵽֡β�ҳ���������������֡����*/
//             {
//                 IMU_frame.check_sum = imu_data;
//                 UART5_Frame_Flag = Receive_stop;
//                 UART5_Process_Flag = Process_run; // ��ȥ����
//             }
//             else if (imu_cnt > 11)
//             /*�����쳣�����������*/
//             {
//                 IMU_frame.check_sum = 0;
//                 UART5_Frame_Flag = Receive_stop;
//                 UART5_Process_Flag = Process_run; // ��ȥ����
//             }
//             else
//             /*�������ս����ݶ�*/
//             {
//                 IMU_frame.dat[imu_cnt - 5] = imu_data;
//                 imu_checksum += imu_data;
//             }
//         }
//         HAL_UART_Receive_IT(&huart5, &imu_data, 1); // ���¿����жϽ���
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
        // UART2_Frame_Flag = Receive_run; // ��ʶ���ڽ���
        // �������ݰ������ṹ��
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
            UART2_Process_Flag = Process_run; // ��ȥ����
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