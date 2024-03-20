#include "main.h"
#include "user_init.h"
#include "uart0_interrupt.h"
#include "pll.h"
#include "fm33lc0xx_fl_iwdt.h"
#include "menu.h"
#include "ad5412_spi.h"
#include "bsp_spi.h"
uint16_t Vi5300_distance = 0;    

 
FL_ErrorStatus IWDT_Init(void)
{
	FL_IWDT_InitTypeDef IWDT_InitStruct;
	/* 默认不使用窗口 */
	IWDT_InitStruct.iwdtWindows    = 0;
	/*最长溢出时间*/
	IWDT_InitStruct.overflowPeriod = FL_IWDT_PERIOD_4000MS;
	return FL_IWDT_Init(IWDT, &IWDT_InitStruct);
}

int main(void)
{
	uint32_t count_5300 = 0;
	MF_Clock_Init();
	//MF_SystemClock_Config();
	SelRCHFToPLL(FL_RCC_RCHF_FREQUENCY_8MHZ, 48-1);
	FL_Init();
	MF_Config_Init();		
	GPIO_Init();	//GPIO初始化
	gpio_spi_config();
	AD5412_Init();	
	VI5300_Init();//5300初始化
	Flash_CaclDate_Read();//读取存储数据
	Flash_CmdDate_Read();
	VI5300_Config_RefTof(stRefTof.RefTof);
	VI5300_Start_Continuous_Measure(); 
	UserInit();	//串口初始化
//	FL_DMA_Enable(DMA);
	Test_Uart0();	
	ATIM_Start();
	Distance_display_Buf_Init();
	Account_Aout_K_and_B();
	
	IWDT_Init();
	__enable_irq();	//打开全局中断使能
	
	while(1)
	{	
		FL_IWDT_ReloadCounter(IWDT);            //清系统看门狗	
		//n = 10;	//13.3ms
		n = 13;	//17.3ms
		//n = 20;	//26.7ms
		while(FL_GPIO_GetInputPin(VL53L1X_INT_GPIO_Port,VL53L1X_INT_Pin) == 1)
		{
			Uart3_Rcv_Handle();
			if(n == 0)
			{
				//LED2_flag = 1;
				VI5300_Init();
				VI5300_Config_RefTof(stRefTof.RefTof);
				VI5300_Start_Continuous_Measure(); 
				break;
			}
		}
		count_5300++;
		if(count_5300 > 155520000)
		{
			//Dout_Switch_Off();
			count_5300 = 0;
			VI5300_Init();
			VI5300_Config_RefTof(stRefTof.RefTof);
			VI5300_Start_Continuous_Measure(); 
			//Dout_Switch_On();
		}
		Vi5300_distance = Vi5300_GetDisTance();
		Get_Distance_avg();			
		Digital_Output();
		Analog_Output();
		//Aout_test();			
		//Digital_Input();			
		Menu_Key_Operate();
		Uart_Task();
		//LED2_flag = 0;
	}
}

