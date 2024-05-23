#include "main.h"
#include "stdio.h"
#include "temperatureread.h"

// Assume hi2c1 is defined and initialized elsewhere
extern I2C_HandleTypeDef hi2c1;

void MPU_Setting() {
    uint16_t DevAddress = 0b1101000; /* Device address is b110100X LSB bit is the logic level of AD0 */
    int ret;

    /* Check if device is ready to communicate */
    while((ret = HAL_I2C_IsDeviceReady(&hi2c1, (DevAddress << 1), 10, HAL_MAX_DELAY)) != HAL_OK) {
        switch(ret) {
            case HAL_ERROR:
                printf("HAL_ERROR\n\r");
                break;
            case HAL_BUSY:
                printf("HAL_BUSY\n\r");
                break;
            case HAL_TIMEOUT:
                printf("HAL_TIMEOUT\n\r");
                break;
            default:
                printf("Unknown error\n\r");
                break;
        }
    }

    /* Set power management register 1 to 0 to enable temperature readings */
    uint8_t PWR_MGMT_1 = 0x0;
    if(HAL_I2C_Mem_Write(&hi2c1, (DevAddress << 1), 107, I2C_MEMADD_SIZE_8BIT, &PWR_MGMT_1, 1, 100) != HAL_OK) {
        printf("Error writing PWR_MGMT_1\r\n");
    }
}

int16_t MPU_temp_read() {
    uint16_t DevAddress = 0b1101000; /* Device address is b110100X LSB bit is the logic level of AD0 */
    uint16_t tempRegAddress1 = 65;
    uint16_t tempRegAddress2 = 66;
    uint8_t TEMP_LOW;
    uint8_t TEMP_HIGH;

    /* Read the high byte of the temperature sensor register */
    if(HAL_I2C_Mem_Read(&hi2c1, (DevAddress << 1), tempRegAddress1, I2C_MEMADD_SIZE_8BIT, &TEMP_HIGH, 1, 100) != HAL_OK) {
        printf("Error reading TEMP_HIGH\r\n");
        return -1; // Indicate an error
    }

    /* Read the low byte of the temperature sensor register */
    if(HAL_I2C_Mem_Read(&hi2c1, (DevAddress << 1), tempRegAddress2, I2C_MEMADD_SIZE_8BIT, &TEMP_LOW, 1, 100) != HAL_OK) {
        printf("Error reading TEMP_LOW\r\n");
        return -1; // Indicate an error
    }

    /* Combine the high and low bytes */
    int16_t tempValue = (TEMP_HIGH << 8) | TEMP_LOW;
    /* Calculate the temperature in degrees Celsius */
    float tempCelsius = ((tempValue / 340.0) + 36.53);
    int16_t tempCelsiusInt = (int16_t)(tempCelsius * 100); // Convert to int representation for fixed-point

    /* Print the temperature value */
    printf("Temperature value = %d.%02d °C\r\n", (tempCelsiusInt / 100), (tempCelsiusInt % 100));
    printf("-------------- \r\n");

    return tempCelsiusInt;
}
