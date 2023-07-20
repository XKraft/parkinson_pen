#include <stdio.h>
#include <string.h>
#include "mpu6050_i2c.h"
#include "inv_mpu.h"

int main() {
    stdio_init_all();
    
    printf("Hello, MPU6050! Get Data By DMP...\n");

    // // This example will use I2C0 on the default SDA and SCL pins (4, 5 on a Pico)
    // i2c_init(i2c_default, 400 * 1000);
    // gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    // gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
    // gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
    // gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);
    // // Make the I2C pins available to picotool
    // bi_decl(bi_2pins_with_func(PICO_DEFAULT_I2C_SDA_PIN, PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C));

    // sleep_ms(1000);
    // mpu6050_reset();

    while(MPU6050_DMP_Init());

    // int16_t acceleration[3], gyro[3], temp;
    // float _gyro[3], acc[3], pitch, yaw, roll;
    float pitch, yaw, roll;

    while (1) {
        // if(check_mpu6050() != 0x00)
        // {
        //     printf("error: mpu6050 is sleeping\n");
        //     wakeup_mpu6050();
        //     set_mpu6050_acc_config();
        //     set_mpu6050_gyro_config();
        //     read_mpu6050_acc_config();
        //     read_mpu6050_gyro_config();
        // }

        // mpu6050_read_raw(acceleration, gyro, &temp);

        // // These are the raw numbers from the chip, so will need tweaking to be really useful.
        // // See the datasheet for more information
        // // printf("Acc. X = %d, Y = %d, Z = %d\n", acceleration[0], acceleration[1], acceleration[2]);
        // // printf("Gyro. X = %d, Y = %d, Z = %d\n", gyro[0], gyro[1], gyro[2]);
        // // // Temperature is simple so use the datasheet calculation to get deg C.
        // // // Note this is chip temperature.
        // // printf("Temp. = %f\n", (temp / 340.0) + 36.53);

        // calculate_attitude(acceleration, gyro, acc, _gyro, pitch, yaw, roll);
        // printf("acc.x = %f, acc.y = %f, acc.z = %f\n", acc[0], acc[1], acc[2]);
        // printf("gyro.x = %f, gyro.y = %f, gyro.z = %f\n", _gyro[0], _gyro[1], _gyro[2]);
        
        MPU6050_DMP_Get_Data(&pitch, &roll, &yaw);
        printf("pitch:%f, roll:%f, yaw:%f\n", pitch, roll, yaw);

        sleep_ms(10);
    }
}