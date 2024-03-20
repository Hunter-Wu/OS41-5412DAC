#ifndef AD54XX_SPI_H_
#define AD54XX_SPI_H_
#include <stdint.h>
#include "fm33lc0xx_fl_gpio.h"

//output  ad5412
#define Aout_SPI_SSN_PIN       		FL_GPIO_PIN_8 	//
#define Aout_SPI_SCLK_PIN       	FL_GPIO_PIN_9 	//
#define Aout_SPI_MISO_PIN       	FL_GPIO_PIN_10 	//
#define Aout_SPI_MOSI_PIN       	FL_GPIO_PIN_11 	//
#define Aout_SPI_Port  						GPIOB

#define Aout_SPI_SSN_H()        	FL_GPIO_SetOutputPin(Aout_SPI_Port, Aout_SPI_SSN_PIN)
#define Aout_SPI_SSN_L()        	FL_GPIO_SetOutputPin(Aout_SPI_Port, Aout_SPI_SSN_PIN)

void SPI_Init(void);
void AD5412_Reset(void);
void AD5412_Config(void);
void AD5412_Iout0_24mA(void);
void AD5412_Iout0_20mA(void);
void AD5412_Iout4_20mA(void);
void AD5412_V_5v(void);
void AD5412_V_10v(void);
void AD5412_V_5v_(void);
void AD5412_V_10v_(void);
void AD5412_Init(void);
void AD5412_Data(uint16_t dat);

#endif
