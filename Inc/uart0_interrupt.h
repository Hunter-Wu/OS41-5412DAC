#ifndef __UART_H__
#define __UART_H__
#include "main.h"

void Test_Uart0(void);
//extern void Test_Uart1();
//extern void Test_Uart4();
//extern void Test_Uart5();

extern uint8_t Adr485;
extern uint8_t Baud485;
extern uint8_t Mode485;


void Uart0DMA_Config(uint8_t *buffer, uint32_t length);
void Uart3_Rcv_Handle(void);
void Uart_Task(void);
void Flash_CaclDate_Write(void);
void Flash_CmdDate_Write(void);
void Flash_CaclDate_Read(void);
void Flash_CmdDate_Read(void);
void Uart_Send_L(uint8_t cmd,uint8_t data);
void Uart_Send_CMD(uint8_t cmd,uint16_t data);

#endif
