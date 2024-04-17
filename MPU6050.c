#include<MPU6050.h>
#include<main.h>
extern I2C_HandleTypeDef hi2c1;
void mpu6050_Setting(){
	/*all the if function below is just to debug, technically only the function is needed*/
	/*this part is to check if the sensor is connected correctly*/
	  /*this function is in pooling mode*/
	  /*the required valuable that is required to be filled in the function is available on the data sheet document(https://invensense.tdk.com/wp-content/uploads/2015/02/MPU-6000-Register-Map1.pdf)*/
	 /*I2C_HandleTypeDef= the type of hi2c that you are using in this specific case is hi2c1,uint16_t DevAddress=is the devaddress specified in the data book depends on the condition in this case its 0, uint32_t Trials=how many trial, uint32_t Timeout= well timeout :))*/
	  HAL_StatusTypeDef ret= HAL_I2C_IsDeviceReady(&hi2c1, (DEVICE_ADRESS <<1) + 0 , 1, 100);/*(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint32_t Trials, uint32_t Timeout)*/
	  if (ret==HAL_OK){
		  printf("thedeviceisready\n");
	  }
	  else{
		  printf("F*CK\n");
	  }
	  /*this part is to configure the sensor data register of the sensor, in specific the full scale range of the gyro, the same function can technically do all kind of config as stated in the registor map, all detail related is in the register map document (https://invensense.tdk.com/wp-content/uploads/2015/02/MPU-6000-Register-Map1.pdf)*/
	  /*the first two part is the same, uint16_t MemAddress= the register, its in the register map of configuration,  uint16_t MemAddSize=size of bit that you want to add, uint8_t *pData=buffer, the last two is the same*/

	  uint8_t temp_data=FS_GYRO_500;
	  ret=HAL_I2C_Mem_Write(&hi2c1,(DEVICE_ADRESS <<1) + 0, REG_CONFIG_GYRO,1, &temp_data,1, 100);/*(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout);*/
	  if (ret==HAL_OK){
	  	  printf("writting register to 27(configuring gyro)\n");
	    }
	    else{
	  	  printf("F*CK\n");
	    }
	  /*this part is to configure the sensor data register of the sensor, in specific the full scale range of the acc, the same function can technically do all kind of config as stated in the registor map, all detail related is in the register map document (https://invensense.tdk.com/wp-content/uploads/2015/02/MPU-6000-Register-Map1.pdf)*/
	  uint8_t temp_data=FS_ACC_4G;
	  ret=HAL_I2C_Mem_Write(&hi2c1,(DEVICE_ADRESS <<1) + 0, REG_CONFIG_ACC,1, &temp_data,1, 100);/*(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout);*/
	  if (ret==HAL_OK){
		  printf("writting register to 28(configuring acc)\n");
		}
		else{
		  printf("F*CK\n");
		}
	  /*im too lazy to copy paste but its the same as above and its to escape sleep mode lol*/
	  uint8_t temp_data=0;
	  ret=HAL_I2C_Mem_Write(&hi2c1,(DEVICE_ADRESS <<1) + 0,REG_USER_CTRL,1, &temp_data,1, 100);/*(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout);*/
	  if (ret==HAL_OK){
		  printf("writting register to 107(escape sleep mode)\n");
		}
		else{
		  printf("F*CK\n");
		}
}
void mpu6050_read(){
	/*im too lazy to copy paste but its the same as above and its to read data lol*/
    uint8_t ACC_DATA_XCORD_HIGH;
	uint8_t ACC_DATA_XCORD_LOW;
	if((HAL_I2C_Mem_Write(&hi2c, (DEVICE_ADRESS <<1) + 0,REGACC_DATA_XCORD_HIGH, 1,&ACC_DATA_XCORD_HIGH,1,100)!= HAL_OK)) {printf("Error reading\r\n"); exit(1);};
	if((HAL_I2C_Mem_Write(&hi2c, (DEVICE_ADRESS <<1) + 0,REGACC_DATA_XCORD_LOW, 1,&ACC_DATA_XCORD_LOW,1,100)!= HAL_OK)) {printf("Error reading\r\n"); exit(1);};
	int16_t xacc_value = (ACC_DATA_XCORD_HIGH << 8) |ACC_DATA_XCORD_LOW;
	printf("XACC cord value = %d\r\n", xacc_value);
	uint8_t ACC_DATA_YCORD_HIGH;
	uint8_t ACC_DATA_YCORD_LOW;
	if((HAL_I2C_Mem_Write(&hi2c, (DEVICE_ADRESS <<1) + 0,REGACC_DATA_YCORD_HIGH, 1,&ACC_DATA_YCORD_HIGH,1,100)!= HAL_OK)) {printf("Error reading\r\n"); exit(1);};
	if((HAL_I2C_Mem_Write(&hi2c, (DEVICE_ADRESS <<1) + 0,REGACC_DATA_YCORD_LOW, 1,&ACC_DATA_YCORD_LOW,1,100)!= HAL_OK)) {printf("Error reading\r\n"); exit(1);};
	int16_t yacc_value = (ACC_DATA_YCORD_HIGH << 8) | ACC_DATA_YCORD_LOW;
	printf("YACC cord value = %d\r\n", yacc_value);
	uint8_t ACC_DATA_ZCORD_HIGH;
	uint8_t ACC_DATA_ZCORD_LOW;
	if((HAL_I2C_Mem_Write(&hi2c, (DEVICE_ADRESS <<1) + 0,REGACC_DATA_ZCORD_HIGH, 1,&ACC_DATA_ZCORD_HIGH,1,100)!= HAL_OK)) {printf("Error reading\r\n"); exit(1);};
	if((HAL_I2C_Mem_Write(&hi2c, (DEVICE_ADRESS <<1) + 0,REGACC_DATA_ZCORD_LOW, 1,&ACC_DATA_ZCORD_LOW,1,100)!= HAL_OK)) {printf("Error reading\r\n"); exit(1);};
	int16_t zacc_value = (ACC_DATA_ZCORD_HIGH << 8) | ACC_DATA_ZCORD_LOW;
	printf("ZACC cord value = %d\r\n", zacc_value);
    uint8_t GYRO_DATA_XCORD_HIGH;
	uint8_t GYRO_DATA_XCORD_LOW;
	if((HAL_I2C_Mem_Write(&hi2c, (DEVICE_ADRESS <<1) + 0,REGGYRO_DATA_XCORD_HIGH, 1,&GYRO_DATA_XCORD_HIGH,1,100)!= HAL_OK)) {printf("Error reading\r\n"); exit(1);};
	if((HAL_I2C_Mem_Write(&hi2c, (DEVICE_ADRESS <<1) + 0,REGGYRO_DATA_XCORD_LOW, 1,&GYRO_DATA_XCORD_LOW,1,100)!= HAL_OK)) {printf("Error reading\r\n"); exit(1);};
	int16_t xgyro_value = (GYRO_DATA_XCORD_HIGH<< 8) |  GYRO_DATA_XCORD_LOW;
	printf("XGYRO cord value = %d\r\n", xgyro_value);
	uint8_t GYRO_DATA_YCORD_HIGH;
	uint8_t GYRO_DATA_YCORD_LOW;
	if((HAL_I2C_Mem_Write(&hi2c, (DEVICE_ADRESS <<1) + 0,REGGYRO_DATA_YCORD_HIGH, 1,&GYRO_DATA_YCORD_HIGH,1,100)!= HAL_OK)) {printf("Error reading\r\n"); exit(1);};
	if((HAL_I2C_Mem_Write(&hi2c, (DEVICE_ADRESS <<1) + 0,REGGYRO_DATA_YCORD_LOW, 1,&GYRO_DATA_YCORD_LOW,1,100)!= HAL_OK)) {printf("Error reading\r\n"); exit(1);};
	int16_t ygyro_value = (GYRO_DATA_YCORD_HIGH << 8) | GYRO_DATA_YCORD_LOW;
	printf("YGYRO cord value = %d\r\n", ygyro_value);
	uint8_t GYRO_DATA_ZCORD_HIGH;
	uint8_t GYRO_DATA_ZCORD_LOW;
	if((HAL_I2C_Mem_Write(&hi2c, (DEVICE_ADRESS <<1) + 0,REGGYRO_DATA_ZCORD_HIGH, 1,&GYRO_DATA_ZCORD_HIGH,1,100)!= HAL_OK)) {printf("Error reading\r\n"); exit(1);};
	if((HAL_I2C_Mem_Write(&hi2c, (DEVICE_ADRESS <<1) + 0,REGGYRO_DATA_ZCORD_LOW, 1,data,1,100)!= HAL_OK)) {printf("Error reading\r\n"); exit(1);};
	int16_t zgyro_value = ( GYRO_DATA_ZCORD_HIGH << 8) | &GYRO_DATA_ZCORD_LOW;
	printf("ZGYRO cord value = %d\r\n", zgyro_value);
}
