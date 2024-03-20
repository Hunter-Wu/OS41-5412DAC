#include "bsp_spi.h"



void  gpio_spi_config(void)
{
    FL_GPIO_InitTypeDef      GPIO_InitStruct;

    GPIO_InitStruct.pin =  FL_GPIO_PIN_8 | FL_GPIO_PIN_9 | FL_GPIO_PIN_11 ;
    GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.pull = FL_DISABLE;
    GPIO_InitStruct.remapPin = FL_DISABLE;
    FL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.pin =  FL_GPIO_PIN_10;
    GPIO_InitStruct.mode = FL_GPIO_MODE_INPUT;
    GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.pull = FL_DISABLE;
    GPIO_InitStruct.remapPin = FL_DISABLE;
    FL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

void spi_set_mosi(uint8_t val)
{
    if(val)
    {
        SPI_MOSI_H();
    }else{
        SPI_MOSI_L();
    }
}

uint8_t spi_read_byte(void)
{
    uint8_t i = 0,val = 0;
    for( i = 0; i < 8; i++)
    {
        val = val << 1;
        SPI_SCLK_L();
		FL_DelayUs(1);
        if(SPI_MISO_GET()){
           val |= 1;
        }
        SPI_SCLK_H();
		FL_DelayUs(1);
    }
    return val;
}

uint8_t spi_write_byte(uint8_t data)
{
	uint8_t i = 0, val = 0;;
    for( i= 0; i < 8; i++)
    {
        SPI_SCLK_L();
        spi_set_mosi(data&0x80);
		FL_DelayUs(1);
        SPI_SCLK_H();
		FL_DelayUs(1);
        data = data << 1;
		if(SPI_MISO_GET()){
           val |= 1;
        }
		val = val << 1;
    }
	return val;
}


