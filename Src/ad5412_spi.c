
#include "user_init.h"
#include "fm33lc0xx_fl_spi.h"
#include "ad5412_spi.h"
#include "bsp_spi.h"
#include "menu.h"
#if 0
/********************************************************************
*@函数名：bsp_SPI_GPIO_Config
*@功  能：初始化SPI对应的GPIO
*@形  参：NULL
*@返回值：NULL
*@备  注：NULL
********************************************************************/
void bsp_SPI_GPIO_Config(void)
{
	FL_GPIO_InitTypeDef      GPIO_InitStruct;

	GPIO_InitStruct.pin =   FL_GPIO_PIN_8 | FL_GPIO_PIN_9 | FL_GPIO_PIN_10 | FL_GPIO_PIN_11;
	GPIO_InitStruct.mode = FL_GPIO_MODE_DIGITAL;
	GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.pull = FL_DISABLE;
	GPIO_InitStruct.remapPin = FL_DISABLE;
	FL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}


/********************************************************************
*@函数名：bsp_SPI_Config
*@功  能：初始化SPI功能，全双工
*@形  参：NULL
*@返回值：NULL
*@备  注：NULL
********************************************************************/
static void bsp_SPI_Config(void)
{
	FL_SPI_InitTypeDef     SPI_InitStruct;

	SPI_InitStruct.transferMode = FL_SPI_TRANSFER_MODE_FULL_DUPLEX;
	SPI_InitStruct.mode = FL_SPI_WORK_MODE_MASTER;
	SPI_InitStruct.dataWidth = FL_SPI_DATA_WIDTH_8B;
	SPI_InitStruct.clockPolarity = FL_SPI_POLARITY_NORMAL;
	SPI_InitStruct.clockPhase = FL_SPI_PHASE_EDGE2;
	SPI_InitStruct.softControl = FL_ENABLE;
	SPI_InitStruct.baudRate = FL_SPI_BAUDRATE_DIV2;
	SPI_InitStruct.bitOrder = FL_SPI_BIT_ORDER_MSB_FIRST;

	FL_SPI_Init(SPI1, &SPI_InitStruct);

	FL_SPI_ClearTXBuff(SPI1);
	FL_SPI_ClearRXBuff(SPI1);
}


/********************************************************************
*@函数名：bsp_SPI_Init
*@功  能：初始化SPI
*@形  参：NULL
*@返回值：NULL
*@备  注：NULL
********************************************************************/
void bsp_SPI_Init(void)
{
	bsp_SPI_GPIO_Config();
	bsp_SPI_Config();
}



/********************************************************************
*@函数名：bsp_SPI_NSS_Low
*@功  能：设置SPI片选信号为低
*@形  参：NULL
*@返回值：NULL
*@备  注：NULL
********************************************************************/
void bsp_SPI_NSS_Low(void)
{
  FL_SPI_SetSSNPin(SPI1, FL_SPI_SSN_LOW);
}

/********************************************************************
*@函数名：bsp_SPI_NSS_High
*@功  能：设置SPI片选信号为高
*@形  参：NULL
*@返回值：NULL
*@备  注：NULL
********************************************************************/
void bsp_SPI_NSS_High(void)
{
  FL_SPI_SetSSNPin(SPI1, FL_SPI_SSN_HIGH);
}

#define SPI_TIMEOUT           0XFFFF
/********************************************************************
*@函数名：bsp_SPI_WriteByte
*@功  能：通过SPI写一个字节的数据
*@形  参：NULL
*@返回值：NULL
*@备  注：NULL
********************************************************************/
uint8_t bsp_SPI_WriteByte(uint8_t byte)
{
	uint8_t data = 0;
	uint32_t timeout;

	timeout = 0;
	FL_SPI_WriteTXBuff(SPI1, byte);
	while(FL_SPI_IsActiveFlag_TXBuffEmpty(SPI1) == FL_RESET)
	{
	  timeout++;
		if(timeout > SPI_TIMEOUT)
		{
		  goto exit;
		}
	}
	
	timeout = 0;
	while(FL_SPI_IsActiveFlag_RXBuffFull(SPI1) == FL_RESET)
	{
		timeout++;
		if(timeout > SPI_TIMEOUT)
		{
		  goto exit;
		}
	}
	data = FL_SPI_ReadRXBuff(SPI1);
	exit:
	
	return data;
}


/********************************************************************
*@函数名：bsp_SPI_ReadByte
*@功  能：通过SPI读一个字节的数据
*@形  参：NULL
*@返回值：NULL
*@备  注：NULL
********************************************************************/
uint8_t bsp_SPI_ReadByte(void)
{
  return bsp_SPI_WriteByte(0XFF);
}



