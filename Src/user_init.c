#include "user_init.h"
#include "uart0_interrupt.h"
#include "vi_sw_i2c.h"
#include "VI5300_API.h"
#include "VI5300_Config.h"
#include "VI5300_Firmware.h"
#include "fm33lc0xx_fl_flash.h"
#include "fm33lc0xx_fl_dma.h"

#include "menu.h"


VI5300_Dist_TypeDef result1;
uint16_t millier = 0;    
uint16_t millier1;
uint32_t confidence;
uint32_t peak;

uint8_t m_cnt = 0;    
//uint16_t m_ms = 0;    

uint8_t n = 0;  
//	uint8_t LED2_flag = 0;  


uint16_t  dis_buf[15] = {0};	
float  d_k[14] = {0};		
float  d_b[14] = {0};		
uint16_t  Calib_DisBuf[40]; 
uint8_t   Calib_Cnt;
uint8_t   sendflag = 1;
uint8_t  aRxBuffer[20];
uint32_t sum_data;
uint8_t datacnt=0;
uint8_t caclflag=0;
	



void FoutInit(void)
{
    FL_GPIO_InitTypeDef init = {0};
    
    init.pin = FL_GPIO_PIN_11;
    init.mode = FL_GPIO_MODE_DIGITAL;
    init.outputType = FL_GPIO_OUTPUT_PUSHPULL;
    init.pull = FL_DISABLE;
    FL_GPIO_Init(GPIOD, &init);
    
    FL_GPIO_SetFOUT0(GPIO, FL_GPIO_FOUT0_SELECT_AHBCLK_DIV64);
}


#ifdef __CC_ARM
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE
{ 
    int handle; 
}; 
FILE __stdout;
#endif

//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
    x = x; 
} 

//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
    FL_UART_WriteTXBuff(UART0, (uint8_t)ch);
    while(FL_UART_IsActiveFlag_TXBuffEmpty(UART0) != FL_SET);
    return ch;
}

void UartInit(uint8_t baud)
{
    FL_UART_InitTypeDef UART_InitStruct = {0}; 
	uint32_t u32BaudRate;

	if(baud == 2)
    {
		u32BaudRate = 9600;
	}						
	else if(baud == 3)
    {
		u32BaudRate = 19200;
	}		
    else if(baud == 5)
    {
		u32BaudRate = 115200;
	}
    else
	{
		u32BaudRate = 115200;
	}			
    UART_InitStruct.clockSrc = FL_RCC_UART0_CLK_SOURCE_APB1CLK;
    UART_InitStruct.baudRate            = u32BaudRate;							        //波特率
    UART_InitStruct.dataWidth           = FL_UART_DATA_WIDTH_8B;			      	//数据位数
    UART_InitStruct.stopBits            = FL_UART_STOP_BIT_WIDTH_1B;				//停止位
    UART_InitStruct.parity              = FL_UART_PARITY_NONE;				        //奇偶校验
    UART_InitStruct.transferDirection   = FL_UART_DIRECTION_TX_RX;	                //接收-发送使能
    FL_UART_Init(UART0, &UART_InitStruct);	
}

void DebugUartInit(void)
{
	FL_GPIO_InitTypeDef GPIO_InitStruct = {0};
	//uint32_t u32BaudRate;

	//PA13:UART0-RX   PA14:UART0-TX
	GPIO_InitStruct.pin        = FL_GPIO_PIN_13|FL_GPIO_PIN_14;
	GPIO_InitStruct.mode       = FL_GPIO_MODE_DIGITAL;
	GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;   
	GPIO_InitStruct.pull       = FL_ENABLE;
	GPIO_InitStruct.remapPin = FL_DISABLE;
	FL_GPIO_Init(GPIOA, &GPIO_InitStruct);	
	UartInit(Baud485);
}

