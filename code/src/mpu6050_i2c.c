#include "mpu6050_i2c.h"

#define GRAVITY_ACC 9.8

/* Example code to talk to a MPU6050 MEMS accelerometer and gyroscope

   This is taking to simple approach of simply reading registers. It's perfectly
   possible to link up an interrupt line and set things up to read from the
   inbuilt FIFO to make it more useful.

   NOTE: Ensure the device is capable of being driven at 3.3v NOT 5v. The Pico
   GPIO (and therefor I2C) cannot be used at 5v.

   You will need to use a level shifter on the I2C lines if you want to run the
   board at 5v.

   Connections on Raspberry Pi Pico board, other boards may vary.

   GPIO PICO_DEFAULT_I2C_SDA_PIN (On Pico this is GP4 (pin 6)) -> SDA on MPU6050 board
   GPIO PICO_DEFAULT_I2C_SCL_PIN (On Pico this is GP5 (pin 7)) -> SCL on MPU6050 board
   3.3v (pin 36) -> VCC on MPU6050 board
   GND (pin 38)  -> GND on MPU6050 board
*/

// By default these devices  are on bus address 0x68
static int addr = 0x68;

#ifdef i2c_default
void mpu6050_reset() {
    // Two byte reset. First byte register, second byte data
    // There are a load more options to set up the device in different ways that could be added here
    uint8_t buf[] = {0x6B, 0x80};
    i2c_write_blocking(i2c_default, addr, buf, 2, false);
}

void mpu6050_read_raw(int16_t accel[3], int16_t gyro[3], int16_t *temp) {
    // For this particular device, we send the device the register we want to read
    // first, then subsequently read from the device. The register is auto incrementing
    // so we don't need to keep sending the register we want, just the first.

    uint8_t buffer[6];

    // Start reading acceleration registers from register 0x3B for 6 bytes
    uint8_t val = 0x3B;
    i2c_write_blocking(i2c_default, addr, &val, 1, true); // true to keep master control of bus
    i2c_read_blocking(i2c_default, addr, buffer, 6, false);

    for (int i = 0; i < 3; i++) {
        accel[i] = (buffer[i * 2] << 8 | buffer[(i * 2) + 1]);
    }

    // Now gyro data from reg 0x43 for 6 bytes
    // The register is auto incrementing on each read
    val = 0x43;
    i2c_write_blocking(i2c_default, addr, &val, 1, true);
    i2c_read_blocking(i2c_default, addr, buffer, 6, false);  // False - finished with bus

    for (int i = 0; i < 3; i++) {
        gyro[i] = (buffer[i * 2] << 8 | buffer[(i * 2) + 1]);;
    }

    // Now temperature from reg 0x41 for 2 bytes
    // The register is auto incrementing on each read
    val = 0x41;
    i2c_write_blocking(i2c_default, addr, &val, 1, true);
    i2c_read_blocking(i2c_default, addr, buffer, 2, false);  // False - finished with bus

    *temp = buffer[0] << 8 | buffer[1];
}

uint8_t check_mpu6050()
{
    uint8_t val = 0x6B;
    uint8_t my_buf = 0xFF;
    i2c_write_blocking(i2c_default, addr, &val, 1, true);
    i2c_read_blocking(i2c_default, addr, &my_buf, 1, false);
    // printf("power_register:%x\n", my_buf);
    return my_buf;
}

void wakeup_mpu6050()
{
    uint8_t buf[] = {0x6B, 0x00};
    i2c_write_blocking(i2c_default, addr, buf, 2, false);
}

uint8_t mpu6050_who_I_am()
{
    uint8_t val = 0x75;
    uint8_t my_buf = 0x00;
    i2c_write_blocking(i2c_default, addr, &val, 1, true);
    i2c_read_blocking(i2c_default, addr, &my_buf, 1, false);
    return my_buf;
}
void set_mpu6050_acc_config()
{
    uint8_t buf[] = {0x1C, 0x00};
    i2c_write_blocking(i2c_default, addr, buf, 2, false);
}
void set_mpu6050_gyro_config()
{
    uint8_t buf[] = {0x1B, 0x18};
    i2c_write_blocking(i2c_default, addr, buf, 2, false);
}
void read_mpu6050_acc_config()
{
    uint8_t val = 0x1C;
    uint8_t my_buf = 0xFF;
    i2c_write_blocking(i2c_default, addr, &val, 1, true);
    i2c_read_blocking(i2c_default, addr, &my_buf, 1, false);
    printf("acc_config:%x\n", my_buf);
}
void read_mpu6050_gyro_config()
{
    uint8_t val = 0x1B;
    uint8_t my_buf = 0xFF;
    i2c_write_blocking(i2c_default, addr, &val, 1, true);
    i2c_read_blocking(i2c_default, addr, &my_buf, 1, false);
    printf("acc_config:%x\n", my_buf);
}

void calculate_attitude(int16_t accel[3], int16_t gyro[3], float acc[3], float _gyro[3], float pitch, float yaw, float roll)
{
    for(int i = 0; i < 3; ++i)
    {
        acc[i] = accel[i] * 2 * GRAVITY_ACC / 32768.0;
        _gyro[i] = gyro[i] * 2000 / 32768.0;
    }
    pitch = 0;
    yaw = 0;
    roll = 0;
}
/*
slave_addr:从设备地址，这样从设备只有mpu6050，默认为i2c_default
reg_addr:从设备子地址，即要修改的寄存器
len：buf长度
buf：要发送的值
该函数提供给DMP使用对寄存器写入数据
*/
uint8_t mpu6050_write(uint8_t slave_addr, uint8_t reg_addr, uint8_t len, uint8_t* buf)
{
    // uint8_t _buf[2] = {0 ,0};
    // _buf[0] = reg_addr;
    // int res = 0;
    // for(int i = 0; i < len; ++i)
    // {
    //     _buf[1] = buf[i];
    //     res = i2c_write_blocking(i2c_default, slave_addr, _buf, 2, false);
    //     if(res == 0 || res == PICO_ERROR_GENERIC)
    //         return -1;
    // }
    // return 0;
    uint8_t* p = malloc((len + 1) * sizeof(uint8_t));
    int res = 0;
    p[0] = reg_addr;
    for(int i = 1; i < len + 1; ++i)
        p[i] = buf[i - 1];
    res = i2c_write_blocking(i2c_default, slave_addr, p, len + 1, false);
    free(p); p = NULL;
    if(res == 0 || res == PICO_ERROR_GENERIC)
        return -1;
    return 0;
}
/*
slave_addr:从设备地址，这样从设备只有mpu6050，默认为i2c_default
reg_addr:从设备子地址，即要修改的寄存器
len：buf长度
buf：要发送的值
该函数提供给DMP使用从寄存器读出数据
*/
uint8_t mpu6050_read(uint8_t slave_addr, uint8_t reg_addr, uint8_t len, uint8_t* buf)
{
    i2c_write_blocking(i2c_default, slave_addr, &reg_addr, 1, true);
    i2c_read_blocking(i2c_default, slave_addr, buf, len, false);
    return 0;
}
#endif