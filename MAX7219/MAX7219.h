/*
 * MAX7219.h
 *
 *  Created on: 16 Jan 2023
 *      Author: Castanier Romain
 */

#ifndef MAX7219_H_
#define MAX7219_H_


typedef struct {

	SPI_HandleTypeDef *hspi;
	uint8_t Raw[10][8];					//Stock Cara pour Matrice
	uint8_t Size;						//Nb Cara à afficher
	uint8_t Shift[10];					//Shiftage des Cara

	uint8_t Raw_2[10][8];
	uint8_t Size_2;
	uint8_t Shift_2[10];

}Matrix;

#define DEFAULT 0
#define CENTER 2


//Fonction de test pour comprendre le fonctionnement du MAX7219
void Test_Alphabet(Matrix * device,int nb);
void Test_Blink_Heart(Matrix * device);
void Test_Romain_Slide_rigth(Matrix * device);
void Test_Data_Slide_up(Matrix * device, char * data, uint8_t opt);

//Fonction utilisable comme bibliothéque
void Display_Show_Matrice(Matrix * device,uint8_t opt);
void Display_Print_Matrice(Matrix * device, char* print_data, uint8_t choose_raw);
void Display_Reset_Data_Matrice_All(Matrix * device);
void Display_Change_Data_Matrice_Up(Matrix * device, uint8_t opt);

#endif /* MAX7219_H_ */
