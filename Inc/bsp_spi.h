#ifndef BSP_SPI_H
#define BSP_SPI_H
#include "fm33lc0xx_fl_gpio.h"
#include "fm33lc0xx_fl_spi.h"


#define SPI_SSN_PIN       	FL_GPIO_PIN_8 	//
#define SPI_SCLK_PIN       	FL_GPIO_PIN_9 	//
#define SPI_MISO_PIN       	FL_GPIO_PIN_10 	//
#define SPI_MOSI_PIN       	FL_GPIO_PIN_11 	//
#define SPI_Port  					GPIOB

#define SPI_SSN_H()        	FL_GPIO_SetOutputPin(SPI_Port, SPI_SSN_PIN)
#define SPI_SSN_L()        	FL_GPIO_ResetOutputPin(SPI_Port, SPI_SSN_PIN)

#define SPI_SCLK_H()        FL_GPIO_SetOutputPin(SPI_Port, SPI_SCLK_PIN)
#define SPI_SCLK_L()       	FL_GPIO_ResetOutputPin(SPI_Port, SPI_SCLK_PIN)

#define SPI_MISO_H()        FL_GPIO_SetOutputPin(SPI_Port, SPI_MISO_PIN)
#define SPI_MISO_L()       	FL_GPIO_ResetOutputPin(SPI_Port, SPI_MISO_PIN)
#define SPI_MISO_GET()  		FL_GPIO_GetInputPin(SPI_Port, SPI_MISO_PIN)

#define SPI_MOSI_H()        FL_GPIO_SetOutputPin(SPI_Port, SPI_MOSI_PIN)
#define SPI_MOSI_L()       	FL_GPIO_ResetOutputPin(SPI_Port, SPI_MOSI_PIN)

void  gpio_spi_config(void);
void spi_set_mosi(uint8_t val);
uint8_t spi_write_byte(uint8_t data);
uint8_t spi_read_byte(void);

#endif
