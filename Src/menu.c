
#include "user_init.h"
#include "uart0_interrupt.h"
#include "vi_sw_i2c.h"
#include "VI5300_API.h"
#include "VI5300_Config.h"
#include "VI5300_Firmware.h"
#include "fm33lc0xx_fl_flash.h"
#include "fm33lc0xx_fl_dma.h"
#include "menu.h"
#include "a_i2c.h"
#include "ad5412_spi.h"



const unsigned char TM16_num_buf[20]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x7,0x7F,0x6F,//0~9
	                            0x5c,0x7c,0x58,0x5e,0x79,0x71,//a,b,c,d,e,f	                       
	                            0x00,0x40,0x01};//"��"��"-","_"

const unsigned char Menu_Mode_Char[Display_Char_Num]={0x6D,0x79,0x54,0x6D};                     //"SEnS"
const unsigned char Menu_Mode_Normal_Char[Display_Char_Num]={0x08,0x08,0x31,0x01};              //"__|����"
const unsigned char Menu_Mode_2P_Char[Display_Char_Num]={0x08,0x37,0x08,0x80+0x5B};             //"_N_.2"
const unsigned char Menu_Mode_Mid_Char[Display_Char_Num]={0x00,0x37,0x04,0x5E};                 //" Nid"

const unsigned char Menu_Success_Char[Display_Char_Num]={0x6F,0x5C,0x5C,0x5E};                  //"good"
const unsigned char Menu_Fail_Char[Display_Char_Num]={0x71,0x77,0x04,0x38};                     //"FAiL"

const unsigned char Menu_Speed_Char[Display_Char_Num]={0x6D,0x73,0x79,0x5E};                    //"SPEd"
const unsigned char Menu_Speed_Accurate_Char[Display_Char_Num]={0x76,0x80+0x50,0x6D,0x5C};      //"H.rSo"
const unsigned char Menu_Speed_Standard_Char[Display_Char_Num]={0x00,0x6D,0x78,0x5E};           //" Std"

const unsigned char Menu_L_or_D_Char[Display_Char_Num]={0x00,0x38,0x40,0x5E};                   //" L-d"
const unsigned char Menu_Light_Char[Display_Char_Num]={0x38,0x40,0x5C,0x54};                    //"L-on"
const unsigned char Menu_Dark_Char[Display_Char_Num]={0x5E,0x40,0x5C,0x54};                     //"d-on"

const unsigned char Menu_IO_Out_Char[Display_Char_Num]={0x00,0x73,0x40,0x54};                   //" P-n"
const unsigned char Menu_NPN_Out_Char[Display_Char_Num]={0x00,0x54,0x73,0x54};                  //" nPn"
const unsigned char Menu_PNP_Out_Char[Display_Char_Num]={0x00,0x73,0x54,0x73};                  //" PnP"
const unsigned char Menu_Pull_Out_Char[Display_Char_Num]={0x73,0x3E,0x38,0x38};                 //"PULL"

/*
const unsigned char Menu_Aout_Mode_Char[Display_Char_Num]={0x00,0x77,0x5C,0x80+0x37};          //" Ao.N"
const unsigned char Menu_Iout_Mode_Char[Display_Char_Num]={0x04,0x5C,0x3E,0x78};                //"ioUt"
const unsigned char Menu_Vout_Mode_Char[Display_Char_Num]={0x1C,0x5C,0x3E,0x78};                //"voUt"
*/

const unsigned char Menu_Aout_Direction_Char[Display_Char_Num]={0x00,0x77,0x5C,0x80+0x5E};      //" Ao.d"
const unsigned char Menu_Aout_Forward_Char[Display_Char_Num]={0x77,0x5C,0x40,0x71};             //"Ao-F"
const unsigned char Menu_Aout_Reverse_Char[Display_Char_Num]={0x77,0x5C,0x40,0x50};             //"Ao-r"

const unsigned char Menu_Aout_Scope_Char[Display_Char_Num]={0x00,0x77,0x5C,0x80+0x6D};          //" Ao.S"
const unsigned char Menu_Aout_Full_Char[Display_Char_Num]={0x71,0x3E,0x38,0x38};                //"FULL"
const unsigned char Menu_Aout_Window_Char[Display_Char_Num]={0x50,0x77,0x54,0x6F};              //"rAng"

const unsigned char Menu_Input_Char[Display_Char_Num]={0x06,0x54,0x73,0x78};                    //"InPt"
const unsigned char Menu_Input_Off_Char[Display_Char_Num]={0x06,0x54,0x5C,0x71};                //"InoF"
const unsigned char Menu_Laser_Ctrl_Char[Display_Char_Num]={0x38,0x58,0x78,0x50};               //"Lctr"

/*
const unsigned char Menu_Adr485_Char[Display_Char_Num]={0x00,0x77,0x5E,0x50};                   //" Adr"
const unsigned char Menu_Baud_Char[Display_Char_Num]={0x7C,0x77,0x1C,0x5E};                     //"bAud"
const unsigned char Menu_Baud_9600_Char[Display_Char_Num]={0x6F,0x80+0x7D,0x3F,0x3F};           //"9.600"
const unsigned char Menu_Baud_19200_Char[Display_Char_Num]={0x06,0x6F,0x80+0x5B,0x3F};          //"19.20"
const unsigned char Menu_Baud_115200_Char[Display_Char_Num]={0x06,0x06,0x6D,0x80+0x5B};         //"115.2"
const unsigned char Menu_Mode485_Char[Display_Char_Num]={0x78,0x80+0x37,0x5C,0x5E};             //"t.Nod"
const unsigned char Menu_Mode485_Series_Char[Display_Char_Num]={0x00,0x6D,0x79,0x50};           //" SEr"
const unsigned char Menu_Mode485_Polling_Char[Display_Char_Num]={0x73,0x5C,0x38,0x38};          //"PoLL"
*/

const unsigned char Menu_Reset_Char[Display_Char_Num]={0x50,0x6D,0x79,0x78};                    //"rSEt"
const unsigned char Menu_No_Char[Display_Char_Num]={0x00,0x00,0x54,0x5C};                       //"  no"
const unsigned char Menu_Yes_Char[Display_Char_Num]={0x00,0x6E,0x79,0x6D};                      //" yES"


const unsigned char Menu_Aout_Adj1_Char[Display_Char_Num]={0x77,0x5E,0x0E,0x06};         				//"AdJ1"
const unsigned char Menu_Aout_Adj2_Char[Display_Char_Num]={0x77,0x5E,0x0E,0x5B};        				//"AdJ2"
const unsigned char Menu_Calibration_Char[Display_Char_Num]={0x00,0x58,0x77,0x38};         			//" cAL"



const unsigned char Menu_Soft_Version_Char[Display_Char_Num]={0x77,0x06,0x80+0x3F,0x06};        //"A1.01"




extern uint32_t DataBuffer2[16];


uint8_t Menu_Display_Buf[Display_Char_Num];

static uint8_t Key_Value_Last = Key_Value_Null;
static uint16_t Key_Pushing_Count = 0;
static uint8_t Key_State = Key_State_Wait;

volatile uint8_t Para_Save_flag = 0;

uint8_t Teach_Mode = Teach_Mode_Normal;
uint16_t Teach_Point[3];
//float Teach_Window[2];
uint8_t IO_Out_Speed = IO_Out_Speed_Standard;
uint8_t IO_Out_Reverse = IO_Out_Light;
uint8_t IO_Out_Type = IO_Out_NPN;
uint8_t Aout_Direction = Aout_Forward;
uint8_t Aout_Scope = Aout_Full;
uint8_t Input_Function = Input_Off;


static float Teach_Point_temp[3];

uint16_t Distance_Buf[Distance_Buf_len]; 
uint8_t Distance_Buf_P = 0;
uint16_t Distance_avg;
uint16_t Distance_analog;


uint16_t Distance_display_Buf[Distance_display_Buf_len]; 
uint8_t Distance_display_Buf_P = 0;
uint16_t Distance_display = 65535;
uint16_t Distance_teach = 65535;


uint8_t Dout_State = Dout_Off;
uint8_t Dout_State_Buf[Dout_State_Buf_len];
uint8_t Dout_State_Buf_P = 0;
uint8_t D_or_A_monitor_times;


#if (Aout_Mode == Vout_Mode)
	uint16_t Aout_Adjust1 = Aout_200mv_Default;
	uint16_t Aout_Adjust2 = Aout_10v_Default;
	uint16_t Aout_External = Aout_10v2_Default;
#else
	uint16_t Aout_Adjust1 = Aout_4mA_Default;
	uint16_t Aout_Adjust2 = Aout_20mA_Default;
	uint16_t Aout_External = Aout_0mA_Default;
#endif

float K_Aout;
float B_Aout;
uint16_t Aout_min;
uint16_t Aout_max;


uint8_t Aout_Adj = 0;
uint8_t Key_Teach_Count = 0;


void Distance_display_Buf_Init(void)
{
	uint8_t i;

	for(i=0;i<Distance_display_Buf_len;i++)
	{
		Distance_display_Buf[i] = 65535;
	}
}

/*void Get_Distance_analog(void)
{
	uint8_t i, j, k;

	j = 0;
	k = 0;
	for(i=0;i<Distance_Buf_len;i++)
	{
		if(Distance_Buf[i] == 65535)
		{
			j++;
		}
		else
		{
			k++;
		}
	}
	if((j==0) || (k==0))
	{
		Distance_analog = Distance_avg;
	}
}*/

void Get_Distance_analog(void)
{
	uint8_t i, j, k, p;
	uint32_t d;

	p = Distance_Buf_P;
	d = 0;
	j = 0;
	k = 0;
	for(i=0;i<D_or_A_monitor_times;i++)
	{
		if(p == 0)
		{
			p = Distance_Buf_len - 1;
		}
		else
		{
			p--;
		}
		
		d += Distance_Buf[p];
		if(Distance_Buf[p] == 65535)
		{
			j++;
		}
		else
		{
			k++;
		}
	}
	if((j==0) || (k==0))
	{
		Distance_analog = d/D_or_A_monitor_times;
	}
}

