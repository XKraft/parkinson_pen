#ifndef __ATM_MS901M_H
#define __ATM_MS901M_H
#include "stdio.h"
/* ATK-MS901M UARTͨѶ֡������󳤶� */
#define FRAME_DATA_MAX_SIZE       64

/* ATK-MS901M�����ϴ�֡ID */
#define FRAME_ID_ATTITUDE        0x01    /* ��̬�� */
#define FRAME_ID_QUAT            0x02    /* ��Ԫ�� */
#define FRAME_ID_GYRO_ACCE       0x03    /* �����ǡ����ٶȼ� */
#define FRAME_ID_MAG             0x04    /* ������ */
#define FRAME_ID_BARO            0x05    /* ��ѹ�� */
#define FRAME_ID_PORT            0x06    /* �˿� */

/* ATK-MS901M֡���� */
#define ATK_MS901M_FRAME_ID_TYPE_UPLOAD     0       /* ATK-MS901M�����ϴ�֡ID */
#define ATK_MS901M_FRAME_ID_TYPE_ACK        1       /* ATK-MS901MӦ��֡ID */

typedef struct
{
    uint8_t head_l;                                 /* ��λ֡ͷ */
    uint8_t head_h;                                 /* ��λ֡ͷ */
    uint8_t id;                                     /* ֡ID */
    uint8_t len;                                    /* ���ݳ��� */
    uint8_t dat[FRAME_DATA_MAX_SIZE];     /* ���� */
    uint8_t check_sum;                              /* У��� */
} atk_ms901m_frame_t;                               /* ATK-MS901M UARTͨѶ֡�ṹ�� */

/* ��̬�����ݽṹ�� */
typedef struct
{
    float roll;                                     /* ����ǣ���λ���� */
    float pitch;                                    /* �����ǣ���λ���� */
    float yaw;                                      /* ����ǣ���λ���� */
} atk_ms901m_attitude_data_t;

/* ��Ԫ�����ݽṹ�� */
typedef struct
{
    float q0;                                       /* Q0 */
    float q1;                                       /* Q1 */
    float q2;                                       /* Q2 */
    float q3;                                       /* Q3 */
} atk_ms901m_quaternion_data_t;

/* ���������ݽṹ�� */
typedef struct
{
    struct
    {
        int16_t x;                                  /* X��ԭʼ���� */
        int16_t y;                                  /* Y��ԭʼ���� */
        int16_t z;                                  /* Z��ԭʼ���� */
    } raw;
    float x;                                        /* X����ת���ʣ���λ��dps */
    float y;                                        /* Y����ת���ʣ���λ��dps */
    float z;                                        /* Z����ת���ʣ���λ��dps */
} atk_ms901m_gyro_data_t;

/* ���ٶȼ����ݽṹ�� */
typedef struct
{
    struct
    {
        int16_t x;                                  /* X��ԭʼ���� */
        int16_t y;                                  /* Y��ԭʼ���� */
        int16_t z;                                  /* Z��ԭʼ���� */
    } raw;
    float x;                                        /* X����ٶȣ���λ��G */
    float y;                                        /* Y����ٶȣ���λ��G */
    float z;                                        /* Z����ٶȣ���λ��G */
} atk_ms901m_accelerometer_data_t;

/* ���������ݽṹ�� */
typedef struct
{
    int16_t x;                                      /* X��ų�ǿ�� */
    int16_t y;                                      /* Y��ų�ǿ�� */
    int16_t z;                                      /* Z��ų�ǿ�� */
    float temperature;                              /* �¶ȣ���λ���� */
} atk_ms901m_magnetometer_data_t;

/* ��ѹ�����ݽṹ�� */
typedef struct
{
    int32_t pressure;                               /* ��ѹ����λ��Pa */
    int32_t altitude;                               /* ���Σ���λ��cm */
    float temperature;                              /* �¶ȣ���λ���� */
} atk_ms901m_barometer_data_t;

typedef enum
{
    Receive_run,
    Receive_stop,
} Receive_status;

typedef enum
{
    Process_run,
    Process_stop,
} Process_status;

typedef enum
{
    wait_for_head_l = 0x00,                         /* 等待低位帧头 */
    wait_for_head_h = 0x01,                         /* 等待高位帧头 */
    wait_for_id     = 0x02,                         /* 等待帧ID */
    wait_for_len    = 0x04,                         /* 等待数据长度 */
    wait_for_dat    = 0x08,                         /* 等待数据 */
    wait_for_sum    = 0x16,                         /* 等待校验和 */
} atk_ms901m_handle_state_t;                        /* 帧处理状态机状态枚举 */

uint8_t ProcessIMU_FrameBuf(void);
// void IMU_IDLECallback(UART_HandleTypeDef *huart);
void data_phase();

extern uint8_t imu_data;
extern atk_ms901m_attitude_data_t Attitude;
extern uint8_t UART2_Process_Flag;
// extern uint8_t IMU_DMA_receive_flag;
// extern uint8_t imu_buf[FRAME_DATA_MAX_SIZE + 5]; // ���ջ�����
extern uint8_t imu_cnt;                            // ���ռ�����

#endif
