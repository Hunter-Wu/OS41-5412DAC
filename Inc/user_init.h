#ifndef __USER_INIT_H__
#define __USER_INIT_H__

#include "main.h"
#include <stdio.h>
#include "VI5300_API.h"

#define AOUT_USE_SPI  1
#define AOUT_USE_I2C  0


#define Dout_Select       			FL_GPIO_PIN_8 	//
#define Dout_Select_Port  			GPIOA
#define Dout_Select_NPN()       	FL_GPIO_ResetOutputPin(Dout_Select_Port, Dout_Select)
#define Dout_Select_PNP()       	FL_GPIO_SetOutputPin(Dout_Select_Port, Dout_Select)

#define Dout_Switch       			FL_GPIO_PIN_9 	//
#define Dout_Switch_Port  			GPIOA
#define Dout_Switch_Off()       	FL_GPIO_ResetOutputPin(Dout_Switch_Port, Dout_Switch)
#define Dout_Switch_On()        	FL_GPIO_SetOutputPin(Dout_Switch_Port, Dout_Switch)

#define Laser_indicator       		FL_GPIO_PIN_5 	
#define Laser_indicator_Port  		GPIOC
#define Laser_indicator_On()  		FL_GPIO_SetOutputPin(Laser_indicator_Port, Laser_indicator)
#define Laser_indicator_Off()  		FL_GPIO_ResetOutputPin(Laser_indicator_Port, Laser_indicator)

#define IO_Led 						FL_GPIO_PIN_10     //输出指示灯
#define IO_Led_Port 				GPIOC
#define IO_Led_On()       			FL_GPIO_ResetOutputPin(IO_Led_Port, IO_Led)
#define IO_Led_Off()        		FL_GPIO_SetOutputPin(IO_Led_Port, IO_Led)

/*
#define MCU_485_EN        			FL_GPIO_PIN_15 	//芯片使能IO
#define MCU_485_EN_Port   			GPIOA
#define RS485_Send()  				FL_GPIO_SetOutputPin(MCU_485_EN_Port, MCU_485_EN)
#define RS485_Receive()  			FL_GPIO_ResetOutputPin(MCU_485_EN_Port, MCU_485_EN)
*/

#define VL53L1X_XSHUT_Pin       	FL_GPIO_PIN_3 //芯片使能IO
#define VL53L1X_XSHUT_GPIO_Port 	GPIOC

//input
#define VL53L1X_INT_Pin         	FL_GPIO_PIN_2  //中断IO
#define VL53L1X_INT_GPIO_Port   	GPIOC

//Key_1
#define Key_Teach					FL_GPIO_PIN_9
#define Key_Teach_Port  			GPIOC
#define Read_Key_Teach()			FL_GPIO_GetInputPin(Key_Teach_Port,Key_Teach)


//Key_2
#define Key_Up						FL_GPIO_PIN_13
#define Key_Up_Port  				GPIOB
#define Read_Key_Up()				FL_GPIO_GetInputPin(Key_Up_Port,Key_Up)


//Key_3
#define Key_Down					FL_GPIO_PIN_14
#define Key_Down_Port  				GPIOB
#define Read_Key_Down()				FL_GPIO_GetInputPin(Key_Down_Port,Key_Down)

#define IO_Input					FL_GPIO_PIN_10
#define IO_Input_Port  				GPIOA
#define Read_IO_Input()				FL_GPIO_GetInputPin(IO_Input_Port,IO_Input)


//---------------------------------------------------------------------------------
//#define LED1_Pin FL_GPIO_PIN_3     //红灯
//#define LED1_GPIO_Port GPIOC


//#define LED2_Pin FL_GPIO_PIN_2		//绿灯
//#define LED2_GPIO_Port GPIOC

//#define KEY_Pin FL_GPIO_PIN_0
//#define KEY_GPIO_Port GPIOB   //按键
	
//#define WH_IN_Pin FL_GPIO_PIN_10     //白线输入检测
//#define WH_IN_GPIO_Port GPIOC
	
	
//#define OUT2_Pin FL_GPIO_PIN_8		//黑线输出
//#define OUT2_GPIO_Port GPIOC



extern uint8_t caclflag;
extern float  d_k[14];		
extern float  d_b[14];		
extern  uint16_t  Calib_DisBuf[40]; 
extern	uint8_t n;  
//extern	uint8_t LED2_flag;  

extern uint8_t Input_Function;

extern VI5300_OFFSET_Calib_Data stRefTof;

extern uint8_t Aout_Adj;
extern uint16_t Aout_Adjust1;
extern uint16_t Aout_Adjust2;


void MF_DMA_Channel2_Init(void);
void MF_DMA_Channel3_Init(void);

void Calib_kb(void);
void UartInit(uint8_t baud);

void UserInit(void);
void FoutInit(void);
void GPIO_Init(void);
void VI5300_Init(void);
uint16_t Vi5300_GetDisTance(void);
void Waitfor_Vi5300_Getok(void);
void ATIM_Start(void);
void ATIM_IRQHandler(void);

#endif
