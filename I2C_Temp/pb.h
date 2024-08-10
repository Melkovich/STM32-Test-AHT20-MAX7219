/*
 * pb.h
 *
 *  Created on: Mar 23, 2024
 *      Author: 33789
 */

#ifndef PB_H_
#define PB_H_

#include "MAX7219.h"
#include "I2C.h"

void Task_Display(AHT20* aht, Matrix *device);
void Task_Display_Temperature(AHT20* aht, Matrix *device);
void Task_Display_Humidity(AHT20* aht, Matrix *device);
void Task_Display_3(AHT20* aht, Matrix *device);

#endif /* PB_H_ */
