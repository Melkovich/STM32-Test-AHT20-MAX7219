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


//Fonction pour envoyer du texte à TeraTerm
void UART_Message(char * buffer, UART_HandleTypeDef* huart2)
{
	uint32_t len = strlen(buffer);
	HAL_UART_Transmit(huart2,(uint8_t*)buffer, len, 100);
}

//Fonction pour décomposer un float en pls décimal pour afficher sur TeraTerm
void UART_FloatToDisplay(float nb,uint8_t precision,UART_HandleTypeDef* huart2,Matrix *device)
{
	char Buf[6];
	char Buf_2[6];
	char tmpchar;

	/*Bloc pour afficher température sur Matrice à LED*/
	int tmp = nb*powf(10,4);
	sprintf(Buf_2,"%d",tmp);
	tmpchar = Buf_2[2];
	Buf_2[2] = '.';
	Buf_2[3] = tmpchar ;
	Buf_2[4]= 'd';
	Buf_2[5] = '\0';

	tmp = fmod(nb,powf(10,2));//Extraire dizaine
	float puissance = precision;


	sprintf(Buf,"%d",tmp);								//Copier le int en un char pour afficher
	HAL_UART_Transmit(huart2,Buf, 1, 100);

	/*Bloc pour extraire et afficher unité*/
	tmp = nb * pow(10,precision-1) ;
	tmp = fmod(nb,powf(10,1));
	sprintf(Buf,"%d",tmp);
	HAL_UART_Transmit(huart2,Buf, 1, 100);

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
	Display_Print_Matrice(device, Buf_2, DEFAULT);
	UART_Message("\r\n", huart2);
}

//Fonction pour init aht20
void AHT_Init(I2C_HandleTypeDef *hi2c1, UART_HandleTypeDef* huart2)
{
	HAL_Delay(40);																		//Delai avant utilisation du capteur
	uint8_t status=0;
	uint8_t Commande[2]= {0x08,0x00};
	HAL_StatusTypeDef ret=0;

	/*Commande d'initialisation*/
	ret = HAL_I2C_Mem_Write(hi2c1, ADDRESS, INITIALISATION_CMD, 1, Commande,sizeof(Commande), 100);
	if(ret!=HAL_OK)
		UART_Message("Error Init cmd not send \n\r", huart2);

	/*Vérifier que aht20 est bien prêt pour convertir soit 0x18 = 0001.1000 avec Bit[7] = 0 -> Status signifie est Free et Bit[3] =  1 -> siginfie Calibré */
	ret = HAL_I2C_Master_Receive(hi2c1, ADDRESS|0X01, &status, 1, 100);
	if(ret!=HAL_OK)
		UART_Message("Error Master do not receive \n\r", huart2);
	if((status&0x18)==0x18)
	{
		UART_Message("Init OK \n\r", huart2);
	}
	else
	{
		UART_Message("Error init fail \n\r", huart2);
	}

}

//Fonction pour lancer une conversion
void AHT_TrigMeasure(I2C_HandleTypeDef* hi2c1,UART_HandleTypeDef* huart2)
{

	uint8_t Commande[2] = {0x33,0x00};
	HAL_StatusTypeDef ret = HAL_I2C_Mem_Write(hi2c1, ADDRESS, TRIG_MEASURE, I2C_MEMADD_SIZE_8BIT, Commande, 2, HAL_MAX_DELAY);				// Ecrit dans esclave à adresse de trigger measure avec les lignes de codes associées (voir datasheet)

	if(ret==HAL_OK)
	{
		UART_Message("Trigger OK \n\r", huart2);
	}
	else
	{
		UART_Message("Trigger KO \n\r", huart2);
	}

}

//Fonction pour lire donnée, convertir et afficher
void AHT_Readdata(I2C_HandleTypeDef* hi2c1,UART_HandleTypeDef* huart2,Matrix *device)
{

	uint8_t rawData[6]={0};
	int tmp = 0;

	HAL_StatusTypeDef ret = HAL_I2C_Master_Receive(hi2c1, ADDRESS|0x01, rawData, sizeof(rawData), 100);
	if(ret!=HAL_OK)
		UART_Message("Read KO \n\r", huart2);

	uint32_t hum =  ((uint32_t)rawData[1]<<16)									//	Prend les octets de donnée de humidité on les mets dans un gros uint32 pour les ranger
							   |((uint32_t)rawData[2]<<8)
							   |((uint32_t)rawData[3]);

			hum = hum >> 4;

	uint32_t temp =  ((uint32_t)rawData[3]<<16)
									   |((uint32_t)rawData[4]<<8)
									   |((uint32_t)rawData[5]);
			temp = temp & 0x07FFFF;

			float humidite = ((float) hum / (float) (1<<20))*100;				// Formule datasheet avec 2^20 <=> 1<<20
			float temperature = ((float)temp/(float)(1<<20))*200-50;


//			UART_Message("**********Value H*****************\r\n", huart2);
//			UART_FloatToDisplay(humidite,4, huart2);							// Précision de 4 chiffres après la virgule
			UART_Message("**********Value T*****************\r\n", huart2);
			UART_FloatToDisplay(temperature,2, huart2,device);
			return;

}

//Fonction pour reset paramétre par défaut aht20 si problème dans registre
void AHT_Reset(I2C_HandleTypeDef* hi2c1,UART_HandleTypeDef* huart2)
{
	uint8_t data = RES;
	if(HAL_I2C_Master_Transmit(hi2c1, ADDRESS, &data, sizeof(data), 100)==HAL_OK)
	{
		HAL_Delay(20);
		UART_Message("Reset**** \r\n", huart2);
	}
}

//Fonction pour trouver les adresses connecté aux bus I2C
void AHT_Check(I2C_HandleTypeDef* hi2c1,UART_HandleTypeDef* huart2)
{
	uint8_t data;
	char tampon[12];
	for(uint16_t i=0;i<127;i++)
	{
		if(HAL_I2C_Master_Receive(hi2c1, i<<1, &data, 1, 100)==HAL_OK)
		{
			sprintf(tampon,"0x%02X \r\n",i);
			UART_Message(tampon, huart2);
		}
	}
	UART_Message("************AddressCheck******************\r\n", huart2);
}

//Fonction pour attendre que aht20 est finis de mesurer
void AHT_IsBusy(I2C_HandleTypeDef* hi2c1,UART_HandleTypeDef* huart2)
{
	HAL_StatusTypeDef ret = 0;
	uint8_t status = 0x98;		// 0X18 and 0X80 = Bit de status à 1 Busy => 1001.1000
	UART_Message("Wait for measure \n\r", huart2);
	while(status==0x98)
	{
		ret = HAL_I2C_Master_Receive(hi2c1, ADDRESS|0X01, &status, 1, 100);				// Si revient à normal on à status = 0x18 donc fin
		if(ret!=HAL_OK)
			UART_Message("Error Master can't read \n\r", huart2);

	}
	UART_Message("End of measure \n\r", huart2);
}
