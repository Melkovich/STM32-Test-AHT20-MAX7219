/*
 * pb.h
 *
 *  Created on: Mar 23, 2024
 *      Author: 33789
 */

#ifndef PB_H_
#define PB_H_

#include "MAX7219.h"

void pb_launcher(UART_HandleTypeDef *huart,I2C_HandleTypeDef *hi2c, Matrix *device);


#endif /* PB_H_ */
