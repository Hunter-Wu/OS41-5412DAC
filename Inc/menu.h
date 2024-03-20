


#define Vout_Mode								1
#define Iout_Mode								2
#define Aout_Mode								Iout_Mode

#if (Aout_Mode == Vout_Mode)
	#define D5351_200mv_Default    				655
	#define D5351_10v_Default    				32767
	#define Aout_Range							10000
	#define D5351_10v2_Default    				33422
	#define Adj1_Value							200
	#define Adj2_Value							10000
	#define Adj_Shake							3
	#define Adj1_Max							400
	#define Adj1_Min							0
	#define Adj2_Max							11000
	#define Adj2_Min							9000
#else
	#define D5351_4mA_Default     				5243
	#define D5351_20mA_Default    				26134
	#define Aout_Range							16
	#define D5351_0mA_Default    				0
	#define Adj1_Value							4000
	#define Adj2_Value							20000
	#define Adj_Shake							4
	#define Adj1_Max							4400
	#define Adj1_Min							3600
	#define Adj2_Max							22000
	#define Adj2_Min							18000
#endif

#define Flash_para_num							13

#define Distance_Scope							4500
#define Distance_Teach							4000
//#define Distance_Scope						2550
//#define Distance_Teach						2500
#define Distance_Buf_len						4
#define Distance_display_Buf_len				10
#define Display_Char_Num        				4
#define Dout_State_Buf_len						4



extern uint8_t Aout_Direction;
extern uint8_t Aout_Scope;
extern uint8_t Teach_Mode;
extern uint16_t Teach_Point[3];
extern uint8_t IO_Out_Speed;
extern uint8_t IO_Out_Reverse;
extern uint8_t IO_Out_Type;
extern uint16_t D5351_Adjust1;
extern uint16_t D5351_Adjust2;
extern uint8_t Input_Function;

extern uint8_t Menu_Display_Buf[Display_Char_Num];


//----------------------------------------------------------------------

//Key
#define Key_Value_Null          0x00
#define Key_Value_Down          0x01
#define Key_Value_Up            0x02
#define Key_Value_Teach         0x04

#define Key_Value_Up_Down       0x03
#define Key_Value_Teach_Down    0x05
#define Key_Value_Teach_Up      0x06
#define Key_Value_All           0x07

#define Key_State_Wait          0x00
#define Key_State_Pushing       0x01
#define Key_State_Short         0x02
#define Key_State_Long          0x03
#define Key_State_Long_next     0x04

#define Key_Short_Delay         2
//#define Key_Long_Delay          200
#define Key_Long_Delay          150
//#define Key_Long_Delay          100



//Menu
#define Menu_Measure            0x00

#define Menu_Mode               0x10
#define Menu_Mode_Normal        0x11
#define Menu_Mode_2P            0x12
#define Menu_Mode_Mid           0x13

#define Menu_Mode_Teaching      0x1B
#define Menu_Teach_Success      0x1C
#define Menu_Teach_Fail         0x1D
#define Menu_Teach_Step         0x1E

#define Menu_Speed              0x20
#define Menu_Speed_Accurate     0x21
#define Menu_Speed_Standard     0x22

#define Menu_L_or_D             0x28
#define Menu_Light              0x29
#define Menu_Dark               0x2A

#define Menu_IO_Out             0x40
#define Menu_NPN_Out            0x41
#define Menu_PNP_Out            0x42
#define Menu_Pull_Out           0x43

/*
#define Menu_Aout_Mode          0x48
#define Menu_Iout_Mode          0x49
#define Menu_Vout_Mode          0x4A
*/

#define Menu_Aout_Direction     0x50
#define Menu_Aout_Forward       0x51
#define Menu_Aout_Reverse       0x52

#define Menu_Aout_Scope         0x58
#define Menu_Aout_Full          0x59
#define Menu_Aout_Window        0x5A

#define Menu_Input              0x70
#define Menu_Input_Off          0x71
#define Menu_Laser_Ctrl         0x72


/*
#define Menu_Adr485             0x74
#define Menu_Adr485_Para        0x75

#define Menu_Baud               0x76
#define Menu_Baud_Para          0x77

#define Menu_Mode485            0x78
#define Menu_Mode485_Para       0x79
*/

#define Menu_Reset              0x80
#define Menu_Reset_No           0x81
#define Menu_Reset_Yes          0x82


#define Menu_Soft_Version       0xF0
#define Menu_Aout_Adj1        	0xF1
#define Menu_Aout_Adj2        	0xF2
#define Menu_Calibration   		0xF3


//Para
#define Teach_Mode_Normal       0x00
#define Teach_Mode_2P           0x01
#define Teach_Mode_Mid          0x02

#define IO_Out_Speed_Accurate   0x00
#define IO_Out_Speed_Standard   0x01

#define IO_Out_Light            0x00
#define IO_Out_Dark             0x01

#define IO_Out_NPN              0x00
#define IO_Out_PNP              0x01
#define IO_Out_Pull             0x02

/*
#define Iout_Mode               0x00
#define Vout_Mode               0x01
*/

#define Aout_Forward            0x00
#define Aout_Reverse            0x01

#define Aout_Full               0x00
#define Aout_Window             0x01

#define Input_Off               0x00
#define Input_Laser_Ctrl        0x01

/*
#define Baud_2400               0x00
#define Baud_4800               0x01
#define Baud_9600               0x02
#define Baud_19200              0x03
#define Baud_38400              0x04
#define Baud_115200             0x05

#define Mode_485_Series         0x00
#define Mode_485_Polling        0x01
*/

//-------------------------------------------------------------------------------
#define Dout_Off      0
#define Dout_On       1

void Get_Distance_avg(void);
void Uart_Send_Display_Buf(void);
void Menu_Key_Operate(void);
void Digital_Output(void);
void Distance_display_Buf_Init(void);

void Aout_Select_10V(void);
void Account_Aout_K_and_B(void);
void Analog_Output(void);
void Aout_test(void);
void Digital_Input(void);