void Get_Distance_avg(void)
{
	uint8_t i;
	//uint16_t tempW;
	uint32_t tempW;
	//uint8_t max, min;
	uint8_t num;

	if(Distance_Buf_P >= Distance_Buf_len)
	{
		Distance_Buf_P = 0;
	}
	Distance_Buf[Distance_Buf_P] = Vi5300_distance;
	Distance_Buf_P++;
	
	tempW = 0;
	for(i=0;i<Distance_Buf_len;i++)
	{
		if(Distance_Buf[i] == 0xffff)
		{
			break;
		}
		tempW += Distance_Buf[i];
	}
	if(i == 4){
		Distance_avg = tempW / Distance_Buf_len;
	}else{
		Distance_avg = 0xffff;
	}
	Get_Distance_analog();
		
	if(Distance_Buf_P == Distance_Buf_len){
		if(Distance_display_Buf_P >= Distance_display_Buf_len)
		{
			Distance_display_Buf_P = 0;
		}
		Distance_display_Buf[Distance_display_Buf_P] = Distance_avg;
		Distance_display_Buf_P++;
		
		/*max = 0;
		min = 0;
		tempW = Distance_display_Buf[0];
		for(i = 1; i < Distance_display_Buf_len; i++)
		{
			tempW += Distance_display_Buf[i];
			if(Distance_display_Buf[i] > Distance_display_Buf[max])
			{
				max = i;
			}
			if(Distance_display_Buf[i] < Distance_display_Buf[min])
			{
				min = i;
			}
		}
		tempW -= Distance_display_Buf[max];
		tempW -= Distance_display_Buf[min];
		
		tempW = tempW / (Distance_display_Buf_len-2);
		*/
			
		num = 0;
		tempW = 0;
		for(i = 0; i < Distance_display_Buf_len; i++)
		{
			if(Distance_display_Buf[i] != 0xffff)
			{
				tempW += Distance_display_Buf[i];
				num++;
			}
		}
		//if(num > 0)
		if(Distance_avg != 0xffff){
			tempW = tempW / num;
		}else{
			tempW = 0xffff;
		}
		Distance_teach = tempW;
		
		if(Distance_display > tempW){
			if((Distance_display - tempW) >= 3)
			{
				Distance_display = tempW;
			}
		}else{
			if((tempW - Distance_display) >= 3)
			{
				Distance_display = tempW;
			}
		}
	}
}


void Init_Para_Factory(void)
{
	Aout_Direction = Aout_Forward;		
	Aout_Scope = Aout_Full;		
	Teach_Mode = Teach_Mode_Normal;	
	Teach_Point[0] = Distance_Teach;	
	Teach_Point[1] = Distance_Teach;	
	IO_Out_Speed = IO_Out_Speed_Standard;	
	IO_Out_Reverse = IO_Out_Light;	
	IO_Out_Type = IO_Out_NPN;	
	Input_Function = Input_Off;	
			

	DataBuffer2[0] = 0;
	DataBuffer2[1] = Aout_Direction;
	DataBuffer2[2] = Aout_Scope;
	DataBuffer2[3] = Teach_Mode;
	DataBuffer2[4] = Teach_Point[0];
	DataBuffer2[5] = Teach_Point[1];
	DataBuffer2[6] = IO_Out_Speed;
	DataBuffer2[7] = IO_Out_Reverse;
	DataBuffer2[8] = IO_Out_Type;
	DataBuffer2[12] = Input_Function;

#if (Aout_Mode == Vout_Mode)
	Aout_Adjust1 = Aout_200mv_Default;
	Aout_Adjust2 = Aout_10v_Default;
#else
	Aout_Adjust1 = Aout_4mA_Default;
	Aout_Adjust2 = Aout_20mA_Default;
#endif
	DataBuffer2[9] = Aout_Adjust1;
	DataBuffer2[10] = Aout_Adjust2;
}

uint8_t Get_Key_Value_Current(void)
{
    uint8_t key_value;
    
    key_value = 0;
    if(Read_Key_Down() == 0)
    {
        key_value |= Key_Value_Down;
    }
    if(Read_Key_Up() == 0)
    {
        key_value |= Key_Value_Up;
    }
    if(Read_Key_Teach() == 0)
    {
        key_value |= Key_Value_Teach;
    }
    
    return key_value;
}

//12.5ms
void Get_Key_Value_and_State(void)
{
    static uint8_t Key_Value_Temp = Key_Value_Null;         
    static uint8_t Key_Temp_Count = 0;
    uint8_t key_value_current;
    
    key_value_current = Get_Key_Value_Current();
    
    if(key_value_current == Key_Value_Last)
    {
        if(key_value_current == Key_Value_Null)
        {
            Key_Value_Temp = Key_Value_Null;
            Key_Temp_Count = 0;
            //Key_Pushing_Count = 0;
            //Key_State = Key_State_Wait;
        }else{
            if(Key_Pushing_Count < Key_Long_Delay)
            {
                Key_Pushing_Count++;
            }else{
                Key_State = Key_State_Long;
            }
        }
    }else{
        if(key_value_current == Key_Value_Temp)
        {
            if(Key_Temp_Count < 3)
            {
                Key_Temp_Count++;
            }
            
            if(Key_Temp_Count >= 3)
            {
                if(key_value_current == Key_Value_Null)        
                {
                    if(Key_State == Key_State_Long_next)
                    {
                        Key_State = Key_State_Wait;
                        Key_Value_Last = Key_Value_Null;
                    }else if(Key_Pushing_Count >= Key_Short_Delay){
                        Key_State = Key_State_Short;
                    }else{
                        Key_State = Key_State_Wait;
                        Key_Value_Last = Key_Value_Null;
                    }
                }else if(key_value_current > Key_Value_Last){    
                    Key_Value_Last = key_value_current;
                    Key_Pushing_Count = 0;
                    Key_State = Key_State_Pushing;
                }               
            }           
        }else{
            Key_Value_Temp = key_value_current;
            Key_Temp_Count = 1;
        }
    }  
}

void Code_to_Buf(uint8_t* buf, const uint8_t* code, uint8_t num)
{
    uint8_t i;
    
    for(i=0;i<num;i++)
    {
        *(buf+i) = *(code+i);
    }
}

/*void Baud_to_Display_Buf(uint8_t baud_seq)
{
    if(baud_seq == Baud_9600)
    {
        Code_to_Buf(Menu_Display_Buf, Menu_Baud_9600_Char, Display_Char_Num);
    }
    else if(baud_seq == Baud_19200)
    {
        Code_to_Buf(Menu_Display_Buf, Menu_Baud_19200_Char, Display_Char_Num);
    }
    else
    {
        Code_to_Buf(Menu_Display_Buf, Menu_Baud_115200_Char, Display_Char_Num);
    }
}*/

void TPx_to_Display_Buf(uint8_t x)
{
    Menu_Display_Buf[0] = 0x00;         //" "
    Menu_Display_Buf[1] = 0x38;         //"L"
    Menu_Display_Buf[2] = 0x73;         //"P"
    Menu_Display_Buf[3] = TM16_num_buf[x%10];
}

void Int16_to_Display_Buf(int dat)
{
    uint8_t neg_flag;
    
    if(dat < 0)
    {
        dat = -dat;
        neg_flag = 1;
    }else{
        neg_flag = 0;
    }
    
    Menu_Display_Buf[0] = TM16_num_buf[dat / 1000];
    if(neg_flag)
    {
        Menu_Display_Buf[0] |= 0x80;
    }
    dat = dat % 1000;
    Menu_Display_Buf[1] = TM16_num_buf[dat / 100];
    dat = dat % 100;
    Menu_Display_Buf[2] = TM16_num_buf[dat / 10];
    Menu_Display_Buf[3] = TM16_num_buf[dat % 10];

}

void Distance_to_Display_Buf(void)
{
    if((Distance_display > Distance_Scope) || (Distance_display == 4500)){
        Menu_Display_Buf[0] = 0x40;
        Menu_Display_Buf[1] = 0x40;
        Menu_Display_Buf[2] = 0x40;
        Menu_Display_Buf[3] = 0x40;
    }else{
		Int16_to_Display_Buf(Distance_display);
    }
		
}

void Distance2_to_Display_Buf(void)
{
	if(Distance_display >= 4500)
	{
        Menu_Display_Buf[0] = 0x40;
        Menu_Display_Buf[1] = 0x40;
        Menu_Display_Buf[2] = 0x40;
        Menu_Display_Buf[3] = 0x40;
    }else{
		Int16_to_Display_Buf(Distance_display);
    }
}

void Menu_Display(void)
{
    Uart_Send_Display_Buf();
}


