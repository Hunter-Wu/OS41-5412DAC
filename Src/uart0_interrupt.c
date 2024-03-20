#include "main.h"
#include "user_init.h"
#include "uart0_interrupt.h"
#include "fm33lc0xx_fl_flash.h"
#include "fm33lc0xx_fl_dma.h"

#include "menu.h"
#include "VI5300_API.h"


#define FLASH_PROG_ADDR1         0x0000e000
#define FLASH_PROG_ADDR2         0x0000f000
#define FLASH_PAGE_SIZE         256
#define BUFFER_SIZE     		20
uint32_t DataBuffer1[BUFFER_SIZE];
uint32_t DataBuffer2[BUFFER_SIZE];
uint32_t DataBuffer[BUFFER_SIZE];


#define Uart3_RXWAIT 1
#define Uart3_RXHEAD 2
#define Uart3_RXFinish 3
#define Uart3_RX_JZ 4

#define  Mode_485_Series 0
#define  Mode_485_ASK   1

//#define Baud_9600 2
//#define Baud_115200 5

static uint8_t temp_data;

uint8_t Adr485 = 1;//地址
uint8_t Baud485 = 5;//波特率
uint8_t Mode485 = Mode_485_ASK;//485模式

uint8_t JZFlag = 0;//地址

uint8_t JiaoZhun_ModeFlag = 0;


static uint8_t Uart3_Tx_Buf[256];

static uint8_t Uart3_Rcv_Count = 0;
static uint8_t Uart3_Rx_Buf[256];
volatile uint8_t Uart3_Rx_State = Uart3_RXWAIT;

static uint8_t Uart4_Tx_Buf[8];

VI5300_OFFSET_Calib_Data stRefTof;


struct UARTOpStruct
{
    uint8_t *TxBuf; //发送数据指针
    uint8_t TxLen;  //待发送数据长度
    uint8_t TxOpc;  //已发送数据长度
    uint8_t *RxBuf; //接收数据指针
    uint8_t RxLen;  //待接收数据长度
    uint8_t RxOpc;  //已接收数据长度
};
struct UARTOpStruct UARTxOp;

uint8_t RxData[11];
uint8_t RxDataCnt;



void Uart0_485_Enable(void)
{
    //FL_GPIO_SetOutputPin(MCU_485_EN_Port, MCU_485_EN);
}

void Uart0_485_Disable(void)
{
    //FL_GPIO_ResetOutputPin(MCU_485_EN_Port, MCU_485_EN);
}

/*
校准数据存储
*/
void Flash_CaclDate_Write(void)
{
	uint8_t i;
	FL_FLASH_PageErase(FLASH, FLASH_PROG_ADDR1);
	//memset(Calib_DisBuf, 0x55, BUFFER_SIZE * 4);
	//FL_FLASH_Program_Word(FLASH, FLASH_PROG_ADDR1, Calib_DisBuf[0]);

	//for(i=0;i<16;i++)
	for(i=0;i<19;i++)
	{
		FL_FLASH_Program_Word(FLASH, FLASH_PROG_ADDR1+i*4, Calib_DisBuf[i]);
	}
	//FL_FLASH_Write_Dma(FLASH,FLASH_PROG_ADDR1,DataBuffer2);
	
}

/*
指令数据存储
*/
void Flash_CmdDate_Write(void)
{
	uint8_t i;
	FL_FLASH_PageErase(FLASH, FLASH_PROG_ADDR2);
	for(i=0;i<Flash_para_num;i++)
	{
		FL_FLASH_Program_Word(FLASH, FLASH_PROG_ADDR2+i*4, DataBuffer2[i]);
	}
	//memset(DataBuffer1, 0x55, BUFFER_SIZE * 4);
	//FL_FLASH_Program_Word(FLASH, FLASH_PROG_ADDR2, DataBuffer2[0]);
	//FL_FLASH_Write_Dma(FLASH,FLASH_PROG_ADDR2,DataBuffer2);
}

