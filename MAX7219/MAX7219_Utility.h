/*
 * MAX7219_Utility.h
 *
 *  Created on: Apr 4, 2024
 *      Author: 33789
 */

#ifndef MAX7219_UTILITY_H_
#define MAX7219_UTILITY_H_


void MAX7219_init(SPI_HandleTypeDef *hspi,Matrix *device);
void MAX7219_Test_Mode(SPI_HandleTypeDef *hspi);
void MAX7219_off(Matrix *device);
void MAX7219_on(Matrix *device);

//Fonction à rajouter dans des boucles
void MAX7219_no_op(Matrix *device);
void MAX7219_SendData(Matrix *device,uint8_t adresse,uint8_t data);
void MAX7219_clear_all(Matrix *device);
void MAX7219_clear(Matrix *device);
void MAX7219_SendPackage(Matrix *device,uint8_t *commande,uint8_t size);


//Fonction utiliser pour init *3 matrice led (à cause de daisy chain)
void MAX7219_decode_mode_off(Matrix * device);
void MAX7219_Set_Intensity(Matrix * device);
void MAX7219_Scan_Limit(Matrix * device);
void MAX7219_ShutDown_off(Matrix * device);
void MAX7219_Display_Test_off(Matrix * device);

#endif /* MAX7219_UTILITY_H_ */
