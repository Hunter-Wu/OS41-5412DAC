#include "a_i2c.h"

void Aout_I2C_SDA_Out(void)
{
	FL_GPIO_InitTypeDef GPIO_InitStruct = {0};

	GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
	GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.pull = FL_DISABLE;
	GPIO_InitStruct.pin = Aout_I2C_SDA;
	FL_GPIO_Init(Aout_I2C_SDA_Port, &GPIO_InitStruct);
}

void Aout_I2C_SDA_In(void)
{
	FL_GPIO_InitTypeDef GPIO_InitStruct = {0};

	GPIO_InitStruct.mode = FL_GPIO_MODE_INPUT;
	GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_OPENDRAIN;
	GPIO_InitStruct.pull = FL_DISABLE;
	GPIO_InitStruct.pin = Aout_I2C_SDA;
	FL_GPIO_Init(Aout_I2C_SDA_Port, &GPIO_InitStruct);
}

void Aout_I2c_Start(void)
{
    Aout_I2C_SDA_H();
    Aout_I2C_SCL_H();
    FL_DelayUs(1);
    Aout_I2C_SDA_L();
    FL_DelayUs(1);
    Aout_I2C_SCL_L();
    FL_DelayUs(1);
} 

void Aout_I2c_Stop(void)
{
    Aout_I2C_SCL_L();
    Aout_I2C_SDA_L();
    FL_DelayUs(1);
    Aout_I2C_SCL_H();
    FL_DelayUs(1);
    Aout_I2C_SDA_H();
    FL_DelayUs(1);
    Aout_I2C_SCL_L();
} 

//return: ack
uint8_t Aout_I2C_Send_Byte(uint8_t dat)
{
	uint8_t ack;	
	uint8_t i;
    for(i=0;i<8;i++)
    {
		if(dat&0x80)
		{
			Aout_I2C_SDA_H();           //SDA -> 1
		}
		else
		{
			Aout_I2C_SDA_L();           //SDA -> 0
		}
		dat = dat << 1;
        FL_DelayUs(1);
        Aout_I2C_SCL_H();               //SCL -> 1
        FL_DelayUs(1);
        Aout_I2C_SCL_L();               //SCL -> 0
        FL_DelayUs(1);
    }

    //ack
    Aout_I2C_SDA_In();                  //SDA is input
    FL_DelayUs(1);
    Aout_I2C_SCL_H();                   //SCL -> 1
    FL_DelayUs(1);
    ack = Aout_I2C_ReadSDA();
    Aout_I2C_SCL_L();                   //SCL -> 0
    FL_DelayUs(1);
    Aout_I2C_SDA_Out();
    return ack;
}

void Aout_I2C_Select_10V(void)
{
    Aout_I2c_Start();
    Aout_I2C_Send_Byte(0xb0);
    Aout_I2C_Send_Byte(0x01);
    Aout_I2C_Send_Byte(0x77);
    Aout_I2c_Stop();
}

void Aout_I2C_Send_Data(uint16_t dat)
{
    uint8_t tempB;
    Aout_I2c_Start();
    Aout_I2C_Send_Byte(0xb0);
    Aout_I2C_Send_Byte(0x02);
    tempB = dat&0xff;
    Aout_I2C_Send_Byte(tempB);
    tempB = (dat>>8);
    Aout_I2C_Send_Byte(tempB);
    Aout_I2c_Stop();
}
