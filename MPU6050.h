/*
 * MPU6050.c
 *
 *  Created on: Apr 17, 2024
 *      Author: ASUS
 */

#ifndef INC_MPU6050_C_
#define INC_MPU6050_C_
void mpu6050_read();
void mpu6050_Setting();
#define DEVICE_ADRESS 0x68

#define FS_GYRO_250 0
#define FS_GYRO_500 8
#define FS_GYRO_1000 9
#define FS_GYRO_2000 10

#define FS_ACC_2G 0
#define FS_ACC_4G 8
#define FS_ACC_8G 9
#define FS_ACC_16G 10

#define REG_CONFIG_GYRO 27
#define REG_CONFIG_ACC	28
#define REG_USER_CTRL	107

#define REGACC_DATA_XCORD_HIGH	59
#define REGACC_DATA_XCORD_LOW	60
#define REGACC_DATA_YCORD_HIGH	61
#define REGACC_DATA_YCORD_LOW	62
#define REGACC_DATA_ZCORD_HIGH	63
#define REGACC_DATA_ZCORD_LOW	64

#define REGGYRO_DATA_XCORD_HIGH	67
#define REGGYRO_DATA_XCORD_LOW	68
#define REGGYRO_DATA_YCORD_HIGH	69
#define REGGYRO_DATA_YCORD_LOW	70
#define REGGYRO_DATA_ZCORD_HIGH	71
#define REGGYRO_DATA_ZCORD_LOW	72
#endif /* INC_MPU6050_C_ */