/*
校准数据读取
*/
void Flash_CaclDate_Read(void)
{	  
	uint8_t i;

	FL_FLASH_Read_Dma(FLASH,FLASH_PROG_ADDR1,DataBuffer1,BUFFER_SIZE);
	//for(i=0;i<16;i++)
	for(i=0;i<19;i++)
	{
		Calib_DisBuf[i] = DataBuffer1[i];
	}
	caclflag = DataBuffer1[18];
	if(caclflag > 1)
	{
		caclflag = 0;
	}
	else if(caclflag == 1)
	{
		 Calib_kb();
	}
}

/*
指令数据读取
*/
void Flash_CmdDate_Read(void)
{
	FL_FLASH_Read_Dma(FLASH,FLASH_PROG_ADDR2,DataBuffer2,BUFFER_SIZE);

	DataBuffer2[0] = 0;
	Aout_Direction = DataBuffer2[1];	//模拟输出方向
	if(Aout_Direction > 1)
	{
		Aout_Direction = Aout_Forward;		//默认为正向
		DataBuffer2[1] = Aout_Direction;
	}
	Aout_Scope = DataBuffer2[2];	//模拟输出范围
	if(Aout_Scope > 1)
	{
		Aout_Scope = Aout_Full;		//默认为全量程
		DataBuffer2[2] = Aout_Scope;
	}

	Teach_Mode = DataBuffer2[3];	//教导模式
	if(Teach_Mode > 2)
	{
		Teach_Mode = Teach_Mode_Normal;	//默认为单点教导模式
		DataBuffer2[3] = Teach_Mode;
	}
	Teach_Point[0] = DataBuffer2[4];	//教导点1
	if(Teach_Point[0] > Distance_Teach)
	{
		Teach_Point[0] = Distance_Teach;	//默认为最远点
		DataBuffer2[4] = Teach_Point[0];
	}
	Teach_Point[1] = DataBuffer2[5];	//教导点2
	if(Teach_Point[1] > Distance_Teach)
	{
		Teach_Point[1] = Distance_Teach;	//默认为最远点
		DataBuffer2[5] = Teach_Point[1];
	}
	IO_Out_Speed = DataBuffer2[6];	//响应速度
	if(IO_Out_Speed > 1)
	{
		IO_Out_Speed = IO_Out_Speed_Standard;	//默认为标准响应速度
		DataBuffer2[6] = IO_Out_Speed;
	}
	IO_Out_Reverse = DataBuffer2[7];	//亮暗通模式
	if(IO_Out_Reverse > 1)
	{
		IO_Out_Reverse = IO_Out_Light;	//默认为亮通模式
		DataBuffer2[7] = IO_Out_Reverse;
	}
	IO_Out_Type = DataBuffer2[8];	//IO输出模式
	if(IO_Out_Type > 2)
	{
		IO_Out_Type = IO_Out_NPN;	//默认为NPN输出模式
		DataBuffer2[8] = IO_Out_Type;
	}

	if(DataBuffer2[9] != 0xffffffff)
	{
		Aout_Adjust1 = DataBuffer2[9];
	}
	else
	{
		DataBuffer2[9] = Aout_Adjust1;
	}

	if(DataBuffer2[10] != 0xffffffff)
	{
		Aout_Adjust2 = DataBuffer2[10];
	}
	else
	{
		DataBuffer2[10] = Aout_Adjust2;
	}

	if((DataBuffer2[11] != 0xffffffff) && (DataBuffer2[11] != 0))
	{
		stRefTof.RefTof = DataBuffer2[11];
	}
	else
	{
		VI5300_Start_RefTof_Calibration(&stRefTof);
		DataBuffer2[11] = stRefTof.RefTof;
		Flash_CmdDate_Write();
	}

	Input_Function = DataBuffer2[12];	//输入功能
	if(Input_Function > 1)
	{
		Input_Function = Input_Off;	//默认输入功能关闭
		DataBuffer2[12] = Input_Function;
	}
}



//串口DMA发送