void Uart4Init(void)
{
	FL_GPIO_InitTypeDef GPIO_InitStruct = {0};
	FL_UART_InitTypeDef UART_InitStruct = {0}; 

	//PB2:UART0-RX   PB3:UART0-TX
	GPIO_InitStruct.pin        = FL_GPIO_PIN_2|FL_GPIO_PIN_3;
	GPIO_InitStruct.mode       = FL_GPIO_MODE_DIGITAL;
	GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;   
	GPIO_InitStruct.pull       = FL_ENABLE;
	GPIO_InitStruct.remapPin = FL_DISABLE;
	FL_GPIO_Init(GPIOB, &GPIO_InitStruct);	
	  
	UART_InitStruct.clockSrc = FL_RCC_UART0_CLK_SOURCE_APB1CLK;
	UART_InitStruct.baudRate            = 403200;							        //波特率
	UART_InitStruct.dataWidth           = FL_UART_DATA_WIDTH_8B;			      	//数据位数
	UART_InitStruct.stopBits            = FL_UART_STOP_BIT_WIDTH_1B;				//停止位
	UART_InitStruct.parity              = FL_UART_PARITY_NONE;				        //奇偶校验
	UART_InitStruct.transferDirection   = FL_UART_DIRECTION_TX_RX;	                //接收-发送使能
	FL_UART_Init(UART4, &UART_InitStruct);	
}

/**
  * @brief  DMA_Channel2 Initialization function
  * @param  void
  * @retval None
  */
void MF_DMA_Channel2_Init(void)
{
    /*IO CONFIG*/
    FL_DMA_InitTypeDef    defaultInitStruct;

    defaultInitStruct.periphAddress = FL_DMA_PERIPHERAL_FUNCTION2;
    defaultInitStruct.direction = FL_DMA_DIR_RAM_TO_PERIPHERAL;
    defaultInitStruct.memoryAddressIncMode = FL_DMA_MEMORY_INC_MODE_INCREASE;
    defaultInitStruct.flashAddressIncMode = FL_DMA_CH7_FLASH_INC_MODE_INCREASE;
    defaultInitStruct.dataSize = FL_DMA_BANDWIDTH_8B;
    defaultInitStruct.priority = FL_DMA_PRIORITY_HIGH;
    defaultInitStruct.circMode = FL_DISABLE;

    FL_DMA_Init(DMA,&defaultInitStruct,FL_DMA_CHANNEL_2 );   
}

void MF_DMA_Channel3_Init(void)
{

    /*IO CONFIG*/
    FL_DMA_InitTypeDef    defaultInitStruct;

    defaultInitStruct.periphAddress = FL_DMA_PERIPHERAL_FUNCTION4;
    defaultInitStruct.direction = FL_DMA_DIR_RAM_TO_PERIPHERAL;
    defaultInitStruct.memoryAddressIncMode = FL_DMA_MEMORY_INC_MODE_INCREASE;
    defaultInitStruct.flashAddressIncMode = FL_DMA_CH7_FLASH_INC_MODE_INCREASE;
    defaultInitStruct.dataSize = FL_DMA_BANDWIDTH_8B;
    defaultInitStruct.priority = FL_DMA_PRIORITY_HIGH;
    defaultInitStruct.circMode = FL_DISABLE;

    FL_DMA_Init(DMA,&defaultInitStruct,FL_DMA_CHANNEL_3 );   
}


// GPIO INIT

void GPIO_Init(void)
{

	FL_GPIO_InitTypeDef GPIO_InitStruct = {0};

	Laser_indicator_On();
	Dout_Select_NPN();
	Dout_Switch_Off();
	IO_Led_Off();

	//output
	GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
	GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.pull = FL_DISABLE;
	
	GPIO_InitStruct.pin = Laser_indicator;
	FL_GPIO_Init(Laser_indicator_Port, &GPIO_InitStruct);	

	GPIO_InitStruct.pin = Dout_Select;
	FL_GPIO_Init(Dout_Select_Port, &GPIO_InitStruct);	

	GPIO_InitStruct.pin = Dout_Switch;
	FL_GPIO_Init(Dout_Switch_Port, &GPIO_InitStruct);	

	GPIO_InitStruct.pin = IO_Led;
	FL_GPIO_Init(IO_Led_Port, &GPIO_InitStruct);	

	GPIO_InitStruct.pin = VL53L1X_XSHUT_Pin;
	FL_GPIO_Init(VL53L1X_XSHUT_GPIO_Port, &GPIO_InitStruct);


	//input
	GPIO_InitStruct.pin = VL53L1X_INT_Pin;
	GPIO_InitStruct.mode = FL_GPIO_MODE_INPUT;
	GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_OPENDRAIN;
	GPIO_InitStruct.pull = FL_DISABLE;
	FL_GPIO_Init(VL53L1X_INT_GPIO_Port, &GPIO_InitStruct);

	GPIO_InitStruct.pin = Key_Up;
	FL_GPIO_Init(Key_Up_Port, &GPIO_InitStruct);

	GPIO_InitStruct.pin = Key_Down;
	FL_GPIO_Init(Key_Down_Port, &GPIO_InitStruct);

	GPIO_InitStruct.pin = Key_Teach;
	FL_GPIO_Init(Key_Teach_Port, &GPIO_InitStruct);

	GPIO_InitStruct.pin = IO_Input;
	FL_GPIO_Init(IO_Input_Port, &GPIO_InitStruct);

}


