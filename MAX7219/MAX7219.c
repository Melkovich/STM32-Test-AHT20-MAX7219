/*
 * MAX7219.c
 *
 *  Created on: Mar 31, 2024
 *      Author: 33789
 */

#include "main.h"
#include "stm32f4xx_hal_gpio.h"
#include "stm32f4xx_hal_spi.h"
#include "MAX7219.h"
#include "MAX7219_Utility.h"
#include <string.h>

#define CHAR_MAX 43
//LISTE DE CARACTERE
uint8_t CARACTER_R[8] = {0x00,0x00,0x1c,0x22,0x22,0x3c,0x28,0x24}; // Caractere R
uint8_t CARACTER_O[8] = {0x00,0x18,0x24,0x24,0x24,0x18,0x00};
uint8_t CARACTER_M[8];

uint8_t LETTERS[CHAR_MAX][8]={
		{0x00,0x08,0x14,0x14,0x14,0x14,0x08,0x00},/*0*/
		{0x00,0x1C,0x08,0x08,0x08,0x0C,0x08,0x00},/*1*/
		{0x00,0x3C,0x04,0x18,0x20,0x20,0x1C,0x00},/*2*/
		{0x00,0x1C,0x20,0x20,0x1C,0x20,0x1C,0x00},/*3*/
		{0x00,0x10,0x10,0x3C,0x14,0x14,0x04,0x00},/*4*/
		{0x00,0x1C,0x20,0x24,0x1C,0x04,0x3C,0x00},/*5*/
		{0x00,0x18,0x24,0x24,0x1C,0x04,0x38,0x00},/*6*/
		{0x00,0x10,0x10,0x38,0x10,0x20,0x3C,0x00},/*7*/
		{0x00,0x18,0x24,0x24,0x18,0x24,0x18,0x00},/*8*/
		{0x00,0x04,0x08,0x18,0x24,0x24,0x18,0x00},/*9*/
		{0x00,0x24,0x24,0x3C,0x24,0x24,0x18,0x00},/*A*/
		{0x00,0x1C,0x24,0x24,0x1C,0x24,0x1C,0x00},/*B*/
		{0x00,0x38,0x04,0x04,0x04,0x04,0x38,0x00},/*C*/
		{0x00,0x1C,0x24,0x24,0x24,0x24,0x1C,0x00},/*D*/
		{0x00,0x3C,0x04,0x3C,0x04,0x3C,0x00,0x00},/*E*/
		{0x00,0x04,0x04,0x1C,0x04,0x04,0x3C,0x00},/*F*/
		{0x00,0x38,0x24,0x34,0x04,0x24,0x18,0x00},/*G*/
		{0x00,0x24,0x24,0x3C,0x24,0x24,0x00,0x00},/*H*/
		{0x00,0x1C,0x08,0x08,0x08,0x1C,0x00,0x00},/*I*/
		{0x00,0x1C,0x10,0x10,0x10,0x38,0x00,0x00},/*J*/
		{0x00,0x24,0x14,0x0C,0x14,0x24,0x04,0x00},/*K*/
		{0x00,0x3C,0x04,0x04,0x04,0x04,0x04,0x00},/*L*/
		{0x00,0x22,0x22,0x2A,0x36,0x22,0x00,0x00},/*M*/
		{0x00,0x22,0x32,0x2A,0x26,0x22,0x22,0x00},/*N*/
		{0x00,0x18,0x24,0x24,0x24,0x24,0x18,0x00},/*O*/
		{0x00,0x04,0x04,0x1C,0x24,0x24,0x1C,0x00},/*P*/
		{0x40,0x38,0x34,0x24,0x24,0x24,0x18,0x00},/*Q*/
		{0x00,0x24,0x14,0x1C,0x24,0x24,0x1C,0x00},/*R*/
		{0x18,0x24,0x20,0x18,0x04,0x24,0x18,0x00},/*S*/
		{0x00,0x08,0x08,0x08,0x08,0x08,0x3E,0x00},/*T*/
		{0x00,0x18,0x24,0x24,0x24,0x24,0x00,0x00},/*U*/
		{0x00,0x08,0x14,0x22,0x22,0x22,0x22,0x00},/*V*/
		{0x00,0x44,0x6C,0x54,0x44,0x44,0x00,0x00},/*W*/
		{0x00,0x28,0x28,0x10,0x28,0x28,0X00,0x00},/*X*/
		{0x10,0x10,0x10,0x18,0x24,0x24,0x24,0x00},/*Y*/
		{0x00,0x7C,0x08,0x10,0x20,0x7C,0x00,0x00},/*Z*/
		{0x00,0x04,0x00,0x00,0x00,0x00,0x00,0x00},/*.*/
		{0x00,0x04,0x04,0x00,0x00,0x04,0x04,0x00},/*:*/
		{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/* */
		{0x00,0x00,0x00,0x00,0x08,0x14,0x08,0x00},/*°*/
		{0x00,0x00,0x24,0x08,0x10,0x24,0x00,0x00},/*%*/
		{0x00,0x18,0x3C,0x7E,0xFF,0xE7,0x42,0x00},/*HEART ON*/
		{0xFF,0xE7,0xC3,0x81,0x00,0x18,0xBD,0xFF},/*HEART OFF*/
};

//Fonction pour tester alphabet
void Test_Alphabet(Matrix * device,int nb)
{
	 uint8_t Commande[2];
	 HAL_StatusTypeDef ret = 0;

	 for(int a=0;a<nb;a++)
	 {
		 for(int j=1;j<=8;j++)									//Compte les lignes
		 {
			 Commande[0] = j;
			 Commande[1]= LETTERS[a][j-1];

			 for(int i=0;i<3;i++)								//Permet que chaque matrices aient bien reçu la même donnée à afficher sur leurs lignes
			 {
				 MAX7219_SendData(device,Commande[0], Commande[1]);
			 }

		 }
		 HAL_Delay(1000);
	 }
}

//Fonction pour tester l'utilisation des no-op
void Test_Blink_Heart(Matrix * device)
{
	 uint8_t Commande[6]={0x00};								// Les 0 servent pour faire le no-op
	 HAL_StatusTypeDef ret = 0;

	static int blink = 41;

	 switch (blink) {
		case 41:
			blink = 42;
			break;
		case 42 :
			blink = 41;
		default:
			blink = 41;											// Si bug on a juste un coeur
			break;
	}

		 for(int j=1;j<=8;j++)
		 {
			 Commande[0] = j;									// Comme 3 matrices, en cascades et chacune sur 16 bits, on peut répartir les ordres d'affichages comme suit : 0-1 = Matrice3, 2-3 = Matrice2, 4-5 = Matrice1
			 Commande[1]= LETTERS[blink][j-1];

			 MAX7219_SendPackage(device, Commande,sizeof(Commande));

		 }
		 HAL_Delay(1000);
}

//Fonction pour tester affichage de plusieurs caractéres en même temps + décalage sur la droite
void Test_Romain_Slide(Matrix * device)
{
	 uint8_t Commande[6]={0x00};								// Les 0 servent pour faire le no-op
	 HAL_StatusTypeDef ret = 0;

	 uint32_t mask1 =0;											// Mask grand pour faire le registre des 3 matrices en entiers
	 uint8_t mask2 = 0;
	 uint8_t mask3 = 0;

	 for(int i=0;i<36;i++)
	 {
		 for(int j=1;j<=8;j++)
		 {
			 mask1 = LETTERS[27][j-1]<<i>>10|LETTERS[24][j-1]<<i>>6|LETTERS[22][j-1]<<i>>1|LETTERS[10][j-1]<<i<<3|LETTERS[18][j-1]<<i<<7|LETTERS[23][j-1]<<i<<11;		// Mot ROMAIN écris sur 32 bits le code permet de placer les décalages entre les caractéres sur chaque matrice selon longueur
			 mask2 = mask1>>8 ;																	// décalage des 8 bits de poids forts à droite
			 mask3 = mask1>>16;																// décalage des 16 bits de poids forts à droite lors de déplacement

			 Commande[4] = j;
			 Commande[5]= mask1;			//Matrice 1

			 Commande[2] = j;
			 Commande[3]= mask2;			//Matrice 2

			 Commande[0] = j;
			 Commande[1]= mask3;			//Matrice 3

			 MAX7219_SendPackage(device, Commande,sizeof(Commande));


		 }
	HAL_Delay(100);
	 }
}

//Fonction pour écrire des char sur matrice
void Display_Print_Matrice(Matrix * device, char* print_data,uint8_t opt)
{
	char tmp;
	device->Size = strlen(print_data);

	for(int i = 0; i<device->Size; i++)
	{
		tmp = *(print_data+i);

		switch (tmp) {
		case '0':
			for(int j =0;j<8;j++)
				device->Raw[i][j] = LETTERS[0][j];
			device->Shift[i] = 4;
			break;
		case '1':
			for(int j =0;j<8;j++)
				device->Raw[i][j] = LETTERS[1][j];
			device->Shift[i] = 4;
			break;
		case '2':
			for(int j =0;j<8;j++)
				device->Raw[i][j] = LETTERS[2][j];
			device->Shift[i] = 5;
			break;
		case '3':
			for(int j =0;j<8;j++)
				device->Raw[i][j] = LETTERS[3][j];
			device->Shift[i] = 5;
			break;
		case '4':
			for(int j =0;j<8;j++)
				device->Raw[i][j] = LETTERS[4][j];
			device->Shift[i] = 5;
			break;
		case '5':
			for(int j =0;j<8;j++)
				device->Raw[i][j] = LETTERS[5][j];
			device->Shift[i] = 5;
			break;
		case '6':
			for(int j =0;j<8;j++)
				device->Raw[i][j] = LETTERS[6][j];
			device->Shift[i] = 5;
			break;
		case '7':
			for(int j =0;j<8;j++)
				device->Raw[i][j] = LETTERS[7][j];
			device->Shift[i] = 5;
			break;
		case '8':
			for(int j =0;j<8;j++)
				device->Raw[i][j] = LETTERS[8][j];
			device->Shift[i] = 5;
			break;
		case '9':
			for(int j =0;j<8;j++)
				device->Raw[i][j] = LETTERS[9][j];
			device->Shift[i] = 5;
			break;
		case 'A':
			for(int j =0;j<8;j++)
				device->Raw[i][j] = LETTERS[10][j];
			device->Shift[i] = 5;
			break;
		case 'B':
			for(int j =0;j<8;j++)
				device->Raw[i][j] = LETTERS[11][j];
			device->Shift[i] = 5;
			break;
		case 'C':
			for(int j =0;j<8;j++)
				device->Raw[i][j] = LETTERS[12][j];
			device->Shift[i] = 5;
			break;
		case 'D':
			for(int j =0;j<8;j++)
				device->Raw[i][j] = LETTERS[13][j];
			device->Shift[i] = 5;
			break;
		case 'E':
			for(int j =0;j<8;j++)
				device->Raw[i][j] = LETTERS[14][j];
			device->Shift[i] = 5;
			break;
		case 'F':
			for(int j =0;j<8;j++)
				device->Raw[i][j] = LETTERS[15][j];
			device->Shift[i] = 5;
			break;
		case 'G':
			for(int j =0;j<8;j++)
				device->Raw[i][j] = LETTERS[16][j];
			device->Shift[i] = 5;
			break;
		case 'H':
			for(int j =0;j<8;j++)
				device->Raw[i][j] = LETTERS[17][j];
			device->Shift[i] = 5;
			break;
		case 'I':
			for(int j =0;j<8;j++)
				device->Raw[i][j] = LETTERS[18][j];
			device->Shift[i] = 4;
			break;
		case 'J':
			for(int j =0;j<8;j++)
				device->Raw[i][j] = LETTERS[19][j];
			device->Shift[i] = 5;
			break;
		case 'K':
			for(int j =0;j<8;j++)
				device->Raw[i][j] = LETTERS[20][j];
			device->Shift[i] = 5;
			break;
		case 'L':
			for(int j =0;j<8;j++)
				device->Raw[i][j] = LETTERS[21][j];
			device->Shift[i] = 6;
			break;
		case 'M':
			for(int j =0;j<8;j++)
				device->Raw[i][j] = LETTERS[22][j];
			device->Shift[i] = 6;
			break;
		case 'N':
			for(int j =0;j<8;j++)
				device->Raw[i][j] = LETTERS[23][j];
			device->Shift[i] = 5;
			break;
		case 'O':
			for(int j =0;j<8;j++)
				device->Raw[i][j] = LETTERS[24][j];
			device->Shift[i] = 5;
			break;
		case 'P':
			for(int j =0;j<8;j++)
				device->Raw[i][j] = LETTERS[25][j];
			device->Shift[i] = 5;
			break;
		case 'Q':
			for(int j =0;j<8;j++)
				device->Raw[i][j] = LETTERS[26][j];
			device->Shift[i] = 5;
			break;
		case 'R':
			for(int j =0;j<8;j++)
				device->Raw[i][j] = LETTERS[27][j];
			device->Shift[i] = 5;
			break;
		case 'S':
			for(int j =0;j<8;j++)
				device->Raw[i][j] = LETTERS[28][j];
			device->Shift[i] = 5;
			break;
		case 'T':
			for(int j =0;j<8;j++)
				device->Raw[i][j] = LETTERS[29][j];
			device->Shift[i] = 5;
			break;
		case 'U':
			for(int j =0;j<8;j++)
				device->Raw[i][j] = LETTERS[30][j];
			device->Shift[i] = 5;
			break;
		case 'V':
			for(int j =0;j<8;j++)
				device->Raw[i][j] = LETTERS[31][j];
			device->Shift[i] = 5;
			break;
		case 'W':
			for(int j =0;j<8;j++)
				device->Raw[i][j] = LETTERS[32][j];
			device->Shift[i] = 5;
			break;
		case 'X':
			for(int j =0;j<8;j++)
				device->Raw[i][j] = LETTERS[33][j];
			device->Shift[i] = 5;
			break;
		case 'Y':
			for(int j =0;j<8;j++)
				device->Raw[i][j] = LETTERS[34][j];
			device->Shift[i] = 5;
			break;
		case 'Z':
			for(int j =0;j<8;j++)
				device->Raw[i][j] = LETTERS[35][j];
			device->Shift[i] = 5;
			break;
		case '.':
			for(int j =0;j<8;j++)
				device->Raw[i][j] = LETTERS[36][j];
			device->Shift[i] = 2;
			break;
		case ':':
			for(int j =0;j<8;j++)
				device->Raw[i][j] = LETTERS[37][j];
			device->Shift[i] = 2;
			break;
		case ' ':
			for(int j =0;j<8;j++)
				device->Raw[i][j] = LETTERS[38][j];
			device->Shift[i] = 2;
			break;
		case 'd':										// Provisoire le temp de régler pb °
			for(int j =0;j<8;j++)
				device->Raw[i][j] = LETTERS[39][j];
			device->Shift[i] = 4;
			break;
		case '%':
			for(int j =0;j<8;j++)
				device->Raw[i][j] = LETTERS[40][j];
			device->Shift[i] = 5;
			break;
		case '\0':
			break;
		default:
			break;
		}

	}
	Display_Raw_Matrice(device,opt);
}


void Display_Raw_Matrice(Matrix * device,uint8_t opt)
{
	 uint8_t Commande[6]={0x00};
	 int shift = 0;
	 int cpt = 0;

	 uint32_t mask1 =0;											// Mask grand pour faire le registre des 3 matrices en entiers
	 uint8_t mask2 = 0;
	 uint8_t mask3 = 0;

	 for(int j=1;j<=8;j++)
	 {
		 	 while(cpt<device->Size)
		 	 {
		 		 mask1 += device->Raw[cpt][j-1]<<shift<<opt>>2;									//2 sert à recentrer au début de matrice
		 		 shift += device->Shift[cpt];
		 		 cpt++;

		 	 }
		 	 cpt = 0;
		 	 shift = 0;
		 	 mask2 = mask1>>8 ;																	// décalage des 8 bits de poids forts à droite
			 mask3 = mask1>>16;																// décalage des 16 bits de poids forts à droite lors de déplacement

			 Commande[4] = j;
			 Commande[5]= mask1;			//Matrice 1

			 Commande[2] = j;
			 Commande[3]= mask2;			//Matrice 2

			 Commande[0] = j;
			 Commande[1]= mask3;			//Matrice 3

		 MAX7219_SendPackage(device, Commande,sizeof(Commande));

		 mask1=0;
		 mask2=0;
		 mask3=0;

	 }

}

