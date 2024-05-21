#include<main.h>
#include"stdio.h"
void MPU_Setting(){
	uint16_t DevAddress = 0b1101000; /* Device address is b110100X LSB bit is the logic level of AD0 */
	I2C_HandleTypeDef hi2c1;


		  /* Check if device is ready to communicate */
		  int ret;
		  while((ret = HAL_I2C_IsDeviceReady(&hi2c1, (DevAddress << 1), 10, HAL_MAX_DELAY)) != HAL_OK)
				  {
					  switch(ret)
					  {
					  case 1:
						  printf("HAL_ERROR\n\r");
						  break;
					  case 2:
						  printf("HAL_BUSY\n\r");
						  break;
					  case 3:
						  printf("HAL_TIMEOUT\n\r");
					  default:
						  printf("Unknown error\n\r");
						  break;
					  }
				  }

		  /* Set power management register 1 to 0 to enable temperature readings */
		  uint8_t PWR_MGMT_1 = 0x0;
		  if((HAL_I2C_Mem_Write(&hi2c1, (DevAddress << 1), 107, 1, &PWR_MGMT_1, 1, 100) != HAL_OK)) {printf("Error writing\r\n"); exit(1);};
}
void MPU_temp_read(){
		  uint16_t DevAddress = 0b1101000; /* Device address is b110100X LSB bit is the logic level of AD0 */
		  uint16_t tempRegAddress1 = 65;
		  uint16_t tempRegAddress2 = 66;
		  uint8_t TEMP_LOW;
		  uint8_t TEMP_HIGH;
		  I2C_HandleTypeDef hi2c1;

		  /* Read both temperature sensor registers */
		  if((HAL_I2C_Mem_Read(&hi2c1, (DevAddress << 1), tempRegAddress1, 1, &TEMP_HIGH, 1, 100) != HAL_OK)) {printf("Error reading\r\n"); exit(1);};
		  if((HAL_I2C_Mem_Read(&hi2c1, (DevAddress << 1), tempRegAddress2, 1, &TEMP_LOW, 1, 100) != HAL_OK)) {printf("Error reading\r\n"); exit(1);};

		  /* data[1] contains the 8 most significant bits and data[0] the 8 least significant bits*/
		  /* The value retrieved is a signed integer */
		  int16_t tempValue = (TEMP_HIGH << 8) | TEMP_LOW;
		  tempValue = ((tempValue /= 340.00) + 36.53)*100;
		  printf("Temperature value = %d.%d °C\r\n", (tempValue / 100), (tempValue % 100));
		  printf("-------------- \r\n");
}
