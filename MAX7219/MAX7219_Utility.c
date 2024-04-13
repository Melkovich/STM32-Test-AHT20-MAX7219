/*
 * MAX7219_Utility.c
 *
 *  Created on: Apr 4, 2024
 *      Author: 33789
 */

#include "main.h"
#include "stm32f4xx_hal_gpio.h"
#include "stm32f4xx_hal_spi.h"
#include "MAX7219.h"

extern Matrix *device;					// Déclarer comme variable externe à fichier .c
extern SPI_HandleTypeDef *hspi;

//COMMANDE MATRIX
uint8_t NO_DECODE_MODE[2] = {0x09,0x00};
uint8_t INTENSITY_MIN[2] = {0x0A,0x00};
uint8_t SCAN_LIMIT_MAX[2] = {0x0B,0x07};
uint8_t SHUT_DOWN_OFF[2] = {0x0C,0x01};
uint8_t DISPLAY_TEST_OFF[2] = {0x0F,0x00};
uint8_t DISPLAY_TEST_ON[2] = {0x0F,0x01};
uint8_t NO_OP[2] = {0x00,0x00};
uint8_t SHUT_DOWN_ON[2] = {0x0C,0x00};
uint8_t TEST_MODE_ON[2] = {0x0F,0x01};
uint8_t TEST_MODE_OFF[2] = {0x0F,0x00};


//COMMANDE INITIALISATION
uint8_t Initialisation[5][2] = {
		{0x09,0x00},
		{0x0A,0x00},
		{0x0B,0x07},
		{0x0C,0x01},
		{0x0F,0x00},
};

//Fonction d'initialisation matrice à LED
void MAX7219_init(SPI_HandleTypeDef *hspi,Matrix *device)
{

	HAL_StatusTypeDef ret = 0;
	device->hspi = hspi;

	uint8_t Commande[2];

	for(int j = 0;j<5;j++)
	{
		Commande[0] = Initialisation[j][0];
		Commande[1] = Initialisation [j][1];

		for(int i =0;i<3;i++)
		{
			HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_RESET);
			ret = HAL_SPI_Transmit(device->hspi, Commande,2, 100);
			HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_SET);
		}
	}

	MAX7219_clear_all(device);
}

//Fonction pour éteindre la matrice = mode économie d'énergie, attention garde les digit en mémoire
void MAX7219_off(Matrix *device)
{
	HAL_StatusTypeDef ret = 0;

	for(int i = 0;i<3;i++)
	{
	HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_RESET);
	ret = HAL_SPI_Transmit(device->hspi, SHUT_DOWN_OFF,2, 100);
	HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_SET);

	}

}

//Fonction pour rallumer matrice
void MAX7219_on(Matrix *device)
{
	HAL_StatusTypeDef ret = 0;

	for(int i = 0;i<3;i++)
	{
	HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_RESET);
	ret = HAL_SPI_Transmit(device->hspi, SHUT_DOWN_ON,2, 100);
	HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_SET);

	}
}


//Fonction qui clear les 3 matrices
void MAX7219_clear_all(Matrix *device)
{

	uint8_t Commande[2] = {0x00,0x00};
	HAL_StatusTypeDef ret = 0;

	for(int j=1;j<=8;j++)
	{
		Commande[0] = j;
		for(int i=0;i<3;i++)
		{
			HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_RESET);
			ret = HAL_SPI_Transmit(device->hspi,Commande,2, HAL_MAX_DELAY);
			HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_SET);

		}
	}
}

//Fonction pour donner decode mode aux 3 matrices
void MAX7219_decode_mode_off(Matrix * device)
{
	HAL_StatusTypeDef ret = 0;
	for(int i = 0;i<3;i++)
	{
	HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_RESET);
	ret = HAL_SPI_Transmit(device->hspi, NO_DECODE_MODE,2, 100);
	HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_SET);

	}
}

//Fonction pour ne pas réaliser d'opération
void MAX7219_no_op(Matrix *device)
{
	HAL_StatusTypeDef ret = 0;

	HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_RESET);
	ret = HAL_SPI_Transmit(device->hspi,NO_OP,2, 100);
	HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_SET);
}

//Fonction pour tester le fonctionnement des matrices
 void MAX7219_Test_Mode(SPI_HandleTypeDef *hspi)
{
	 HAL_StatusTypeDef ret = 0;

	 for(int i = 0;i<3;i++)
	 {
		 HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_RESET);
		 ret = HAL_SPI_Transmit(hspi,TEST_MODE_ON,2, HAL_MAX_DELAY);
		 HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_SET);
		 HAL_Delay(1000);
	 }

	 for(int i = 0;i<3;i++)
	 {
		 HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_RESET);
		 ret = HAL_SPI_Transmit(hspi,TEST_MODE_OFF,2, HAL_MAX_DELAY);
		 HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_SET);
		 HAL_Delay(1000);
	 }
}

 void MAX7219_Set_Intensity(Matrix * device)
 {
	 HAL_StatusTypeDef ret = 0;

	 for(int i = 0;i<3;i++)
	 {
		 HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_RESET);
		 ret = HAL_SPI_Transmit(device->hspi,INTENSITY_MIN,2, HAL_MAX_DELAY);
		 HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_SET);
	 }

 }

 void MAX7219_Scan_Limit(Matrix * device)
 {
	 HAL_StatusTypeDef ret = 0;

	 for(int i = 0;i<3;i++)
	 {
		 HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_RESET);
		 ret = HAL_SPI_Transmit(device->hspi,SCAN_LIMIT_MAX,2, HAL_MAX_DELAY);
		 HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_SET);
	 }

 }

 void MAX7219_ShutDown_off(Matrix * device)
 {
	 HAL_StatusTypeDef ret = 0;
	 for(int i = 0;i<3;i++)
	 {
		 HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_RESET);
		 ret = HAL_SPI_Transmit(device->hspi,SHUT_DOWN_OFF,2, HAL_MAX_DELAY);
		 HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_SET);
	 }
 }

 void MAX7219_Display_Test_off(Matrix * device)
 {
	 HAL_StatusTypeDef ret = 0;
	 for(int i = 0;i<3;i++)
	 {
		 HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_RESET);
		 ret = HAL_SPI_Transmit(device->hspi,DISPLAY_TEST_OFF,2, HAL_MAX_DELAY);
		 HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_SET);
	 }
 }

 void MAX7219_SendData(Matrix *device,uint8_t adresse,uint8_t data)
 {
	 HAL_StatusTypeDef ret = 0;
	 uint8_t Commande[2];

		Commande[0] = adresse;
		Commande[1]= data;

		HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_RESET);
		ret = HAL_SPI_Transmit(device->hspi,Commande,2, HAL_MAX_DELAY);
		HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_SET);
 }

 //Fonction qui clear 1 ligne
 void MAX7219_clear(Matrix *device)
 {

 	uint8_t Commande[2] = {0x00,0x00};
 	HAL_StatusTypeDef ret = 0;

 	HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_RESET);
 	ret = HAL_SPI_Transmit(device->hspi,Commande,2, HAL_MAX_DELAY);
 	HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_SET);

 }

 //Fonction pour envoyer directement des paquets pour toutes les matrices en cascade
 void MAX7219_SendPackage(Matrix *device,uint8_t *commande,uint8_t size)
 {
	 HAL_StatusTypeDef ret = 0;

	 HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_RESET);
	 ret = HAL_SPI_Transmit(device->hspi, commande, size, HAL_MAX_DELAY);
	 HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_SET);

 }