void Menu_Key_Operate(void)
{
	static uint8_t Menu_Item = Menu_Measure;
	static uint16_t Menu_Refurbish = 0;
	static uint8_t Teach_Mode_temp;
	static uint8_t Teach_Count = 0;
	static uint16_t Teaching_Count = 0;

	Get_Key_Value_and_State();

	switch(Menu_Item)
    {
		case Menu_Measure:
			if(Para_Save_flag)
			{
				Flash_CmdDate_Write();
				Para_Save_flag = 0;
			}
			if(Menu_Refurbish >= 10)
			{        //gp20200727
				Menu_Refurbish = 0;
			}
			if(Key_State == Key_State_Long)
			{
				if(Key_Value_Last == Key_Value_Teach)
				{
					Code_to_Buf(Menu_Display_Buf, Menu_Mode_Char, Display_Char_Num);
					Menu_Item = Menu_Mode;
					Menu_Refurbish = 0;
				}else if(Key_Value_Last == Key_Value_Teach_Down){
					Code_to_Buf(Menu_Display_Buf, Menu_Soft_Version_Char, Display_Char_Num);
					Menu_Item = Menu_Soft_Version;
					Menu_Refurbish = 0;
				}
			}
			break;
            
        case Menu_Mode:
            if(Menu_Refurbish >= 2000)
            {
				Menu_Item = Menu_Measure;
				Menu_Refurbish = 0;
			}
			if((Key_State == Key_State_Short) || (Key_State == Key_State_Long))
			{
				if(Key_Value_Last == Key_Value_Teach)
                {
					if(Key_State == Key_State_Long)
                    {
                        Menu_Item = Menu_Measure;
                    }else if(Teach_Mode == Teach_Mode_Normal)
                    {
                        Code_to_Buf(Menu_Display_Buf, Menu_Mode_Normal_Char, Display_Char_Num);
                        Menu_Item = Menu_Mode_Normal;
                    }
                    else if(Teach_Mode == Teach_Mode_2P)
                    {
                        Code_to_Buf(Menu_Display_Buf, Menu_Mode_2P_Char, Display_Char_Num);
                        Menu_Item = Menu_Mode_2P;
                    }
                    else //if(Teach_Mode == Teach_Mode_Mid)
                    {
                        Code_to_Buf(Menu_Display_Buf, Menu_Mode_Mid_Char, Display_Char_Num);
                        Menu_Item = Menu_Mode_Mid;
                    }
                    
					Menu_Refurbish = 0;
				}else if(Key_Value_Last == Key_Value_Down){
					Code_to_Buf(Menu_Display_Buf, Menu_Speed_Char, Display_Char_Num);
                    Menu_Item = Menu_Speed;
                    Menu_Refurbish = 0;
				}else if(Key_Value_Last == Key_Value_Up){
                    Code_to_Buf(Menu_Display_Buf, Menu_Reset_Char, Display_Char_Num);
                    Menu_Item = Menu_Reset;
                    Menu_Refurbish = 0;
                }
			}					
			break;
				
		case Menu_Mode_Normal:
            if(Menu_Refurbish >= 2000)
            {
                Code_to_Buf(Menu_Display_Buf, Menu_Mode_Char, Display_Char_Num);
                Menu_Item = Menu_Mode;
                Menu_Refurbish = 0;
            }
            if((Key_State == Key_State_Short) || (Key_State == Key_State_Long))
            {
                if(Key_Value_Last == Key_Value_Teach)
                {
                    Teach_Mode_temp = Teach_Mode_Normal;
                    Teach_Count = 0;
                    TPx_to_Display_Buf(Teach_Count+1);
                    Menu_Item = Menu_Teach_Step;
                    //Menu_Item = Menu_Mode_Teaching;
                    Menu_Refurbish = 0;
                }
                else if(Key_Value_Last == Key_Value_Down)
                {
                    Code_to_Buf(Menu_Display_Buf, Menu_Mode_2P_Char, Display_Char_Num);
                    Menu_Item = Menu_Mode_2P;
                    Menu_Refurbish = 0;
                }
                else if(Key_Value_Last == Key_Value_Up)
                {
                    Code_to_Buf(Menu_Display_Buf, Menu_Mode_Mid_Char, Display_Char_Num);
                    Menu_Item = Menu_Mode_Mid;
                    Menu_Refurbish = 0;
                }
            }
			break;
				
		case Menu_Mode_2P:
            if(Menu_Refurbish >= 2000)
            {
                Code_to_Buf(Menu_Display_Buf, Menu_Mode_Char, Display_Char_Num);
                Menu_Item = Menu_Mode;
                Menu_Refurbish = 0;
            }
            if((Key_State == Key_State_Short) || (Key_State == Key_State_Long))
            {
                if(Key_Value_Last == Key_Value_Teach)
                {
                    Teach_Mode_temp = Teach_Mode_2P;
                    Teach_Count = 0;
                    TPx_to_Display_Buf(Teach_Count+1);
                    Menu_Item = Menu_Teach_Step;
                    //Menu_Item = Menu_Mode_Teaching;
                    Menu_Refurbish = 0;
                }
                else if(Key_Value_Last == Key_Value_Down)
                {
                    Code_to_Buf(Menu_Display_Buf, Menu_Mode_Mid_Char, Display_Char_Num);
                    Menu_Item = Menu_Mode_Mid;
                    Menu_Refurbish = 0;
                }
                else if(Key_Value_Last == Key_Value_Up)
                {
                    Code_to_Buf(Menu_Display_Buf, Menu_Mode_Normal_Char, Display_Char_Num);
                    Menu_Item = Menu_Mode_Normal;
                    Menu_Refurbish = 0;
                }
            }
			break;
				
		case Menu_Mode_Mid:
            if(Menu_Refurbish >= 2000)
            {
                Code_to_Buf(Menu_Display_Buf, Menu_Mode_Char, Display_Char_Num);
                Menu_Item = Menu_Mode;
                Menu_Refurbish = 0;
            }
            if((Key_State == Key_State_Short) || (Key_State == Key_State_Long))
            {
                if(Key_Value_Last == Key_Value_Teach)
                {
                    Teach_Mode_temp = Teach_Mode_Mid;
                    Teach_Count = 0;
                    TPx_to_Display_Buf(Teach_Count+1);
                    Menu_Item = Menu_Teach_Step;
                    //Menu_Item = Menu_Mode_Teaching;
                    Menu_Refurbish = 0;
                }
                else if(Key_Value_Last == Key_Value_Down)
                {
                    Code_to_Buf(Menu_Display_Buf, Menu_Mode_Normal_Char, Display_Char_Num);
                    Menu_Item = Menu_Mode_Normal;
                    Menu_Refurbish = 0;
                }
                else if(Key_Value_Last == Key_Value_Up)
                {
                    Code_to_Buf(Menu_Display_Buf, Menu_Mode_2P_Char, Display_Char_Num);
                    Menu_Item = Menu_Mode_2P;
                    Menu_Refurbish = 0;
                }
            }
			break;
					
		case Menu_Mode_Teaching:
            Teaching_Count++;
            if(Teaching_Count >= 3000)
            {
                Code_to_Buf(Menu_Display_Buf, Menu_Fail_Char, Display_Char_Num);
                Menu_Item = Menu_Teach_Fail;
                Menu_Refurbish = 0;
            }
            if(Menu_Refurbish >= 50)
            {
                Menu_Refurbish = 0;
            }
			if((Key_State == Key_State_Short) || (Key_State == Key_State_Long))
            {
				if(Key_Value_Last == Key_Value_Teach)
				{
					if(Teach_Mode_temp == Teach_Mode_Normal)
					{
						//if(Distance_teach < Distance_Scope)
						if(Distance_teach <= Distance_Teach)
						{
							Teach_Point[0] = Distance_teach;
							Teach_Mode = Teach_Mode_temp;
							Code_to_Buf(Menu_Display_Buf, Menu_Success_Char, Display_Char_Num);
							Para_Save_flag = 1;
							DataBuffer2[3] = Teach_Mode;
							DataBuffer2[4] = Teach_Point[0];


							Menu_Item = Menu_Teach_Success;
						}else{
							Code_to_Buf(Menu_Display_Buf, Menu_Fail_Char, Display_Char_Num);
							Menu_Item = Menu_Teach_Fail;
						}
					}else if(Teach_Mode_temp == Teach_Mode_2P){
						//if(Distance_teach < Distance_Scope)
						if(Distance_teach <= Distance_Teach)
						{
							Teach_Point_temp[Teach_Count] = Distance_teach;
							Teach_Count++;
							if(Teach_Count >= 2)
							{
								if(Teach_Point_temp[0] > Teach_Point_temp[1])
								{
									Teach_Point_temp[2] = Teach_Point_temp[0];
									Teach_Point_temp[0] = Teach_Point_temp[1];
									Teach_Point_temp[1] = Teach_Point_temp[2];
								}
								if((Teach_Point_temp[1] - Teach_Point_temp[0]) > 50)
								{
									Teach_Point[0] = Teach_Point_temp[0];
									Teach_Point[1] = Teach_Point_temp[1];

									Teach_Mode = Teach_Mode_temp;
									Code_to_Buf(Menu_Display_Buf, Menu_Success_Char, Display_Char_Num);
									Para_Save_flag = 1;
									DataBuffer2[3] = Teach_Mode;
									DataBuffer2[4] = Teach_Point[0];
									DataBuffer2[5] = Teach_Point[1];

									Menu_Item = Menu_Teach_Success;
								}else{
									Code_to_Buf(Menu_Display_Buf, Menu_Fail_Char, Display_Char_Num);
									Menu_Item = Menu_Teach_Fail;
								}
							}else{
								TPx_to_Display_Buf(Teach_Count+1);
								Menu_Item = Menu_Teach_Step;
							}
						}else{
							Code_to_Buf(Menu_Display_Buf, Menu_Fail_Char, Display_Char_Num);
							Menu_Item = Menu_Teach_Fail;
						}
					}else if(Teach_Mode_temp == Teach_Mode_Mid){
						//if(Distance_teach < Distance_Scope)
						if(Distance_teach <= Distance_Teach)
						{
							Teach_Point_temp[Teach_Count] = Distance_teach;
							Teach_Count++;
							if(Teach_Count >= 2)
							{
								Teach_Point[0] = (Teach_Point_temp[0] + Teach_Point_temp[1])/2;
								Teach_Point[1] = Teach_Point_temp[1];
								Teach_Mode = Teach_Mode_temp;
								Code_to_Buf(Menu_Display_Buf, Menu_Success_Char, Display_Char_Num);
								Para_Save_flag = 1;
								DataBuffer2[3] = Teach_Mode;
								DataBuffer2[4] = Teach_Point[0];
								DataBuffer2[5] = Teach_Point[1];

								Menu_Item = Menu_Teach_Success;
							}else{
								TPx_to_Display_Buf(Teach_Count+1);
								Menu_Item = Menu_Teach_Step;
							}
						}else{
							Code_to_Buf(Menu_Display_Buf, Menu_Fail_Char, Display_Char_Num);
							Menu_Item = Menu_Teach_Fail;
						}
					}
					Menu_Refurbish = 0;
				}
			}		
			break;
								
		case Menu_Teach_Step:
            if(Menu_Refurbish >= 200)
            {
                Menu_Item = Menu_Mode_Teaching;
                Menu_Refurbish = 0;
            }
			break;
            
        case Menu_Teach_Success:
        case Menu_Teach_Fail:
            if(Menu_Refurbish >= 300)
            {
                Code_to_Buf(Menu_Display_Buf, Menu_Mode_Char, Display_Char_Num);
                Menu_Item = Menu_Mode;
                Menu_Refurbish = 0;
            }
			break;
				
		case Menu_Speed:
            if(Menu_Refurbish >= 2000)
            {
                Menu_Item = Menu_Measure;
                Menu_Refurbish = 0;
            }
            if((Key_State == Key_State_Short) || (Key_State == Key_State_Long))
            {
                if(Key_Value_Last == Key_Value_Teach)
                {
                    if(Key_State == Key_State_Long)
                    {
                        Menu_Item = Menu_Measure;
                    }else if(IO_Out_Speed == IO_Out_Speed_Standard){
                        Code_to_Buf(Menu_Display_Buf, Menu_Speed_Standard_Char, Display_Char_Num);
                        Menu_Item = Menu_Speed_Standard;
                    }else{ //if(IO_Out_Speed == IO_Out_Speed_Accurate)
                        Code_to_Buf(Menu_Display_Buf, Menu_Speed_Accurate_Char, Display_Char_Num);
                        Menu_Item = Menu_Speed_Accurate;
                    }
					Menu_Refurbish = 0;
                }else if(Key_Value_Last == Key_Value_Down){
                    Code_to_Buf(Menu_Display_Buf, Menu_L_or_D_Char, Display_Char_Num);
                    Menu_Item = Menu_L_or_D;
                    Menu_Refurbish = 0;
                }else if(Key_Value_Last == Key_Value_Up){
                    Code_to_Buf(Menu_Display_Buf, Menu_Mode_Char, Display_Char_Num);
                    Menu_Item = Menu_Mode;
                    Menu_Refurbish = 0;
                }
            }
			break;

		case Menu_Speed_Accurate:
            if(Menu_Refurbish >= 2000)
            {
                Code_to_Buf(Menu_Display_Buf, Menu_Speed_Char, Display_Char_Num);
                Menu_Item = Menu_Speed;
                Menu_Refurbish = 0;
            }
            if((Key_State == Key_State_Short) || (Key_State == Key_State_Long))
            {
                if(Key_Value_Last == Key_Value_Teach)
                {
                    Code_to_Buf(Menu_Display_Buf, Menu_Speed_Char, Display_Char_Num);
                    if(IO_Out_Speed != IO_Out_Speed_Accurate)
                    {
						Para_Save_flag = 1;
						IO_Out_Speed = IO_Out_Speed_Accurate;
						DataBuffer2[6] = IO_Out_Speed;
                    }
                    Menu_Item = Menu_Speed;
                    Menu_Refurbish = 0;
                }else if(Key_Value_Last == Key_Value_Down){
                    Code_to_Buf(Menu_Display_Buf, Menu_Speed_Standard_Char, Display_Char_Num);
                    Menu_Item = Menu_Speed_Standard;
                    Menu_Refurbish = 0;
                }else if(Key_Value_Last == Key_Value_Up){
                    Code_to_Buf(Menu_Display_Buf, Menu_Speed_Standard_Char, Display_Char_Num);
                    Menu_Item = Menu_Speed_Standard;
                    Menu_Refurbish = 0;
                }
            }
			break;
								
		case Menu_Speed_Standard:
            if(Menu_Refurbish >= 2000)
            {
                Code_to_Buf(Menu_Display_Buf, Menu_Speed_Char, Display_Char_Num);
                Menu_Item = Menu_Speed;
                Menu_Refurbish = 0;
            }
            if((Key_State == Key_State_Short) || (Key_State == Key_State_Long))
            {
                if(Key_Value_Last == Key_Value_Teach)
                {
                    Code_to_Buf(Menu_Display_Buf, Menu_Speed_Char, Display_Char_Num);
                    if(IO_Out_Speed != IO_Out_Speed_Standard)
                    {
                        Para_Save_flag = 1;
                        IO_Out_Speed = IO_Out_Speed_Standard;
						DataBuffer2[6] = IO_Out_Speed;
                    }
                    Menu_Item = Menu_Speed;
                    Menu_Refurbish = 0;
                }else if(Key_Value_Last == Key_Value_Down){
                    Code_to_Buf(Menu_Display_Buf, Menu_Speed_Accurate_Char, Display_Char_Num);
                    Menu_Item = Menu_Speed_Accurate;
                    Menu_Refurbish = 0;
                }else if(Key_Value_Last == Key_Value_Up){
                    Code_to_Buf(Menu_Display_Buf, Menu_Speed_Accurate_Char, Display_Char_Num);
                    Menu_Item = Menu_Speed_Accurate;
                    Menu_Refurbish = 0;
                }
            }
			break;
						
		case Menu_L_or_D:
            if(Menu_Refurbish >= 2000)
            {
                Menu_Item = Menu_Measure;
                Menu_Refurbish = 0;
            }
            if((Key_State == Key_State_Short) || (Key_State == Key_State_Long))
            {
                if(Key_Value_Last == Key_Value_Teach)
                {
                    if(Key_State == Key_State_Long)
                    {
                        Menu_Item = Menu_Measure;
                    }
                    else if(IO_Out_Reverse == IO_Out_Dark)
                    {
                        Code_to_Buf(Menu_Display_Buf, Menu_Dark_Char, Display_Char_Num);
                        Menu_Item = Menu_Dark;
                    }
                    else //if(IO_Out_Reverse == IO_Out_Light)
                    {
                        Code_to_Buf(Menu_Display_Buf, Menu_Light_Char, Display_Char_Num);
                        Menu_Item = Menu_Light;
                    }

                    Menu_Refurbish = 0;
                }
                else if(Key_Value_Last == Key_Value_Down)
                {
                    Code_to_Buf(Menu_Display_Buf, Menu_IO_Out_Char, Display_Char_Num);
                    Menu_Item = Menu_IO_Out;
                    Menu_Refurbish = 0;
                }
                else if(Key_Value_Last == Key_Value_Up)
                {
                    Code_to_Buf(Menu_Display_Buf, Menu_Speed_Char, Display_Char_Num);
                    Menu_Item = Menu_Speed;
                    Menu_Refurbish = 0;
                }
            }
			break;

		case Menu_Light:
            if(Menu_Refurbish >= 2000)
            {
                Code_to_Buf(Menu_Display_Buf, Menu_L_or_D_Char, Display_Char_Num);
                Menu_Item = Menu_L_or_D;
                Menu_Refurbish = 0;
            }
            if((Key_State == Key_State_Short) || (Key_State == Key_State_Long))
            {
                if(Key_Value_Last == Key_Value_Teach)
                {
                    Code_to_Buf(Menu_Display_Buf, Menu_L_or_D_Char, Display_Char_Num);
                    if(IO_Out_Reverse != IO_Out_Light)
                    {
                        Para_Save_flag = 1;
                        IO_Out_Reverse = IO_Out_Light;
						DataBuffer2[7] = IO_Out_Reverse;
                    }
                    Menu_Item = Menu_L_or_D;
                    Menu_Refurbish = 0;
                }
                else if(Key_Value_Last == Key_Value_Down)
                {
                    Code_to_Buf(Menu_Display_Buf, Menu_Dark_Char, Display_Char_Num);
                    Menu_Item = Menu_Dark;
                    Menu_Refurbish = 0;
                }
                else if(Key_Value_Last == Key_Value_Up)
                {
                    Code_to_Buf(Menu_Display_Buf, Menu_Dark_Char, Display_Char_Num);
                    Menu_Item = Menu_Dark;
                    Menu_Refurbish = 0;
                }
            }
			break;
								
		case Menu_Dark:
            if(Menu_Refurbish >= 2000)
            {
                Code_to_Buf(Menu_Display_Buf, Menu_L_or_D_Char, Display_Char_Num);
                Menu_Item = Menu_L_or_D;
                Menu_Refurbish = 0;
            }
            if((Key_State == Key_State_Short) || (Key_State == Key_State_Long))
            {
                if(Key_Value_Last == Key_Value_Teach)
                {
                    Code_to_Buf(Menu_Display_Buf, Menu_L_or_D_Char, Display_Char_Num);
                    if(IO_Out_Reverse != IO_Out_Dark)
                    {
                        Para_Save_flag = 1;
                        IO_Out_Reverse = IO_Out_Dark;
						DataBuffer2[7] = IO_Out_Reverse;
                    }
                    Menu_Item = Menu_L_or_D;
                    Menu_Refurbish = 0;
                }
                else if(Key_Value_Last == Key_Value_Down)
                {
                    Code_to_Buf(Menu_Display_Buf, Menu_Light_Char, Display_Char_Num);
                    Menu_Item = Menu_Light;
                    Menu_Refurbish = 0;
                }
                else if(Key_Value_Last == Key_Value_Up)
                {
                    Code_to_Buf(Menu_Display_Buf, Menu_Light_Char, Display_Char_Num);
                    Menu_Item = Menu_Light;
                    Menu_Refurbish = 0;
                }
            }
			break;

		case Menu_IO_Out:
			if(Menu_Refurbish >= 2000)
			{
				Menu_Item = Menu_Measure;
				Menu_Refurbish = 0;
			}
			if((Key_State == Key_State_Short) || (Key_State == Key_State_Long))
			{
				if(Key_Value_Last == Key_Value_Teach)
				{
					if(Key_State == Key_State_Long)
					{
						Menu_Item = Menu_Measure;
					}
					else if(IO_Out_Type == IO_Out_PNP)
					{
						Code_to_Buf(Menu_Display_Buf, Menu_PNP_Out_Char, Display_Char_Num);
						Menu_Item = Menu_PNP_Out;
					}
					else if(IO_Out_Type == IO_Out_NPN)
					{
						Code_to_Buf(Menu_Display_Buf, Menu_NPN_Out_Char, Display_Char_Num);
						Menu_Item = Menu_NPN_Out;
					}
					else //if(IO_Out_Type == IO_Out_Pull)
					{
						Code_to_Buf(Menu_Display_Buf, Menu_Pull_Out_Char, Display_Char_Num);
						Menu_Item = Menu_Pull_Out;
					}
					Menu_Refurbish = 0;
				}
				else if(Key_Value_Last == Key_Value_Down)
				{
					Code_to_Buf(Menu_Display_Buf, Menu_Aout_Direction_Char, Display_Char_Num);
					Menu_Item = Menu_Aout_Direction;
					Menu_Refurbish = 0;
				}
				else if(Key_Value_Last == Key_Value_Up)
				{
					Code_to_Buf(Menu_Display_Buf, Menu_L_or_D_Char, Display_Char_Num);
					Menu_Item = Menu_L_or_D;
					Menu_Refurbish = 0;
				}
			}
			break;

		case Menu_NPN_Out:
			if(Menu_Refurbish >= 2000)
			{
				Code_to_Buf(Menu_Display_Buf, Menu_IO_Out_Char, Display_Char_Num);
				Menu_Item = Menu_IO_Out;
				Menu_Refurbish = 0;
			}
			if((Key_State == Key_State_Short) || (Key_State == Key_State_Long))
			{
				if(Key_Value_Last == Key_Value_Teach)
				{
					Code_to_Buf(Menu_Display_Buf, Menu_IO_Out_Char, Display_Char_Num);
					if(IO_Out_Type != IO_Out_NPN)
					{
						Para_Save_flag = 1;
						IO_Out_Type = IO_Out_NPN;
						DataBuffer2[8] = IO_Out_Type;
					}
					Menu_Item = Menu_IO_Out;
					Menu_Refurbish = 0;
				}
				else if(Key_Value_Last == Key_Value_Down)
				{
					Code_to_Buf(Menu_Display_Buf, Menu_PNP_Out_Char, Display_Char_Num);
					Menu_Item = Menu_PNP_Out;
					Menu_Refurbish = 0;
				}
				else if(Key_Value_Last == Key_Value_Up)
				{
					Code_to_Buf(Menu_Display_Buf, Menu_Pull_Out_Char, Display_Char_Num);
					Menu_Item = Menu_Pull_Out;
					Menu_Refurbish = 0;
				}
			}
			break;

		case Menu_PNP_Out:
			if(Menu_Refurbish >= 2000)
			{
				Code_to_Buf(Menu_Display_Buf, Menu_IO_Out_Char, Display_Char_Num);
				Menu_Item = Menu_IO_Out;
				Menu_Refurbish = 0;
			}
			if((Key_State == Key_State_Short) || (Key_State == Key_State_Long))
			{
				if(Key_Value_Last == Key_Value_Teach)
				{
					Code_to_Buf(Menu_Display_Buf, Menu_IO_Out_Char, Display_Char_Num);
					if(IO_Out_Type != IO_Out_PNP)
					{
						Para_Save_flag = 1;
						IO_Out_Type = IO_Out_PNP;
						DataBuffer2[8] = IO_Out_Type;
					}
					Menu_Item = Menu_IO_Out;
					Menu_Refurbish = 0;
				}
				else if(Key_Value_Last == Key_Value_Down)
				{
					Code_to_Buf(Menu_Display_Buf, Menu_Pull_Out_Char, Display_Char_Num);
					Menu_Item = Menu_Pull_Out;
					Menu_Refurbish = 0;
				}
				else if(Key_Value_Last == Key_Value_Up)
				{
					Code_to_Buf(Menu_Display_Buf, Menu_NPN_Out_Char, Display_Char_Num);
					Menu_Item = Menu_NPN_Out;
					Menu_Refurbish = 0;
				}
			}
			break;
						
		case Menu_Pull_Out:
			if(Menu_Refurbish >= 2000)
			{
				Code_to_Buf(Menu_Display_Buf, Menu_IO_Out_Char, Display_Char_Num);
				Menu_Item = Menu_IO_Out;
				Menu_Refurbish = 0;
			}
			if((Key_State == Key_State_Short) || (Key_State == Key_State_Long))
			{
				if(Key_Value_Last == Key_Value_Teach)
				{
					Code_to_Buf(Menu_Display_Buf, Menu_IO_Out_Char, Display_Char_Num);
					if(IO_Out_Type != IO_Out_Pull)
					{
						Para_Save_flag = 1;
						IO_Out_Type = IO_Out_Pull;
						DataBuffer2[8] = IO_Out_Type;
					}
					Menu_Item = Menu_IO_Out;
					Menu_Refurbish = 0;
				}
				else if(Key_Value_Last == Key_Value_Down)
				{
					Code_to_Buf(Menu_Display_Buf, Menu_NPN_Out_Char, Display_Char_Num);
					Menu_Item = Menu_NPN_Out;
					Menu_Refurbish = 0;
				}
				else if(Key_Value_Last == Key_Value_Up)
				{
					Code_to_Buf(Menu_Display_Buf, Menu_PNP_Out_Char, Display_Char_Num);
					Menu_Item = Menu_PNP_Out;
					Menu_Refurbish = 0;
				}
			}
			break;

		case Menu_Aout_Direction:
			if(Menu_Refurbish >= 2000){
				Menu_Item = Menu_Measure;
				Menu_Refurbish = 0;
			}
			if((Key_State == Key_State_Short) || (Key_State == Key_State_Long)){
				if(Key_Value_Last == Key_Value_Teach){
					if(Key_State == Key_State_Long){
						Menu_Item = Menu_Measure;
					}else if(Aout_Direction == Aout_Forward){
						Code_to_Buf(Menu_Display_Buf, Menu_Aout_Forward_Char, Display_Char_Num);
						Menu_Item = Menu_Aout_Forward;
					}else{
						Code_to_Buf(Menu_Display_Buf, Menu_Aout_Reverse_Char, Display_Char_Num);
						Menu_Item = Menu_Aout_Reverse;
					}

					Menu_Refurbish = 0;
				}else if(Key_Value_Last == Key_Value_Down){
					Code_to_Buf(Menu_Display_Buf, Menu_Aout_Scope_Char, Display_Char_Num);
					Menu_Item = Menu_Aout_Scope;
					Menu_Refurbish = 0;
				}else if(Key_Value_Last == Key_Value_Up){
					Code_to_Buf(Menu_Display_Buf, Menu_IO_Out_Char, Display_Char_Num);
					Menu_Item = Menu_IO_Out;
					Menu_Refurbish = 0;
				}
			}
			break;
				
		case Menu_Aout_Forward:
			if(Menu_Refurbish >= 2000){
				Code_to_Buf(Menu_Display_Buf, Menu_Aout_Direction_Char, Display_Char_Num);
				Menu_Item = Menu_Aout_Direction;
				Menu_Refurbish = 0;
			}
			if((Key_State == Key_State_Short) || (Key_State == Key_State_Long)){
				if(Key_Value_Last == Key_Value_Teach){
					Code_to_Buf(Menu_Display_Buf, Menu_Aout_Direction_Char, Display_Char_Num);
					if(Aout_Direction != Aout_Forward){
						Para_Save_flag = 1;
						Aout_Direction = Aout_Forward;
						DataBuffer2[1] = Aout_Direction;
					}
					Menu_Item = Menu_Aout_Direction;
					Menu_Refurbish = 0;
				}else if(Key_Value_Last == Key_Value_Down){
					Code_to_Buf(Menu_Display_Buf, Menu_Aout_Reverse_Char, Display_Char_Num);
					Menu_Item = Menu_Aout_Reverse;
					Menu_Refurbish = 0;
				}else if(Key_Value_Last == Key_Value_Up){
					Code_to_Buf(Menu_Display_Buf, Menu_Aout_Reverse_Char, Display_Char_Num);
					Menu_Item = Menu_Aout_Reverse;
					Menu_Refurbish = 0;
				}
			}
			break;	
						
		case Menu_Aout_Reverse:
			if(Menu_Refurbish >= 2000){
				Code_to_Buf(Menu_Display_Buf, Menu_Aout_Direction_Char, Display_Char_Num);
				Menu_Item = Menu_Aout_Direction;
				Menu_Refurbish = 0;
			}
			if((Key_State == Key_State_Short) || (Key_State == Key_State_Long)){
				if(Key_Value_Last == Key_Value_Teach){
					Code_to_Buf(Menu_Display_Buf, Menu_Aout_Direction_Char, Display_Char_Num);
					if(Aout_Direction != Aout_Reverse){
						Para_Save_flag = 1;
						Aout_Direction = Aout_Reverse;
						DataBuffer2[1] = Aout_Direction;
					}
					Menu_Item = Menu_Aout_Direction;
					Menu_Refurbish = 0;
				}else if(Key_Value_Last == Key_Value_Down){
					Code_to_Buf(Menu_Display_Buf, Menu_Aout_Forward_Char, Display_Char_Num);
					Menu_Item = Menu_Aout_Forward;
					Menu_Refurbish = 0;
				}else if(Key_Value_Last == Key_Value_Up){
					Code_to_Buf(Menu_Display_Buf, Menu_Aout_Forward_Char, Display_Char_Num);
					Menu_Item = Menu_Aout_Forward;
					Menu_Refurbish = 0;
				}
			}
			break;
								
		case Menu_Aout_Scope:
			if(Menu_Refurbish >= 2000){
				Menu_Item = Menu_Measure;
				Menu_Refurbish = 0;
			}
			if((Key_State == Key_State_Short) || (Key_State == Key_State_Long)){
				if(Key_Value_Last == Key_Value_Teach){
					if(Key_State == Key_State_Long)
					{
						Menu_Item = Menu_Measure;
					}else if(Aout_Scope == Aout_Full){
						Code_to_Buf(Menu_Display_Buf, Menu_Aout_Full_Char, Display_Char_Num);
						Menu_Item = Menu_Aout_Full;
					}else{
						Code_to_Buf(Menu_Display_Buf, Menu_Aout_Window_Char, Display_Char_Num);
						Menu_Item = Menu_Aout_Window;
					}
					Menu_Refurbish = 0;
				}else if(Key_Value_Last == Key_Value_Down)
				{
					Code_to_Buf(Menu_Display_Buf, Menu_Input_Char, Display_Char_Num);
					Menu_Item = Menu_Input;
					Menu_Refurbish = 0;
				}else if(Key_Value_Last == Key_Value_Up)
				{
					Code_to_Buf(Menu_Display_Buf, Menu_Aout_Direction_Char, Display_Char_Num);
					Menu_Item = Menu_Aout_Direction;
					Menu_Refurbish = 0;
				}
			}
			break;
				
		case Menu_Aout_Full:
			if(Menu_Refurbish >= 2000){
				Code_to_Buf(Menu_Display_Buf, Menu_Aout_Scope_Char, Display_Char_Num);
				Menu_Item = Menu_Aout_Scope;
				Menu_Refurbish = 0;
			}
			if((Key_State == Key_State_Short) || (Key_State == Key_State_Long))
			{
				if(Key_Value_Last == Key_Value_Teach)
				{
					Code_to_Buf(Menu_Display_Buf, Menu_Aout_Scope_Char, Display_Char_Num);
					if(Aout_Scope != Aout_Full)
					{
						Para_Save_flag = 1;
						Aout_Scope = Aout_Full;
						DataBuffer2[2] = Aout_Scope;
					}
					Menu_Item = Menu_Aout_Scope;
					Menu_Refurbish = 0;
				}else if(Key_Value_Last == Key_Value_Down)
				{
					Code_to_Buf(Menu_Display_Buf, Menu_Aout_Window_Char, Display_Char_Num);
					Menu_Item = Menu_Aout_Window;
					Menu_Refurbish = 0;
				}else if(Key_Value_Last == Key_Value_Up)
				{
					Code_to_Buf(Menu_Display_Buf, Menu_Aout_Window_Char, Display_Char_Num);
					Menu_Item = Menu_Aout_Window;
					Menu_Refurbish = 0;
				}
			}
			break;
				
		case Menu_Aout_Window:
			if(Menu_Refurbish >= 2000)
			{
				Code_to_Buf(Menu_Display_Buf, Menu_Aout_Scope_Char, Display_Char_Num);
				Menu_Item = Menu_Aout_Scope;
				Menu_Refurbish = 0;
			}
			if((Key_State == Key_State_Short) || (Key_State == Key_State_Long))
			{
				if(Key_Value_Last == Key_Value_Teach)
				{
					Code_to_Buf(Menu_Display_Buf, Menu_Aout_Scope_Char, Display_Char_Num);
					if(Aout_Scope != Aout_Window)
					{
						Para_Save_flag = 1;
						Aout_Scope = Aout_Window;
						DataBuffer2[2] = Aout_Scope;
					}
					Menu_Item = Menu_Aout_Scope;
					Menu_Refurbish = 0;
				}
				else if(Key_Value_Last == Key_Value_Down)
				{
					Code_to_Buf(Menu_Display_Buf, Menu_Aout_Full_Char, Display_Char_Num);
					Menu_Item = Menu_Aout_Full;
					Menu_Refurbish = 0;
				}
				else if(Key_Value_Last == Key_Value_Up)
				{
					Code_to_Buf(Menu_Display_Buf, Menu_Aout_Full_Char, Display_Char_Num);
					Menu_Item = Menu_Aout_Full;
					Menu_Refurbish = 0;
				}
			}
			break;
				
		case Menu_Input:
			if(Menu_Refurbish >= 2000){
				Menu_Item = Menu_Measure;
				Menu_Refurbish = 0;
			}
			if((Key_State == Key_State_Short) || (Key_State == Key_State_Long)){
				if(Key_Value_Last == Key_Value_Teach){
					if(Key_State == Key_State_Long){
						Menu_Item = Menu_Measure;
					}else if(Input_Function == Input_Laser_Ctrl){
						Code_to_Buf(Menu_Display_Buf, Menu_Laser_Ctrl_Char, Display_Char_Num);
						Menu_Item = Menu_Laser_Ctrl;
					}else{ //if(Input_Function == Input_Off)
					
						Code_to_Buf(Menu_Display_Buf, Menu_Input_Off_Char, Display_Char_Num);
						Menu_Item = Menu_Input_Off;
					}
					Menu_Refurbish = 0;
				}else if(Key_Value_Last == Key_Value_Down){
					Code_to_Buf(Menu_Display_Buf, Menu_Reset_Char, Display_Char_Num);
					Menu_Item = Menu_Reset;
					Menu_Refurbish = 0;
				}else if(Key_Value_Last == Key_Value_Up){
					Code_to_Buf(Menu_Display_Buf, Menu_Aout_Scope_Char, Display_Char_Num);
					Menu_Item = Menu_Aout_Scope;
					Menu_Refurbish = 0;
				}
			}
			break;
				
		case Menu_Input_Off:
			if(Menu_Refurbish >= 2000){
				Code_to_Buf(Menu_Display_Buf, Menu_Input_Char, Display_Char_Num);
				Menu_Item = Menu_Input;
				Menu_Refurbish = 0;
			}
			if((Key_State == Key_State_Short) || (Key_State == Key_State_Long))
			{
				if(Key_Value_Last == Key_Value_Teach)
				{
					Laser_indicator_On();
					if(Input_Function != Input_Off)
					{
						Para_Save_flag = 1;
						Input_Function = Input_Off;
						DataBuffer2[12] = Input_Function;
					}
					Code_to_Buf(Menu_Display_Buf, Menu_Input_Char, Display_Char_Num);
					
					Menu_Item = Menu_Input;
					Menu_Refurbish = 0;
				}else if(Key_Value_Last == Key_Value_Down)
				{
					Code_to_Buf(Menu_Display_Buf, Menu_Laser_Ctrl_Char, Display_Char_Num);
					Menu_Item = Menu_Laser_Ctrl;
					Menu_Refurbish = 0;
				}else if(Key_Value_Last == Key_Value_Up)
				{
					Code_to_Buf(Menu_Display_Buf, Menu_Laser_Ctrl_Char, Display_Char_Num);
					Menu_Item = Menu_Laser_Ctrl;
					Menu_Refurbish = 0;
				}
			}
			break;
				
		case Menu_Laser_Ctrl:
			if(Menu_Refurbish >= 2000)
			{
				Code_to_Buf(Menu_Display_Buf, Menu_Input_Char, Display_Char_Num);
				Menu_Item = Menu_Input;
				Menu_Refurbish = 0;
			}
			if((Key_State == Key_State_Short) || (Key_State == Key_State_Long))
			{
				if(Key_Value_Last == Key_Value_Teach)
				{
					if(Input_Function != Input_Laser_Ctrl)
					{
						Para_Save_flag = 1;
						Input_Function = Input_Laser_Ctrl;
						DataBuffer2[12] = Input_Function;
					}
					Code_to_Buf(Menu_Display_Buf, Menu_Input_Char, Display_Char_Num);
					
					Menu_Item = Menu_Input;
					Menu_Refurbish = 0;
				}else if(Key_Value_Last == Key_Value_Down)
				{
					Code_to_Buf(Menu_Display_Buf, Menu_Input_Off_Char, Display_Char_Num);
					Menu_Item = Menu_Input_Off;
					Menu_Refurbish = 0;
				}else if(Key_Value_Last == Key_Value_Up)
				{
					Code_to_Buf(Menu_Display_Buf, Menu_Input_Off_Char, Display_Char_Num);
					Menu_Item = Menu_Input_Off;
					Menu_Refurbish = 0;
				}
			}
			break;
						
						
		case Menu_Reset:
			if(Menu_Refurbish >= 2000)
			{
				Menu_Item = Menu_Measure;
				Menu_Refurbish = 0;
			}
			if((Key_State == Key_State_Short) || (Key_State == Key_State_Long))
			{
				if(Key_Value_Last == Key_Value_Teach)
				{
					if(Key_State == Key_State_Long)
					{
						Menu_Item = Menu_Measure;
					}else
					{
						Code_to_Buf(Menu_Display_Buf, Menu_No_Char, Display_Char_Num);
						Menu_Item = Menu_Reset_No;
					}
					Menu_Refurbish = 0;
				}else if(Key_Value_Last == Key_Value_Down)
				{
					Code_to_Buf(Menu_Display_Buf, Menu_Mode_Char, Display_Char_Num);
					Menu_Item = Menu_Mode;
					Menu_Refurbish = 0;
				}else if(Key_Value_Last == Key_Value_Up)
				{
					Code_to_Buf(Menu_Display_Buf, Menu_Input_Char, Display_Char_Num);
					Menu_Item = Menu_Input;
					Menu_Refurbish = 0;
				}
			}
			break;
					
		case Menu_Reset_No:
			if(Menu_Refurbish >= 2000)
			{
				Code_to_Buf(Menu_Display_Buf, Menu_Reset_Char, Display_Char_Num);
				Menu_Item = Menu_Reset;
				Menu_Refurbish = 0;
			}
			if((Key_State == Key_State_Short) || (Key_State == Key_State_Long))
			{
				if(Key_Value_Last == Key_Value_Teach)
				{
					Code_to_Buf(Menu_Display_Buf, Menu_Reset_Char, Display_Char_Num);
					Menu_Item = Menu_Reset;
					Menu_Refurbish = 0;
				}else if(Key_Value_Last == Key_Value_Down)
				{
					Code_to_Buf(Menu_Display_Buf, Menu_Yes_Char, Display_Char_Num);
					Menu_Item = Menu_Reset_Yes;
					Menu_Refurbish = 0;
				}else if(Key_Value_Last == Key_Value_Up)
				{
					Code_to_Buf(Menu_Display_Buf, Menu_Yes_Char, Display_Char_Num);
					Menu_Item = Menu_Reset_Yes;
					Menu_Refurbish = 0;
				}
			}
			break;
						
		case Menu_Reset_Yes:
			if(Menu_Refurbish >= 2000)
			{
				Code_to_Buf(Menu_Display_Buf, Menu_Reset_Char, Display_Char_Num);
				Menu_Item = Menu_Reset;
				Menu_Refurbish = 0;
			}
			if((Key_State == Key_State_Short) || (Key_State == Key_State_Long))
			{
				if(Key_Value_Last == Key_Value_Teach)
				{
					//ִ�и�λ�������ָ�Ĭ��
					Init_Para_Factory();
					Para_Save_flag = 1;
					
					VI5300_Init();
					VI5300_Start_RefTof_Calibration(&stRefTof);
					DataBuffer2[11] = stRefTof.RefTof;
					VI5300_Config_RefTof(stRefTof.RefTof);
					VI5300_Start_Continuous_Measure(); 
					
					Menu_Item = Menu_Measure;
					Menu_Refurbish = 0;
				}else if(Key_Value_Last == Key_Value_Down)
				{
					Code_to_Buf(Menu_Display_Buf, Menu_No_Char, Display_Char_Num);
					Menu_Item = Menu_Reset_No;
					Menu_Refurbish = 0;
				}else if(Key_Value_Last == Key_Value_Up)
				{
					Code_to_Buf(Menu_Display_Buf, Menu_No_Char, Display_Char_Num);
					Menu_Item = Menu_Reset_No;
					Menu_Refurbish = 0;
				}
			}
			break;	

		case Menu_Aout_Adj1:
			if(Menu_Refurbish >= 50)
			{
				if(Key_Teach_Count == 0)
				{
					Code_to_Buf(Menu_Display_Buf, Menu_Aout_Adj1_Char, Display_Char_Num);
				}else if(Key_Teach_Count == 1){
					Int16_to_Display_Buf(Aout_Adjust1/10);
				}else{
					Int16_to_Display_Buf(Aout_Adjust1%10000);
				}		
				Menu_Refurbish = 0;
			}
			if(Key_State == Key_State_Short)
			{
				if(Key_Value_Last == Key_Value_Teach)
				{
					Key_Teach_Count++;
					Aout_Adj = 1;
					if(Key_Teach_Count >= 4)
					{
						Para_Save_flag = 1;
						Key_Teach_Count = 0;
						Aout_Adj = 0;
						DataBuffer2[9] = Aout_Adjust1;
					}
					Menu_Refurbish = 50;
				}else if(Key_Value_Last == Key_Value_Down){
					if(Key_Teach_Count == 0)
					{
						Code_to_Buf(Menu_Display_Buf, Menu_Aout_Adj2_Char, Display_Char_Num);
						Menu_Item = Menu_Aout_Adj2;
						Menu_Refurbish = 0;
						//Key_Teach_Count = 0;
					}else if(Key_Teach_Count == 1)
					{
						if(Aout_Adjust1 <= 65435)
						{
							Aout_Adjust1 += 100;
						}
					}else if(Key_Teach_Count == 2)
					{
						if(Aout_Adjust1 <= 65525)
						{
							Aout_Adjust1 += 10;
						}
					}else if(Key_Teach_Count == 3)
					{
						if(Aout_Adjust1 < 65535)
						{
							Aout_Adjust1 += 1;
						}
					}
				}else if(Key_Value_Last == Key_Value_Up)
				{
					if(Key_Teach_Count == 0)
					{
						Code_to_Buf(Menu_Display_Buf, Menu_Soft_Version_Char, Display_Char_Num);
						Menu_Item = Menu_Soft_Version;
						Menu_Refurbish = 0;
						//Key_Teach_Count = 0;
					}
					else if(Key_Teach_Count == 1)
					{
						if(Aout_Adjust1 >= 100)
						{
							Aout_Adjust1 -= 100;
						}
					}else if(Key_Teach_Count == 2)
					{
						if(Aout_Adjust1 >= 10){
							Aout_Adjust1 -= 10;
						}
					}else if(Key_Teach_Count == 3)
					{
						if(Aout_Adjust1 >= 1){
							Aout_Adjust1 -= 1;
						}
					}
				}
			}else if(Key_State == Key_State_Long)
			{
				if(Key_Value_Last == Key_Value_Teach_Down)
				{
					Menu_Item = Menu_Measure;
					Menu_Refurbish = 0;
					Key_Teach_Count = 0;
					Aout_Adj = 0;
				}
			}
			break;		
				
		case Menu_Aout_Adj2:
			if(Menu_Refurbish >= 50)
			{
				if(Key_Teach_Count == 0)
				{
					Code_to_Buf(Menu_Display_Buf, Menu_Aout_Adj2_Char, Display_Char_Num);
				}else if(Key_Teach_Count == 1){
					Int16_to_Display_Buf(Aout_Adjust2/10);
				}else{
					Int16_to_Display_Buf(Aout_Adjust2%10000);
				}				
				Menu_Refurbish = 0;
			}
			if(Key_State == Key_State_Short)
			{
				if(Key_Value_Last == Key_Value_Teach)
				{
					Key_Teach_Count++;
					Aout_Adj = 2;
					if(Key_Teach_Count >= 4){
						Para_Save_flag = 1;
						Key_Teach_Count = 0;
						Aout_Adj = 0;
						DataBuffer2[10] = Aout_Adjust2;
					}
					Menu_Refurbish = 50;
				}else if(Key_Value_Last == Key_Value_Down)
				{
					if(Key_Teach_Count == 0)
					{
						Code_to_Buf(Menu_Display_Buf, Menu_Calibration_Char, Display_Char_Num);
						Menu_Item = Menu_Calibration;
						Menu_Refurbish = 0;
						//Key_Teach_Count = 0;
					}else if(Key_Teach_Count == 1)
					{
						if(Aout_Adjust2 <= 65435)
						{
							Aout_Adjust2 += 100;
						}
					}else if(Key_Teach_Count == 2)
					{
						if(Aout_Adjust2 <= 65525)
						{
							Aout_Adjust2 += 10;
						}
					}else if(Key_Teach_Count == 3)
					{
						if(Aout_Adjust2 < 65535)
						{
							Aout_Adjust2 += 1;
						}
					}
				}else if(Key_Value_Last == Key_Value_Up)
				{
					if(Key_Teach_Count == 0)
					{
						Code_to_Buf(Menu_Display_Buf, Menu_Aout_Adj1_Char, Display_Char_Num);
						Menu_Item = Menu_Aout_Adj1;
						Menu_Refurbish = 0;
						//Key_Teach_Count = 0;
					}else if(Key_Teach_Count == 1)
					{
						if(Aout_Adjust2 >= 100){
							Aout_Adjust2 -= 100;
						}
					}else if(Key_Teach_Count == 2)
					{
						if(Aout_Adjust2 >= 10){
							Aout_Adjust2 -= 10;
						}
					}else if(Key_Teach_Count == 3)
					{
						if(Aout_Adjust2 >= 1){
							Aout_Adjust2 -= 1;
						}
					}
				}
			}else if(Key_State == Key_State_Long)
			{
				if(Key_Value_Last == Key_Value_Teach_Down)
				{
					Menu_Item = Menu_Measure;
					Menu_Refurbish = 0;
					Key_Teach_Count = 0;
					Aout_Adj = 0;
				}
			}
			break;				
						
		case Menu_Calibration:
			if(Menu_Refurbish >= 200)
			{
				Menu_Refurbish = 0;
				if(Key_Teach_Count == 0)
				{
					Code_to_Buf(Menu_Display_Buf, Menu_Calibration_Char, Display_Char_Num);
				}else{
					Distance2_to_Display_Buf();
				}
			}
			if(Key_State == Key_State_Short)
			{
				if(Key_Value_Last == Key_Value_Teach)
				{
					if(Key_Teach_Count == 0)
					{
						caclflag = 0;
						Int16_to_Display_Buf(0);
						Menu_Display_Buf[0] = 0x73;         //"P"

						Key_Teach_Count++;
					}else{
						Calib_DisBuf[Key_Teach_Count-1] = Distance_teach;
						Key_Teach_Count++;
						if(Key_Teach_Count == 2)
						{
							Int16_to_Display_Buf(10);
							Menu_Display_Buf[0] = 0x73;         //"P"
						}else{
							if(Distance_teach > Calib_DisBuf[Key_Teach_Count-3])
							{
								if(Key_Teach_Count <= 6)
								{
									Int16_to_Display_Buf(10*(Key_Teach_Count-1));
									Menu_Display_Buf[0] = 0x73;         //"P"
								}else if(Key_Teach_Count <= 14)
								{
									Int16_to_Display_Buf(50*(Key_Teach_Count-5));
									Menu_Display_Buf[0] = 0x73;         //"P"
								}else
								{
									Code_to_Buf(Menu_Display_Buf, Menu_Success_Char, Display_Char_Num);
									Key_Teach_Count = 0;
									caclflag = 1;
									//Calib_DisBuf[14] = caclflag;
									Calib_DisBuf[18] = caclflag;
									Calib_kb();
									Flash_CaclDate_Write();
								}
							}else
							{
								Code_to_Buf(Menu_Display_Buf, Menu_Fail_Char, Display_Char_Num);
								Key_Teach_Count = 0;
							}
						}
					}
					Menu_Refurbish = 0;
				}else if(Key_Value_Last == Key_Value_Down)
				{
					if(Key_Teach_Count == 0)
					{
						Code_to_Buf(Menu_Display_Buf, Menu_Soft_Version_Char, Display_Char_Num);
						Menu_Item = Menu_Soft_Version;
						Menu_Refurbish = 0;
					}
				}else if(Key_Value_Last == Key_Value_Up)
				{
					if(Key_Teach_Count == 0)
					{
						Code_to_Buf(Menu_Display_Buf, Menu_Aout_Adj2_Char, Display_Char_Num);
						Menu_Item = Menu_Aout_Adj2;
						Menu_Refurbish = 0;
					}
				}
			}
			else if(Key_State == Key_State_Long)
			{
				if(Key_Value_Last == Key_Value_Teach_Down)
				{
					Menu_Item = Menu_Measure;
					Menu_Refurbish = 0;
					Key_Teach_Count = 0;
				}
			}
			break;		

		case Menu_Soft_Version:
			if(Menu_Refurbish >= 50)
			{
				Code_to_Buf(Menu_Display_Buf, Menu_Soft_Version_Char, Display_Char_Num);
				Menu_Refurbish = 0;
			}
			if(Key_State == Key_State_Short)
			{
				if(Key_Value_Last == Key_Value_Down)
				{
					Code_to_Buf(Menu_Display_Buf, Menu_Aout_Adj1_Char, Display_Char_Num);
					Menu_Item = Menu_Aout_Adj1;
					Menu_Refurbish = 0;
				}
				else if(Key_Value_Last == Key_Value_Up)
				{
					Code_to_Buf(Menu_Display_Buf, Menu_Calibration_Char, Display_Char_Num);
					Menu_Item = Menu_Calibration;
					Menu_Refurbish = 0;
				}
			}
			else if(Key_State == Key_State_Long)
			{
				if(Key_Value_Last == Key_Value_Teach_Down)
				{
					Menu_Item = Menu_Measure;
					Menu_Refurbish = 0;
					Key_Teach_Count = 0;
				}
			}
			break;		
									
		default:	            
        	break;						
	}
		
	if(Key_State == Key_State_Short)
    {
        Teaching_Count = 0;
        Key_State = Key_State_Wait;
        Key_Value_Last = Key_Value_Null;
        Key_Pushing_Count = 0;
    }
    if(Key_State == Key_State_Long)
    {
        Teaching_Count = 0;
        Key_State = Key_State_Long_next;
        Key_Pushing_Count = 0;
    }
		
	if(Menu_Refurbish == 0)
    {
        if((Menu_Item == Menu_Measure) || (Menu_Item == Menu_Mode_Teaching))
        {
            Distance_to_Display_Buf();
        }
        
        Menu_Display();
    }
    Menu_Refurbish++;
	//Menu_Refurbish++;
}