void Uart0DMA_Config(uint8_t *buffer, uint32_t length)
{
    FL_DMA_ConfigTypeDef DMA_ConfigStruct = {0};

    DMA_ConfigStruct.memoryAddress = (uint32_t)buffer;
    DMA_ConfigStruct.transmissionCount = length - 1;
    FL_DMA_StartTransmission(DMA, &DMA_ConfigStruct, FL_DMA_CHANNEL_2);
}

void Uart4DMA_Config(uint8_t *buffer, uint32_t length)
{
    FL_DMA_ConfigTypeDef DMA_ConfigStruct = {0};

    DMA_ConfigStruct.memoryAddress = (uint32_t)buffer;
    DMA_ConfigStruct.transmissionCount = length - 1;
    FL_DMA_StartTransmission(DMA, &DMA_ConfigStruct, FL_DMA_CHANNEL_3);
}


//串口中断接收

void UART0_IRQHandler(void)
{
    //接收中断处理
    if((FL_ENABLE == FL_UART_IsEnabledIT_RXBuffFull(UART0))
            && (FL_SET == FL_UART_IsActiveFlag_RXBuffFull(UART0)))
    {
        //中断转发接收到的数据
        temp_data = FL_UART_ReadRXBuff(UART0);//接收中断标志可通过读取rxreg寄存器清除
        if(Uart3_Rx_State == Uart3_RXWAIT)
        {
            Uart3_Rx_Buf[0] = temp_data;
            if(Uart3_Rx_Buf[0] == 0x68)
            { 
                Uart3_Rx_State = Uart3_RXHEAD;
                Uart3_Rcv_Count = 1;
            }
        }
        else if(Uart3_Rx_State == Uart3_RXHEAD)
        {
			Uart3_Rx_Buf[Uart3_Rcv_Count++] = temp_data;
			if((Uart3_Rcv_Count == (Uart3_Rx_Buf[2] + 4)) && (Uart3_Rcv_Count > 2))
			{
				if((Uart3_Rx_Buf[Uart3_Rcv_Count-1] == 0x16) && (Uart3_Rcv_Count > 6))
				{
					Uart3_Rx_State = Uart3_RXFinish;
				}
				else
				{
					Uart3_Rx_State = Uart3_RXWAIT;
				}
			}					
        }

    }

    //发送中断处理
    if((FL_ENABLE == FL_UART_IsEnabledIT_TXShiftBuffEmpty(UART0))
            && (FL_SET == FL_UART_IsActiveFlag_TXShiftBuffEmpty(UART0)))
    {
        //发送中断标志可通过写txreg寄存器清除或txif写1清除
        //发送指定长度的数据
        if(UARTxOp.TxOpc < UARTxOp.TxLen)
        {
            FL_UART_WriteTXBuff(UART0, UARTxOp.TxBuf[UARTxOp.TxOpc]); //发送一个数据
            UARTxOp.TxOpc++;
        }

        FL_UART_ClearFlag_TXShiftBuffEmpty(UART0);  //清除发送中断标志
    }
}


