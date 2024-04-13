/*
 * I2C.h
 *
 *  Created on: Mar 12, 2024
 *      Author: Castanier Romain
 */

#ifndef I2C_H_
#define I2C_H_

/*--------------------------------------------- Includes -----------------------------------------------------------------*/
#include "MAX7219.h"

/*--------------------------------------------- Define -------------------------------------------------------------------*/
#define ADDRESS 0x38<<1 // Voir datasheet AHT20
#define TRIG_MEASURE 0xAC
#define RES 0xBA // reset
#define INITIALISATION_CMD 0xBE


/*--------------------------------------------- Fonctions ----------------------------------------------------------------*/
void UART_Message(char * buffer, UART_HandleTypeDef* huart2);
void AHT_Init(I2C_HandleTypeDef* hi2c1,UART_HandleTypeDef* huart2);
void AHT_TrigMeasure(I2C_HandleTypeDef* hi2c1,UART_HandleTypeDef* huart2);
void AHT_Readdata(I2C_HandleTypeDef* hi2c1,UART_HandleTypeDef* huart2,Matrix *device);
void AHT_Reset(I2C_HandleTypeDef* hi2c1,UART_HandleTypeDef* huart2);
void AHT_Check(I2C_HandleTypeDef* hi2c1,UART_HandleTypeDef* huart2);
void AHT_IsBusy(I2C_HandleTypeDef* hi2c1,UART_HandleTypeDef* huart2);













#endif /* I2C_H_ */