void Check_Dout_State(void)
{
	uint8_t d_flag = 0;
	uint16_t distance;
	uint16_t band;
	uint8_t i;

	if(IO_Out_Speed == IO_Out_Speed_Standard)
	{
		D_or_A_monitor_times = 2;
	}
	else //if(IO_Out_Speed == IO_Out_Speed_Accurate)
	{
		D_or_A_monitor_times = 4;
	}

	distance = Vi5300_distance;
	if(distance < 60)//��߽�ز��趨
	{
		 band = 5;
	}
	else if(distance < 200)
	{
		 band = 10;
	}		
	else if(distance < 1000)
	{
		 band = 20;
	}	
	else if(distance < 3000)
	{
		 band = 30;
	}
	else
	{
		 band = 50;
	}

	if(IO_Out_Reverse == IO_Out_Dark)
	{
		if(Dout_State == Dout_On)
		{
			d_flag = Dout_Off;
		}
		else
		{
			d_flag = Dout_On;
		}
	}
	else
	{
		d_flag = Dout_State;
	}

	if((Teach_Mode == Teach_Mode_Normal) || (Teach_Mode == Teach_Mode_Mid))
	{
		if(distance < Teach_Point[0])
		{
			d_flag = Dout_On;
		}
		else if((distance > (Teach_Point[0]+band)) || (distance > Distance_Scope))
		{
			d_flag = Dout_Off;
		}
	}
	else //if(Teach_Mode == Teach_Mode_2P)
	{
		if((distance > Teach_Point[0]) && (distance < Teach_Point[1]))
		{
			d_flag = Dout_On;
		}
		else if((distance < (Teach_Point[0]-band)) || (distance > (Teach_Point[1]+band)) || (distance > Distance_Scope))
		{
			d_flag = Dout_Off;
		}
	}

	if(IO_Out_Reverse == IO_Out_Dark)
	{
		if(d_flag == Dout_On)
		{
			d_flag = Dout_Off;
		}
		else
		{
			d_flag = Dout_On;
		}
	}

	if(Dout_State_Buf_P >= D_or_A_monitor_times)
	{
		Dout_State_Buf_P = 0;
	}
	Dout_State_Buf[Dout_State_Buf_P] = d_flag;
	Dout_State_Buf_P++;

	for(i=0;i<D_or_A_monitor_times;i++)
	{
		if(Dout_State == Dout_State_Buf[i])
		{
			break;
		}
	}
	if(i == D_or_A_monitor_times)
	{
		Dout_State = Dout_State_Buf[0];
	}		
}