void Test_Uart0(void)
{
	uint8_t TestTxData[11] = "TestUart-TX";

	//中断发送数组
	UARTxOp.TxBuf = TestTxData;
	UARTxOp.TxLen = 11;
	UARTxOp.TxOpc = 0 + 1;

	//FL_DMA_Enable(DMA);

	Uart0DMA_Config(TestTxData, 11);

	FL_UART_EnableTX(UART0);

	while(!FL_DMA_IsActiveFlag_TransferComplete(DMA, FL_DMA_CHANNEL_2));

	FL_DMA_ClearFlag_TransferComplete(DMA, FL_DMA_CHANNEL_2);
	FL_UART_ClearFlag_TXShiftBuffEmpty(UART0);

	// 等待最后一字节数据发送完成
	while(FL_UART_IsActiveFlag_TXShiftBuffEmpty(UART0) == FL_RESET) {};

	FL_UART_ClearFlag_TXShiftBuffEmpty(UART0);

	//FL_UART_ClearFlag_TXShiftBuffEmpty(UART0);
	//FL_UART_EnableIT_TXShiftBuffEmpty(UART0);
	//FL_UART_WriteTXBuff(UART0, UARTxOp.TxBuf[0]);
	//FL_DelayMs(50);//软件延时
	FL_UART_DisableIT_TXShiftBuffEmpty(UART0);
	FL_UART_EnableIT_RXBuffFull(UART0);
}
/*
//连续发送数据
void Uart_Send_Always(void)
{
	uint16_t sum;
	Uart0_485_Enable();
	FL_DelayUs(10);
	Uart3_Tx_Buf[0] = 0x68;
	Uart3_Tx_Buf[1] = Adr485;
	Uart3_Tx_Buf[2] = 0x05;
	Uart3_Tx_Buf[3] = 0x00;
	Uart3_Tx_Buf[5] = Vi5300_distance >> 8;
	Uart3_Tx_Buf[4] = Vi5300_distance;
	sum = 	Uart3_Tx_Buf[1]+Uart3_Tx_Buf[2]+Uart3_Tx_Buf[3]+Uart3_Tx_Buf[4]+Uart3_Tx_Buf[5];
	Uart3_Tx_Buf[7] = sum >> 8;
	Uart3_Tx_Buf[6] = sum;
	Uart3_Tx_Buf[8] = 0x16;
	Uart0DMA_Config(Uart3_Tx_Buf, 9);
	FL_DelayUs(10000);
	Uart0_485_Disable();
}
*/

//连续发送数据
void Uart_Send_CMD(uint8_t cmd,uint16_t data)
{
	//uint8_t i;
	uint16_t sum;
	Uart0_485_Enable();
	FL_DelayUs(10);
	Uart3_Tx_Buf[0] = 0x68;
	Uart3_Tx_Buf[1] = Adr485;
	Uart3_Tx_Buf[2] = 0x05;
	Uart3_Tx_Buf[3] = cmd;
	Uart3_Tx_Buf[5] = data >> 8;
	Uart3_Tx_Buf[4] = data;
	sum = 	Uart3_Tx_Buf[1]+Uart3_Tx_Buf[2]+Uart3_Tx_Buf[3]+Uart3_Tx_Buf[4]+Uart3_Tx_Buf[5];
	Uart3_Tx_Buf[7] = sum >> 8;
	Uart3_Tx_Buf[6] = sum;
	Uart3_Tx_Buf[8] = 0x16;
	Uart0DMA_Config(Uart3_Tx_Buf, 9);
	/*for(i = 0; i < 9; i++)
	{
		FL_UART_WriteTXBuff(UART0, Uart3_Tx_Buf[i]);      //将发送数据写入发送寄存器
		while(FL_SET != FL_UART_IsActiveFlag_TXBuffEmpty(UART0));       //等待发送完成
	}*/
	if(Baud485 == 2)//9600,延时15ms
	{
		FL_DelayMs(30);
	}
	else if(Baud485 == 3)//19200,延时8ms
	{
		FL_DelayMs(15);
	}	    
	else if(Baud485 == 5)//115200,延时3ms
	{
		FL_DelayMs(3);
	}
	//FL_DelayMs(20);
	Uart0_485_Disable();
}

void Uart_Send_Distance(void)
{
	if((Vi5300_distance <= Distance_Scope) || JiaoZhun_ModeFlag)
	{
		Uart_Send_CMD(0x00,Vi5300_distance);
	}
	else
	{
		Uart_Send_CMD(0x00,0xffff);
	}
}


