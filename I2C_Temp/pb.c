/*
 * pb.c
 *
 *  Created on: Mar 23, 2024
 *      Author: 33789
 */

#include "main.h"
#include "stm32f4xx_hal_gpio.h"
#include "I2C.h"
#include <stdbool.h>

bool flag_pb = false;


void pb_launcher(UART_HandleTypeDef *huart,I2C_HandleTypeDef *hi2c, Matrix *device)
{
	HAL_Delay(10);
	uint32_t pbvalue = HAL_GPIO_ReadPin(BP0_GPIO_Port,BP0_Pin);

	switch (flag_pb) {
		case false:
				if(!pbvalue)
				{
				    AHT_TrigMeasure(hi2c, huart);
				    AHT_IsBusy(hi2c, huart);
				    AHT_Readdata(hi2c, huart, device);
					flag_pb = true;
				}

			break;
		case true :
				if(pbvalue)
				{
					flag_pb = false;
				}

			break;
		default:
			flag_pb = false;
			break;
	}


}
