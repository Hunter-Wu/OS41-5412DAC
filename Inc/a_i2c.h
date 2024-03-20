#ifndef A_I2C_H_
#define A_I2C_H_
#include <stdint.h>
#include "fm33lc0xx_fl_gpio.h"

#define Aout_I2C_SDA       			FL_GPIO_PIN_14 	
#define Aout_I2C_SDA_Port  			GPIOB
#define Aout_I2C_SDA_H()        	FL_GPIO_SetOutputPin(Aout_I2C_SDA_Port, Aout_I2C_SDA)
#define Aout_I2C_SDA_L()       		FL_GPIO_ResetOutputPin(Aout_I2C_SDA_Port, Aout_I2C_SDA)
#define Aout_I2C_ReadSDA()			FL_GPIO_GetInputPin(Aout_I2C_SDA_Port,Aout_I2C_SDA)

#define Aout_I2C_SCL       			FL_GPIO_PIN_13 	
#define Aout_I2C_SCL_Port  			GPIOB
#define Aout_I2C_SCL_H()        	FL_GPIO_SetOutputPin(Aout_I2C_SCL_Port, Aout_I2C_SCL)
#define Aout_I2C_SCL_L()       		FL_GPIO_ResetOutputPin(Aout_I2C_SCL_Port, Aout_I2C_SCL)

void Aout_I2C_SDA_Out(void);
void Aout_I2C_SDA_In(void);
void Aout_I2c_Start(void);
void Aout_I2c_Stop(void);
uint8_t Aout_I2C_Send_Byte(uint8_t dat);
void Aout_Select_10V(void);
void Aout_I2C_Send_Data(uint16_t dat);
#endif