void UserInit(void)
{
	DebugUartInit();
	Uart4Init();
}


// VI5300,init
	
void ZSQ_VI5300_Init(void)
{
	VI5300_Chip_PowerON1();
	if(VI5300_Device_Check()!=0xD8)
	{
	  //printf("Check device ID fail!\r\n");	
	}
	VI5300_init();			
	//#ifdef VI5300_XTALK_CALIBRATION         
	//VI5300_Start_XTalk_Calibration();
	//#endif		
	//#ifdef VI5300_OFFSET_CALIBRATION
	//VI5300_Start_Offset_Calibration();
	//#endif
	VI5300_Interrupt_Enable();
	//VI5300_Start_Continuous_Measure(); 
	//printf("init OK!");

}

void VI5300_Init(void)
{
	i2c_sw_gpio_config();
	ZSQ_VI5300_Init();		
}

#define N 5

uint16_t Dis_Data_cacl(uint16_t dis)
{
	uint8_t i;
	sum_data = 0;
	dis_buf[datacnt++] = dis;
	if(datacnt == N)
	{
		datacnt = 0;
	}
	for(i=0;i<=N;i++)
	{
		sum_data += dis_buf[i];
	}
	return (uint16_t)(sum_data/N);
}		    


uint8_t Checkfor_n(uint16_t dis)
{
	uint8_t i,m;			
	if(dis <= Calib_DisBuf[0])
	{
		m = 0;
	}
	else if(dis >= Calib_DisBuf[13])
	{
		m = 14;
	}
	else
	{
		for(i=0;i<13;i++)
		{
			if(dis > Calib_DisBuf[i] && dis <= Calib_DisBuf[i+1])
			{
				m = i+1;
			}
		}
	}
	return m;
}
		
void Calib_kb(void)
{
	uint8_t i;
	float k,b,m;  
	for(i=0;i<13;i++)
	{
		if(i<5)
		{
			m = 100.0f;
			k = m/(Calib_DisBuf[i+1] - Calib_DisBuf[i]);// (y2-y1)/(x2-x1)
			b = (i+1)*100 - k*Calib_DisBuf[i+1];				
			d_k[i] = k;
			d_b[i] = b;							
		}
		else if(i<13)
		{
			m = 500.0f;
			k = m/(Calib_DisBuf[i+1] - Calib_DisBuf[i]);// (y2-y1)/(x2-x1)
			b = (i*500-1500) - k*Calib_DisBuf[i+1];				
			d_k[i] = k;
			d_b[i] = b;			
		}
	}
}


uint16_t Calib_DisData(uint16_t dis)
{
	uint16_t  dis_mm;
	float d;  
	double y;
	Calib_Cnt = Checkfor_n(dis);
	if(Calib_Cnt == 0)
	{
		dis_mm = 0;
	}
	else if(Calib_Cnt == 14)//µÚ4Çø¼ä
	{
		//dis_mm = 4500;
		//dis_mm = 4501;		//gp20220420
		//dis_mm = 4505;		//gp20220707
		dis_mm = 65535;		//gp20220715
	}
	else
	{
		d = (float)(dis);
		y = d_k[Calib_Cnt-1]*d;
		y = y + d_b[Calib_Cnt-1];
		dis_mm = (uint16_t)(y);
	}
	return dis_mm;
}

		
		
// 原始距离，需进行有效值判定
		