void Digital_Output(void)
{
	Check_Dout_State();
	
	if(IO_Out_Type == IO_Out_NPN)
    {
        Dout_Select_NPN();
    }
    else if(IO_Out_Type == IO_Out_PNP)
    {
        Dout_Select_PNP();
    }
    else //if(IO_Out_Type == IO_Out_Pull)
    {
        Dout_Switch_On();
    }
    
    if(IO_Out_Type == IO_Out_Pull)
    {
        if(Dout_State == Dout_On)
        {
			Dout_Select_NPN();
			IO_Led_On();
        }
        else
        {
			Dout_Select_PNP();
			IO_Led_Off();
        }
    }
    else
    {
        if(Dout_State == Dout_On)
        {
			Dout_Switch_On();
			IO_Led_On();
        }
        else
        {
			Dout_Switch_Off();
			IO_Led_Off();
        }
    }
}


void Aout_Select_10V(void){
#if AOUT_USE_SPI
	AD5412_V_10v();
#elif AOUT_USE_I2C
    Aout_I2C_Select_10V();
#endif
}
	

void Aout_Send_Data(uint16_t dat)
{
#if AOUT_USE_SPI
	AD5412_Data(dat);
#elif AOUT_USE_I2C
    Aout_I2C_Send_Data(dat);
#endif
}


