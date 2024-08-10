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

#define TEMPO 3000;
bool flag_pb = false;

//Fonction qui affiche température par appui sur bouton poussoir
void Task_Display(AHT20* aht, Matrix *device)
{
	HAL_Delay(10);
	uint32_t pbvalue = HAL_GPIO_ReadPin(BP0_GPIO_Port,BP0_Pin);

	switch (flag_pb) {
		case false:
				if(!pbvalue)
				{
				    AHT_TrigMeasure(aht);
				    AHT_IsBusy(aht);
				    AHT_Readdata(aht);
				    MATRIX_Display_Temperature(aht, device,1);
				   // MATRIX_Display_Humidity(aht, device);
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

	Display_Show_Matrice(device, DEFAULT);

}

//Fonction qui affiche température toute les 5 secondes en tâche non blocante
void Task_Display_Temperature(AHT20* aht, Matrix *device)
{

	uint32_t localTick = HAL_GetTick();
	static uint32_t tempo = 0;

	if(tempo>1431655)// nb max de remplissage de 3000 sur 32 bits
		tempo = 0;

	if(localTick < tempo)
	{
		return;
	}

	else
	{
		AHT_TrigMeasure(aht);
		AHT_IsBusy(aht);
		AHT_Readdata(aht);
		MATRIX_Display_Temperature(aht, device,1);
		Display_Show_Matrice(device, DEFAULT);
		tempo+=TEMPO;
	}
}

void Task_Display_Humidity(AHT20* aht, Matrix *device)
{

	uint32_t localTick = HAL_GetTick();
	static uint32_t tempo = 0;

	if(tempo>1431655)// nb max de remplissage de 3000 sur 32 bits
		tempo = 0;

	if(localTick < tempo)
	{
		return;
	}

	else
	{
		AHT_TrigMeasure(aht);
		AHT_IsBusy(aht);
		AHT_Readdata(aht);
		MATRIX_Display_Humidity(aht, device,1);
		Display_Show_Matrice(device, DEFAULT);
		tempo+=TEMPO;
	}

}

//Fonction qui affiche température ou humidité selon appui sur bouton poussoir
void Task_Display_3(AHT20* aht, Matrix *device)
{

	HAL_Delay(20);
	uint32_t pbvalue = HAL_GPIO_ReadPin(BP0_GPIO_Port,BP0_Pin);
	static uint8_t choix = 0;

	AHT_TrigMeasure(aht);		// Déclencher mesure et avoir nouvelle data
	AHT_IsBusy(aht);
	AHT_Readdata(aht);

	switch (flag_pb) {						// flag_bp = mémoire position du bouton, false = position haut, true = position bas
		case false:
				if(!pbvalue)				// pbvalue = 1 appui, pbvalue = 0 pas appui
				{
					switch (choix) {
						case 0:

							MATRIX_Display_Temperature(aht, device,1);			// mettre ordre d'affichage temp = choix 0 et hum = choix 1
							MATRIX_Display_Humidity(aht, device,2);
							Display_Change_Data_Matrice_Up(device, DEFAULT);	// Animation data up

							flag_pb = true;
							choix = 1;
							break;
						case 1:

							MATRIX_Display_Temperature(aht, device,2);
							MATRIX_Display_Humidity(aht, device,1);
							Display_Change_Data_Matrice_Up(device, DEFAULT);

							flag_pb = true;
							choix = 0;
							break;
						default:
							break;
					}
				}

			break;
		case true :
				if(pbvalue)					// Programme boucle tant que pas d'appui bp
				{
					flag_pb = false;
				}
				if(choix == 0)
				{
					Task_Display_Temperature(aht, device);
				}
				if(choix == 1)
				{
					Task_Display_Humidity(aht, device);
				}
			break;
		default:
			flag_pb = false;
			break;
	}

}
