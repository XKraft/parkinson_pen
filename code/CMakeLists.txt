cmake_minimum_required(VERSION 3.12)

# Pull in SDK (must be before project)
include(pico_sdk_import.cmake)

project(parkinson_pen_project C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialize the SDK
pico_sdk_init()

set (SOURCES main.c src/mpu6050_i2c.c src/inv_mpu_dmp_motion_driver.c src/inv_mpu.c src/MyUart.c src/atk_ms901m.c src/kalman.c src/pid.c)

add_executable(${PROJECT_NAME}
        ${SOURCES}
        )

target_include_directories(${PROJECT_NAME}
        PRIVATE
        ${PROJECT_SOURCE_DIR}/inc)

# pull in common dependencies
target_link_libraries(${PROJECT_NAME} pico_stdlib hardware_i2c)

# create map/bin/hex file etc.
pico_add_extra_outputs(${PROJECT_NAME})