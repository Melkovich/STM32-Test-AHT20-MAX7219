/*
 * I2C.c
 *
 *  Created on: Mar 12, 2024
 *      Author: Castanier Romain
 */

#include "main.h"
#include <string.h>
#include"I2C.h"
#include "math.h"

#define TEST_UART 0							//Pour afficher sur TeraTerm valeur capteur
#define MATRIX_DISPLAY 1					//Pour afficher sur Matrice

#if MATRIX_DISPLAY
//Fonction pour afficher température
void MATRIX_Display_Temperature(AHT20* device, Matrix* Matrice,int choix)
{
	char Buf_2[6];
	char tmpchar;
	/*Bloc pour afficher température sur Matrice à LED*/
	int tmp = device->temperature*powf(10,4);
	sprintf(Buf_2,"%d",tmp);
	tmpchar = Buf_2[2];
	Buf_2[2] = '.';
	Buf_2[3] = tmpchar ;
	Buf_2[4]= 'd';
	Buf_2[5] = '\0';
	Display_Print_Matrice(Matrice, Buf_2, choix);
}
//Fonction pour afficher humidité
void MATRIX_Display_Humidity(AHT20* device, Matrix* Matrice,int choix)
{
	char Buf_2[6];
	char tmpchar;
	/*Bloc pour afficher température sur Matrice à LED*/
	int tmp = device->humidite*powf(10,4);
	sprintf(Buf_2,"%d",tmp);
	tmpchar = Buf_2[2];
	Buf_2[2] = '.';
	Buf_2[3] = tmpchar ;
	Buf_2[4]= '%';
	Buf_2[5] = '\0';
	Display_Print_Matrice(Matrice, Buf_2, choix);
}

#endif

#if TEST_UART
//Fonction pour envoyer du texte à TeraTerm
void UART_Message(char * buffer, UART_HandleTypeDef* huart)
{
	uint32_t len = strlen(buffer);
	HAL_UART_Transmit(huart,(uint8_t*)buffer, len, 100);
}

//Fonction pour décomposer un float en pls décimal pour afficher sur TeraTerm
void UART_FloatToDisplay(float nb,uint8_t precision,UART_HandleTypeDef* huart2)
{
	char Buf[6];

	int tmp = fmod(nb,powf(10,2));//Extraire dizaine et unité
	float puissance = precision;
	sprintf(Buf,"%d",tmp);								//Copier le int en un char pour afficher nb avant virgule
	HAL_UART_Transmit(huart2,Buf, 2, 100);

	/*Bloc pour afficher chiffre après virgule selon précision souhaité*/
	UART_Message(",", huart2);
	tmp = nb * pow(10,precision) ;

	for(uint8_t i = precision ; i>0; i--)
	{
		tmp = fmodf(tmp,powf(10,puissance));
		tmp = tmp / pow(10,i-1);
		sprintf(Buf,"%d",tmp);
		HAL_UART_Transmit(huart2,Buf, 1, 100);
		tmp = nb * pow(10,precision);
		puissance = i-1;

	}
//	Display_Print_Matrice(device, Buf_2, DEFAULT);
	UART_Message("\r\n", huart2);
}
#endif
//Fonction pour init aht20
void AHT_Init(I2C_HandleTypeDef *hi2c, UART_HandleTypeDef* huart,AHT20* device)
{
	HAL_Delay(40);	//Delai avant utilisation du capteur

	device->hi2c = hi2c;
	device->huart = huart;

	uint8_t status=0;
	uint8_t Commande[2]= {0x08,0x00};
	HAL_StatusTypeDef ret=0;

	/*Commande d'initialisation*/
	ret = HAL_I2C_Mem_Write(device->hi2c, ADDRESS, INITIALISATION_CMD, 1, Commande,sizeof(Commande), 100);
#if TEST_UART
	if(ret!=HAL_OK)
		UART_Message("Error Init cmd not send \n\r", device->huart);
#endif
	/*Vérifier que aht20 est bien prêt pour convertir soit 0x18 = 0001.1000 avec Bit[7] = 0 -> Status signifie est Free et Bit[3] =  1 -> siginfie Calibré */
	ret = HAL_I2C_Master_Receive(device->hi2c, ADDRESS|0X01, &status, 1, 100);
#if TEST_UART
	if(ret!=HAL_OK)
		UART_Message("Error Master do not receive \n\r", device->huart);

	if((status&0x18)==0x18)
	{
		UART_Message("Init OK \n\r", device->huart);
	}
	else
	{
		UART_Message("Error init fail \n\r", device->huart);
	}
#endif
}