/*
uint16_t GetDisTance(void)
{
	//VI5300_Status ret;

	if(VI5300_Get_Measure_Data(&result1) != VI5300_OK)
	{
		millier = 65535;
		return  millier;
	}
	//VI5300_Get_Measure_Data(&result1);
	if(result1.confidence > 60)//有效值
	//if(ret==VI5300_OK)
	{
		millier = result1.millimeter;
		m_cnt = 0;
	}
	else
	{
		if(m_cnt < 3)
		{
			m_cnt++;
			millier = millier;//小于3次，返回上一次值
		}
		else//连续3次无法获取有效距离
		{
			if(caclflag == 1)
			{
				//millier = Calib_DisBuf[13];
				millier = 65535;//返回最大值
			}
			else
			{
				millier = 65535;//返回最大值
			}
		}
	}
	return  millier;
}
*/

/*
uint16_t GetDisTance(void)
{
	VI5300_Get_Measure_Data(&result1);
	if(millier1 == result1.millimeter && confidence == result1.confidence && peak == result1.peak)
	{
		result1.millimeter = 0xffff;
		result1.confidence = 0;
		result1.peak = 0;
	}
	else
	{
		millier1 = result1.millimeter;
		confidence = result1.confidence;
		peak = result1.peak;
	}

	if(result1.confidence > 60)//有效值
	{
		millier = result1.millimeter;
		m_cnt = 0;
	}
	else
	{
		if(m_cnt < 1)
		{
			m_cnt++;
			millier = millier;
		}
		else
		{
			if(caclflag == 1)
			{
				//millier = Calib_DisBuf[13];
				millier = 65535;//返回最大值
			}
			else
			{
				millier = 65535;//返回最大值
			}
		}
	}
	return  millier;
}
*/
		
uint16_t GetDisTance(void)
{
	VI5300_Get_Measure_Data(&result1);

	/*
	if(millier1 == result1.millimeter && confidence == result1.confidence && peak == result1.peak)
	{
		result1.millimeter = 0xffff;
		result1.confidence = 0;
		result1.peak = 0;
	}
	else
	{
		millier1 = result1.millimeter;
		confidence = result1.confidence;
		peak = result1.peak;
	}
	*/

	if(result1.confidence > 70)//有效值
	{
		millier = result1.millimeter;
		if(millier > 0x7fff)
		{
			millier = 0xffff;
		}
		//m_cnt = 0;
	}
	else
	{
		//if(m_cnt < 1)
		//{
		//		m_cnt++;
		//	  millier = millier;//小于3次，返回上一次值
		//}
		//else//连续3次无法获取有效距离
		//{
		millier = 0xffff;//返回最大值				
		//} 
	}
	return  millier;
}		

// 返回最终校准距离
		
uint16_t Vi5300_GetDisTance(void)
{
	uint16_t distance;
	distance = GetDisTance();
	//校准完成标志位
	if(caclflag == 1)
	{
		distance = Calib_DisData(distance);
	}
	return 	distance;
}


//等待5300一次采集完成
void Waitfor_Vi5300_Getok(void)
{
	while(FL_GPIO_GetInputPin(VL53L1X_INT_GPIO_Port,VL53L1X_INT_Pin) == 1)
	{
	//Uart3_Rcv_Handle();
	}
}

void ATIM_Start(void)
{
    //使能LATIM
    FL_ATIM_Enable(ATIM);
}


void ATIM_IRQHandler(void)
{
	static uint8_t laser_count = 0;
			
    if(FL_ATIM_IsEnabledIT_Update(ATIM) && FL_ATIM_IsActiveFlag_Update(ATIM))
    {
        FL_ATIM_ClearFlag_Update(ATIM);
		if(n != 0)
		{
			n--;
		}
				
		if(laser_count == 0)
		{
			if(Input_Function == Input_Laser_Ctrl)
			{
				if(Read_IO_Input() == 0)
				{
					Laser_indicator_On();
				}
			}
			else
			{
				Laser_indicator_On();
			}
		}
		else
		{
			Laser_indicator_Off();
		}

		laser_count++;
		if(laser_count > 5)
		{
			laser_count = 0;
		}
				
		/*m_ms++;

		if(m_ms > 500)
		{
		   m_ms = 0;
		}
		else if(m_ms > 250)
		{
		  FL_GPIO_ResetOutputPin(LED2_GPIO_Port, LED2_Pin);
		}
		else if(LED2_flag == 1)
		{
		  FL_GPIO_SetOutputPin(LED2_GPIO_Port, LED2_Pin);
		}
		*/
        //处理更新事件
        //...
    }
}