void Account_Aout_K_and_B(void)
{
#if (Aout_Mode == Vout_Mode)
	K_Aout = (float)(Aout_Adjust2 - Aout_Adjust1) / 9800;
	B_Aout = -K_Aout * 200 + Aout_Adjust1;
	if(B_Aout < 0)
	{
		Aout_min = 0;
	}
	else
	{
		Aout_min = (uint16_t)B_Aout;
	}
	Aout_max = Aout_Adjust2;
	Aout_External = Aout_Adjust2 + Aout_Adjust2/50;
#else
	K_Aout = (float)(Aout_Adjust2 - Aout_Adjust1) / 16;
	B_Aout = Aout_Adjust1;
	Aout_min = Aout_Adjust1;
	Aout_max = Aout_Adjust2;
#endif
}

void Analog_Output(void)
{
	uint16_t distance;
	float tempf;
	uint16_t Aout_x;
	uint16_t L1,L2;
	uint16_t L1_band,L2_band;
	
	if(Aout_Adj == 1)
	{
		Aout_Send_Data(Aout_Adjust1);
		Account_Aout_K_and_B();
		return;
	}
	if(Aout_Adj == 2)
	{
		Aout_Send_Data(Aout_Adjust2);
		Account_Aout_K_and_B();
		return;
	}
	
	if(IO_Out_Speed == IO_Out_Speed_Standard)
	{
		distance = Vi5300_distance;
	}
	else
	{
		distance = Distance_analog;
	}
	
	if(Aout_Scope == Aout_Full)
	{
		L1 = 50;
		L2 = Distance_Teach;
		/*
		if(Distance_Scope < 4000)
		{
			L2 = Distance_Scope;
		}
		else
		{
			L2 = 4000;
		}
		
		if(distance < 50)		//0-50mm, 4mA
		{
			distance = 50;
		}
		if(distance > L2)		//>4000mm, 20mA
		{
			distance = L2;
		}
		*/
	}
	else
	{
		L1 = Teach_Point[0];
		L2 = Teach_Point[1];
	}
	L1_band = L1/266 + 5;
	L2_band = L2/266 + 5;
	
	if(Aout_Direction == Aout_Forward)
	{
		if((distance < (L1-L1_band)) || (distance > (L2+L2_band)))
		{
			Aout_x = Aout_External;
		}
		else if(distance < L1)
		{
			Aout_x = Aout_min;
		}
		else if(distance > L2)
		{
			Aout_x = Aout_max;
		}
		else
		{
			tempf = (float)Aout_Range * (distance - L1)/(L2-L1);
			tempf = K_Aout * tempf + B_Aout;
			if(tempf < Aout_min)
			{
				Aout_x = Aout_min;
			}
			else if(tempf > Aout_max)
			{
				Aout_x = Aout_max;
			}
			else
			{
				Aout_x = (uint16_t)tempf;
			}
		}
	}
	else //if(Aout_Direction == Aout_Reverse)
	{
		if((distance < (L1-L1_band)) || (distance > (L2+L2_band)))
		{
			Aout_x = Aout_External;
		}
		else if(distance < L1)
		{
			Aout_x = Aout_max;
		}
		else if(distance > L2)
		{
			Aout_x = Aout_min;
		}
		else
		{
			tempf = (float)Aout_Range * (L2 - distance)/(L2-L1);
			tempf = K_Aout * tempf + B_Aout;
			if(tempf < Aout_min)
			{
				Aout_x = Aout_min;
			}
			else if(tempf > Aout_max)
			{
				Aout_x = Aout_max;
			}
			else
			{
				Aout_x = (uint16_t)tempf;
			}
		}
	}	
	Aout_Send_Data(Aout_x);
}

void Aout_test(void)
{
	static uint16_t interval = 0;
	static uint8_t step = 0;
	
	interval++;
	if(interval < 800){return;}
	interval = 0;
	
	if(step > 10)
	{
		step = 0;
	}
	Aout_Send_Data(0x7fff/10*step);	
	step++;
}

void Digital_Input(void)
{
	if(Input_Function == Input_Laser_Ctrl)
    {
        if(Read_IO_Input())    
        {
            Laser_indicator_Off();
        }
        else
        {
            Laser_indicator_On();
        }
    }	
}

