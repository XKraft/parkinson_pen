#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/i2c.h"
#include "stdlib.h"

void mpu6050_reset();
void mpu6050_read_raw(int16_t accel[3], int16_t gyro[3], int16_t *temp);
uint8_t check_mpu6050();
void wakeup_mpu6050();
uint8_t mpu6050_who_I_am();
void set_mpu6050_acc_config();
void set_mpu6050_gyro_config();
void read_mpu6050_acc_config();
void read_mpu6050_gyro_config();
void calculate_attitude(int16_t accel[3], int16_t gyro[3], float acc[3], float _gyro[3], float pitch, float yaw, float roll);
uint8_t mpu6050_write(uint8_t slave_addr, uint8_t reg_addr, uint8_t len, uint8_t* buf);
uint8_t mpu6050_read(uint8_t slave_addr, uint8_t reg_addr, uint8_t len, uint8_t* buf);