//Fonction pour lancer une conversion
void AHT_TrigMeasure(AHT20* device)
{

	uint8_t Commande[2] = {0x33,0x00};
	HAL_StatusTypeDef ret = HAL_I2C_Mem_Write(device->hi2c, ADDRESS, TRIG_MEASURE, I2C_MEMADD_SIZE_8BIT, Commande, 2, HAL_MAX_DELAY);				// Ecrit dans esclave à adresse de trigger measure avec les lignes de codes associées (voir datasheet)

#if TEST_UART
	if(ret==HAL_OK)
	{
		UART_Message("Trigger OK \n\r", device->huart);
	}
	else
	{
		UART_Message("Trigger KO \n\r", device->huart);
	}
#endif
}

//Fonction pour lire donnée, convertir et afficher
void AHT_Readdata(AHT20* device)
{
	int tmp = 0;

	HAL_StatusTypeDef ret = HAL_I2C_Master_Receive(device->hi2c, ADDRESS|0x01, device->rawData, sizeof(device->rawData), 100);
#if TEST_UART
	if(ret!=HAL_OK)
		UART_Message("Read KO \n\r", device->huart);
#endif
	uint32_t hum =  ((uint32_t)device->rawData[1]<<16)									//	Prend les octets de donnée de humidité on les mets dans un gros uint32 pour les ranger
							   |((uint32_t)device->rawData[2]<<8)
							   |((uint32_t)device->rawData[3]);

			hum = hum >> 4;

	uint32_t temp =  ((uint32_t)device->rawData[3]<<16)
									   |((uint32_t)device->rawData[4]<<8)
									   |((uint32_t)device->rawData[5]);
			temp = temp & 0x07FFFF;

			device->humidite = ((float) hum / (float) (1<<20))*100;				// Formule datasheet avec 2^20 <=> 1<<20
			device->temperature = ((float)temp/(float)(1<<20))*200-50;

#if TEST_UART
			UART_Message("**********Value H*****************\r\n", device->huart);
			UART_FloatToDisplay(device->humidite ,4, device->huart);							// Précision de 4 chiffres après la virgule
			UART_Message("**********Value T*****************\r\n", device->huart);
			UART_FloatToDisplay(device->temperature,4, device->huart);
#endif
			return;

}

//Fonction pour reset paramétre par défaut aht20 si problème dans registre
void AHT_Reset(AHT20* device)
{
	uint8_t data = RES;
	HAL_StatusTypeDef ret = HAL_I2C_Master_Transmit(device->hi2c, ADDRESS, &data, sizeof(data), 100);

#if TEST_UART
	if(ret==HAL_OK)
	{
		HAL_Delay(20);
		UART_Message("Reset**** \r\n", device->huart);
	}
#endif
}
#if TEST_UART
//Fonction pour trouver les adresses connecté aux bus I2C
void AHT_Check(AHT20* device)
{
	uint8_t data;
	char tampon[12];
	for(uint16_t i=0;i<127;i++)
	{
		if(HAL_I2C_Master_Receive(device->hi2c, i<<1, &data, 1, 100)==HAL_OK)
		{
			sprintf(tampon,"0x%02X \r\n",i);
			UART_Message(tampon, device->huart);
		}
	}
	UART_Message("************AddressCheck******************\r\n", device->huart);
}
#endif

//Fonction pour attendre que aht20 est finis de mesurer
void AHT_IsBusy(AHT20* device)
{
	HAL_StatusTypeDef ret = 0;
	uint8_t status = 0x98;		// 0X18 and 0X80 = Bit de status à 1 Busy => 1001.1000
#if TEST_UART
	UART_Message("Wait for measure \n\r", device->huart);
#endif

	while(status==0x98)
	{
		ret = HAL_I2C_Master_Receive(device->hi2c, ADDRESS|0X01, &status, 1, 100);				// Si revient à normal on à status = 0x18 donc fin
#if TEST_UART
		if(ret!=HAL_OK)
			UART_Message("Error Master can't read \n\r", device->huart);
#endif
	}
#if TEST_UART
	UART_Message("End of measure \n\r", device->huart);
#endif
}