/********************************************************************
*@函数名：bsp_SPI_WriteBuf
*@功  能：通过SPI连续写数据
*@形  参：pbuf 指向缓存数据的指针  psize 需要写的数据字节数
*@返回值：NULL
*@备  注：NULL
********************************************************************/
void bsp_SPI_WriteBuf(uint8_t *pbuf, uint16_t psize)
{
	uint32_t timeout, i;
  	
	for(i=0; i<psize; i++)
	{
		timeout = 0;
		FL_SPI_WriteTXBuff(SPI1, pbuf[i]);
		while(FL_SPI_IsActiveFlag_RXBuffFull(SPI1) == FL_SET)
		{
			timeout++;
			if(timeout > SPI_TIMEOUT)
			{
				return;
			}
		}
		FL_SPI_ReadRXBuff(SPI1);
	}
}



/********************************************************************
*@函数名：bsp_SPI_WriteBuf
*@功  能：通过SPI连续读数据
*@形  参：pbuf 指向缓存数据的指针  psize 需要读的数据字节数
*@返回值：实际读取的数据
*@备  注：NULL
********************************************************************/
uint16_t bsp_SPI_ReadBuf(uint8_t *pbuf, uint16_t psize)
{
	uint32_t timeout, i;
	for(i=0; i<psize; i++)
	{
		timeout = 0;
		FL_SPI_WriteTXBuff(SPI1, 0x55);
		while(FL_SPI_IsActiveFlag_RXBuffFull(SPI1) == FL_SET)
		{
			timeout++;
			if(timeout > SPI_TIMEOUT)
			{
				goto exit;
			}
		}
		pbuf[i] = FL_SPI_ReadRXBuff(SPI1);
	}
	exit:
	return i;
}

#endif

void AD5412_Reset(void)
{
	SPI_SSN_L();
	spi_write_byte(0x56);//reset register 
	spi_write_byte(0x00);
	spi_write_byte(0x01);
	SPI_SSN_H();
}

void AD5412_Config(void)
{
	SPI_SSN_L();
	spi_write_byte(0x55);//control register 
	spi_write_byte(0x00);
	spi_write_byte(0x01);
	SPI_SSN_H();
}

void AD5412_Iout0_24mA(void)
{
	SPI_SSN_L();
	spi_write_byte(0x55);//control register 
	spi_write_byte(0x10);//output enable
	spi_write_byte(0x07);
	SPI_SSN_H();
}

void AD5412_Iout_0_20mA(void)
{
	SPI_SSN_L();
	spi_write_byte(0x55);//control register 
	spi_write_byte(0x10);//output enable
	spi_write_byte(0x06);
	SPI_SSN_H();
}

void AD5412_Iout_4_20mA(void)
{
	SPI_SSN_L();
	spi_write_byte(0x55);//control register 
	spi_write_byte(0x10);//output enable
	spi_write_byte(0x05);
	SPI_SSN_H();
}

void AD5412_V_5v(void)
{
	SPI_SSN_L();
	spi_write_byte(0x55);//control register 
	spi_write_byte(0x50);// output enable,ovr =1
	spi_write_byte(0x00);
	SPI_SSN_H();
}

void AD5412_V_10v(void)
{
	SPI_SSN_L();
	spi_write_byte(0x55);//control register 
	spi_write_byte(0x50);//output enable, ovr =1
	spi_write_byte(0x01);
	SPI_SSN_H();
}

void AD5412_V_5v_(void)
{
	SPI_SSN_L();
	spi_write_byte(0x55);//control register 
	spi_write_byte(0x50);//output enable, ovr = 1
	spi_write_byte(0x02);
	SPI_SSN_H();
}

void AD5412_V_10v_(void)
{
	SPI_SSN_L();
	spi_write_byte(0x55);//control register 
	spi_write_byte(0x50);//output enable
	spi_write_byte(0x03);
	SPI_SSN_H();
}


void AD5412_Init(void)
{
	AD5412_Reset();
	AD5412_Config();
	if(Aout_Mode == Iout_Mode)
	{
		AD5412_Iout0_24mA();
	}
	else if(Aout_Mode == Vout_Mode)
	{
		AD5412_V_10v();
	}
}

void AD5412_OutMode_change(void)
{
	AD5412_Reset();
	AD5412_Config();
	if(Aout_Mode == Iout_Mode)
	{
		AD5412_Iout0_24mA();
	}
	else if(Aout_Mode == Vout_Mode)
	{
		AD5412_V_10v();
	}
}


void AD5412_Data(uint16_t dat)
{
	SPI_SSN_L();
	spi_write_byte(0x01);//data register 
	spi_write_byte(dat >> 8);  //data high 8bit
	spi_write_byte(dat&0xff);  //data low 8bit
	SPI_SSN_H();
}