void Uart_Send_CMD_Byte(uint8_t cmd,uint8_t data)
	{
	//uint8_t i;
	uint16_t sum;
	Uart0_485_Enable();
	FL_DelayUs(10);
	Uart3_Tx_Buf[0] = 0x68;
	Uart3_Tx_Buf[1] = Adr485;
	Uart3_Tx_Buf[2] = 0x04;
	Uart3_Tx_Buf[3] = cmd;
	Uart3_Tx_Buf[4] = data;
	sum = 	Uart3_Tx_Buf[1]+Uart3_Tx_Buf[2]+Uart3_Tx_Buf[3]+Uart3_Tx_Buf[4];
	Uart3_Tx_Buf[5] = sum;
	Uart3_Tx_Buf[6] = sum >> 8;
	Uart3_Tx_Buf[7] = 0x16;
	Uart0DMA_Config(Uart3_Tx_Buf, 8);
	/*for(i = 0; i < 9; i++)
	{
	FL_UART_WriteTXBuff(UART0, Uart3_Tx_Buf[i]);      //将发送数据写入发送寄存器
	while(FL_SET != FL_UART_IsActiveFlag_TXBuffEmpty(UART0));       //等待发送完成
	}*/
	if(Baud485 == 2)//9600,延时15ms
	{
		FL_DelayMs(30);
	}
	else if(Baud485 == 3)//19200,延时8ms
	{
		FL_DelayMs(15);
	}	    
	else if(Baud485 == 5)//115200,延时3ms
	{
		FL_DelayMs(3);
	}
	//FL_DelayMs(20);
	Uart0_485_Disable();
}

void Uart_Send_Display_Buf(void)
{
		
    Uart4_Tx_Buf[0] = 0x68;
    Uart4_Tx_Buf[1] = Menu_Display_Buf[0];
    Uart4_Tx_Buf[2] = Menu_Display_Buf[1];
    Uart4_Tx_Buf[3] = Menu_Display_Buf[2];
    Uart4_Tx_Buf[4] = Menu_Display_Buf[3];
    
    Uart4_Tx_Buf[5] = Uart4_Tx_Buf[1];
    Uart4_Tx_Buf[5] += Uart4_Tx_Buf[2];
    Uart4_Tx_Buf[5] += Uart4_Tx_Buf[3];
    Uart4_Tx_Buf[5] += Uart4_Tx_Buf[4];
    
    Uart4_Tx_Buf[6] = 0x16;
    
    Uart4DMA_Config(Uart4_Tx_Buf, 7);
    
}

/*校准数据核验*/
void Uart_Send_CaclData(void)
	{
	uint8_t i;
	uint16_t sum;
	Uart0_485_Enable();
	FL_DelayUs(10);
	Uart3_Tx_Buf[0] = 0x68;
	Uart3_Tx_Buf[1] = Adr485;
	Uart3_Tx_Buf[2] = 0x27;
	Uart3_Tx_Buf[3] = 0x92;
	sum = Uart3_Tx_Buf[1]+Uart3_Tx_Buf[2]+Uart3_Tx_Buf[3];
	for(i=0;i<18;i++)//28个校准数据
	{
		Uart3_Tx_Buf[i*2+5] = Calib_DisBuf[i]>>8;
		Uart3_Tx_Buf[i*2+4] = Calib_DisBuf[i];
		sum += Uart3_Tx_Buf[i*2+4];
		sum += Uart3_Tx_Buf[i*2+5];
	}
	Uart3_Tx_Buf[40] = sum;
	Uart3_Tx_Buf[41] = sum >> 8;
	Uart3_Tx_Buf[42] = 0x16;
	Uart0DMA_Config(Uart3_Tx_Buf, 43);
	if(Baud485 == 2)//9600,延时15ms
	{
		FL_DelayMs(200);
	}
	else if(Baud485 == 3)//19200,延时8ms
	{
		FL_DelayMs(100);
	}	    
	else if(Baud485 == 5)//115200,延时3ms
	{
		FL_DelayMs(20);
	}
	//FL_DelayMs(20);
	Uart0_485_Disable();
}

