/*
 * I2C.h
 *
 *  Created on: Mar 12, 2024
 *      Author: Castanier Romain
 */

#ifndef I2C_H_
#define I2C_H_

typedef struct {

	I2C_HandleTypeDef *hi2c;
	UART_HandleTypeDef* huart;
	uint8_t rawData[6];
	float humidite;
	float temperature;

} AHT20;

/*--------------------------------------------- Includes -----------------------------------------------------------------*/
#include "MAX7219.h"

/*--------------------------------------------- Define -------------------------------------------------------------------*/
#define ADDRESS 0x38<<1 // Voir datasheet AHT20
#define TRIG_MEASURE 0xAC
#define RES 0xBA // reset
#define INITIALISATION_CMD 0xBE


/*--------------------------------------------- Fonctions ----------------------------------------------------------------*/
void UART_Message(char * buffer, UART_HandleTypeDef* huart);
void AHT_Init(I2C_HandleTypeDef* hi2c,UART_HandleTypeDef* huart,AHT20* device);
void AHT_TrigMeasure(AHT20* device);
void AHT_Readdata(AHT20* device);
void AHT_Reset(AHT20* device);
void AHT_Check(AHT20* device);
void AHT_IsBusy(AHT20* device);

void MATRIX_Display_Temperature(AHT20* device, Matrix* Matrice,int choix);
void MATRIX_Display_Humidity(AHT20* device, Matrix* Matrice,int choix);










#endif /* I2C_H_ */