//命令指令解析
void Uart3_Cmd(void)
{
	uint8_t i, flag;
	uint16_t tempW, tempW2;
	static uint16_t adj_count = 0;

	switch(Uart3_Rx_Buf[3])
	{
		case 0x00://距离查询
			if(Uart3_Rx_Buf[2] == 3)
			{
				Uart_Send_Distance();
			}
			break;
				
		case 0x80://地址设置
			if(Uart3_Rx_Buf[2] == 4)
			{
				Adr485 = Uart3_Rx_Buf[4];
				//DataBuffer2[0] = Adr485;
				//Flash_CmdDate_Write();
				Uart_Send_CMD_Byte(0x80,0);
			}
			break;
		
		case 0x81://波特率设置
			if(Uart3_Rx_Buf[2] == 4)
			{
				Uart_Send_CMD_Byte(0x81,0);
				Baud485 = Uart3_Rx_Buf[4];//05 115200; 02 9600
				//DataBuffer2[1] = Baud485;
				//Flash_CmdDate_Write();
				
				FL_DelayMs(100);//延时100ms
				//RCC->SOFTRST = 0x5C5CAABB; //软件复位
				UartInit(Baud485);
				//FL_DelayMs(100);//延时100ms
				//Uart_Send_CMD_Byte(0x81,0);
				FL_DelayMs(100);//延时100ms
			}
			break;			
		case 0x83://设置模式
			if(Uart3_Rx_Buf[2] == 4)
			{
				if(Uart3_Rx_Buf[4] == Mode_485_Series)//连续发送模式
				{
					Mode485 = Mode_485_Series;									
				}
				else if(Uart3_Rx_Buf[4] == Mode_485_ASK)
				{
					Mode485 = Mode_485_ASK;
				}
				//DataBuffer2[2] = Mode485;
				//Flash_CmdDate_Write();
				Uart_Send_CMD_Byte(0x83,0);
			}
			break;
		case 0x90://设置校准模式
			if(Uart3_Rx_Buf[2] == 4)
			{
				JiaoZhun_ModeFlag = Uart3_Rx_Buf[4];
				Uart_Send_CMD_Byte(0x90,0);
				if(JiaoZhun_ModeFlag == 1)//校准模式开启，停止主动上传
				{
					Mode485 = Mode_485_ASK;
				}
			}
			break;
		case 0x91://校准数据下载
			if(Uart3_Rx_Buf[2] == 0x27 && JiaoZhun_ModeFlag == 1)
			{
				for(i=0;i<18;i++)//28个校准数据
				{
					//Calib_DisBuf[i] = Uart3_Rx_Buf[i+4];
					Calib_DisBuf[i] = ((uint16_t)Uart3_Rx_Buf[i*2+5]<<8)|Uart3_Rx_Buf[i*2+4];		
				}
				Uart_Send_CMD_Byte(0x90,0);
			}
			else
			{
				Uart_Send_CMD_Byte(0x90,1);//先开校准模式，否则校准失败
			}
			break;
		case 0x92://校准数据回传
			if(Uart3_Rx_Buf[2] == 4)
			{
				Uart_Send_CaclData();
			}
			break;						
		case 0x93://校准使能
			if(Uart3_Rx_Buf[2] == 4)
			{
				caclflag = Uart3_Rx_Buf[4];
				Calib_DisBuf[18] = caclflag;
				if(caclflag == 1)
				{
					Calib_kb();
				}
				Uart_Send_CMD_Byte(0x93,0);
				Mode485 = Mode_485_Series;
				JiaoZhun_ModeFlag = 0;
				Flash_CaclDate_Write();							
			}
			break;
		
		case 0x94://模拟量校准
			if(Uart3_Rx_Buf[2] == 4)
			{
				if(Uart3_Rx_Buf[4] == 0x00)	//关闭校准
				{
					Aout_Adj = 0;			
					Aout_Adjust1 = DataBuffer2[9];
					Aout_Adjust2 = DataBuffer2[10];
					Uart_Send_CMD_Byte(0x94,0);
				}
				else if(Uart3_Rx_Buf[4] == 0x01)	//开启校准
				{
					if(Aout_Adj == 0)
					{
						Aout_Adj = 1;
						adj_count = 0;
						Uart_Send_CMD_Byte(0x94,0);
					}
					else
					{
						Uart_Send_CMD_Byte(0x94,1);
					}
				}
				
				//Uart_Send_CMD_Byte(0x94,0);
			}
			break;
		
		case 0x95://模拟量数据回传
			if(Uart3_Rx_Buf[2] == 5)
			{
				flag = 0;
				tempW = ((uint16_t)Uart3_Rx_Buf[5]<<8)|Uart3_Rx_Buf[4];		
				if(Aout_Adj == 1)
				{
					if((tempW >= Adj1_Value) && (tempW <= Adj1_Max))
					{
						tempW2 = tempW - Adj1_Value;
						if(tempW2 > (Adj_Shake*2))
						{
							Aout_Adjust1 = (uint32_t)Aout_Adjust1 * Adj1_Value / tempW;
						}
						else if(tempW2 > Adj_Shake)
						{
							Aout_Adjust1--;
						}
						else
						{
							adj_count++;
						}
					}
					else if((tempW < Adj1_Value) && (tempW >= Adj1_Min))
					{
						tempW2 = Adj1_Value - tempW;
						if(tempW2 > (Adj_Shake*2))
						{
							Aout_Adjust1 = (uint32_t)Aout_Adjust1 * Adj1_Value / tempW;
						}
						else if(tempW2 > Adj_Shake)
						{
							Aout_Adjust1++;
						}
						else
						{
							adj_count++;
						}
					}
					
					if(adj_count > 5)
					{
						Aout_Adj = 2;
						adj_count = 0;
					}
				}
				else if(Aout_Adj == 2)
				{
					if((tempW >= Adj2_Value) && (tempW <= Adj2_Max))
					{
						tempW2 = tempW - Adj2_Value;
						if(tempW2 > (Adj_Shake*2))
						{
							Aout_Adjust2 = (uint32_t)Aout_Adjust2 * Adj2_Value / tempW;
						}
						else if(tempW2 > Adj_Shake)
						{
							Aout_Adjust2--;
						}
						else
						{
							adj_count++;
						}
					}
					else if((tempW <= Adj2_Value) && (tempW >= Adj2_Min))
					{
						tempW2 = Adj2_Value - tempW;
						if(tempW2 > (Adj_Shake*2))
						{
							Aout_Adjust2 = (uint32_t)Aout_Adjust2 * Adj2_Value / tempW;
						}
						else if(tempW2 > Adj_Shake)
						{
							Aout_Adjust2++;
						}
						else
						{
							adj_count++;
						}
					}

					if(adj_count > 5)
					{
						Aout_Adj = 0;
						adj_count = 0;
						DataBuffer2[9] = Aout_Adjust1;
						DataBuffer2[10] = Aout_Adjust2;
						flag = 1;
						//Flash_CmdDate_Write();
					}
				}
				
				if(Aout_Adj == 0)
				{
					Uart_Send_CMD_Byte(0x95,1);
					//Flash_CmdDate_Write();
				}
				else
				{
					Uart_Send_CMD_Byte(0x95,0);
				}
				if(flag)
				{
					Flash_CmdDate_Write();
				}
					
			}
			break;					
	}
}


uint8_t Uart_Sum_Check(void)
{
    uint8_t i;
    uint16_t wtemp, wtemp2;
    
    wtemp = 0;
    for(i=1;i<(Uart3_Rx_Buf[2]+1);i++)
    {
        wtemp += Uart3_Rx_Buf[i];
    }
    
    wtemp2 = Uart3_Rx_Buf[i+1];
    wtemp2 *= 256;
    wtemp2 += Uart3_Rx_Buf[i];
    
    if(wtemp == wtemp2)
    {
        return 1;
    }
    
    return 0;
}

//接收数据解析

void Uart3_Rcv_Handle(void)
{
    if(Uart3_Rx_State == Uart3_RXFinish)
    {
		if(Uart_Sum_Check())
		{
			if((Uart3_Rx_Buf[1] == 0xff) || (Uart3_Rx_Buf[1] == Adr485))//地址匹配
			{
				Uart3_Cmd();
			}
		}

		Uart3_Rx_State = Uart3_RXWAIT;
    }
}



//串口任务执行

void Uart_Task(void)
{
	  // 连续发送模式
    if(Mode485 == Mode_485_Series)
    {
        Uart_Send_Distance();		//发送数据
    }
		//查询模式
    Uart3_Rcv_Handle();
}


