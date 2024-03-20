#include "main.h"		 
#include "user_init.h"
#include "fm33lc0xx_fl.h"  	 
#include "VI5300_API.h"
#include "VI5300_Config.h"
#include "VI5300_Firmware.h"
#include "string.h"
#include "vi_sw_i2c.h"

#include "VI5300_UserPlatform.h"

#define PEAK_THRESHOLD 2400
#define SNR_THRESHOLD 80

uint8_t gSalve=VI5300_DEVICE_ADDR,chip_reg=0,chip_reg38,chip_reg3a;
VI5300_OFFSET_Calib_Data  offset_data;
VI5300_XTALK_Calib_Data  xtalk_data;

uint8_t VI5300_Read_OTP(uint8_t base, uint8_t size, uint8_t* out)
{
	WriteOneReg(0x0c, 0x03);
	WriteOneReg(0x0d,  size);
	WriteOneReg(0x0e,  base);
	WriteOneReg(0x0A, 0x09);
	HAL_Delay(5);
	I2C_ReadXBytes(0x0F, out, size);

	return 0;
}
void VI5300_Chip_PowerON1(void)
{
	//HAL_GPIO_WritePin(XSHUT_GPIO_Port, pin, GPIO_PIN_RESET);//set 0
	//HAL_Delay(5);//delay 30ms
	//HAL_GPIO_WritePin(XSHUT_GPIO_Port, pin, GPIO_PIN_SET);//set 1
	//HAL_Delay(20);
	FL_GPIO_ResetOutputPin(VL53L1X_XSHUT_GPIO_Port, VL53L1X_XSHUT_Pin);
	FL_DelayMs(10);
	FL_GPIO_SetOutputPin(VL53L1X_XSHUT_GPIO_Port, VL53L1X_XSHUT_Pin);
	FL_DelayMs(20);
}
uint8_t VI5300_Device_Check(void)
{
	uint8_t Chip_id = 0;
	
	ReadOneReg(0x06, &Chip_id);
	return Chip_id;
	
}
#ifdef CHANGE_MODEL_ADDRESS
uint8_t VI5300_Device_Adress_Modify(uint16_t pin,uint8_t addr_val)
{	
	uint8_t err_sta = 0x00;
	
	VI5300_Chip_PowerON1();
	if(VI5300_Device_Check()!=0xD8)
	{
		//printf("Check device ID fail!\r\n");
		return 0x01; //返回错误状态
	}

	err_sta = WriteOneReg(0x01,0x88);
	if(err_sta!=0x00)
	{
		return 0x01; //返回错误状态
	} 
	err_sta = WriteOneReg(0x06,addr_val);
	if(err_sta!=0x00)
	{
		return 0x02; //返回错误状态
	} 
	gSalve = addr_val;
	VI5300_Device_Check();
	return 0x00;
}
#endif  

VI5300_Status VI5300_Chip_Register_Init_Reg(void)
{
	VI5300_Status ret = VI5300_OK;

	ret |= WriteOneReg(0x00,0x00);
	ret |= WriteOneReg(0x01,0x0c);
	ret |= WriteOneReg(0x07,0x00);
	ret |= WriteOneReg(0x07,0x01);
	ret |= WriteOneReg(0x07,0x00);
	ret |= WriteOneReg(0x04,0x21);
	ret |= WriteOneReg(0x05,0x0E);
	ret |= WriteOneReg(0x08,0x00);
	ret |= WriteOneReg(0x37,0x80);
	if(chip_reg==0x00)
	{
		ret |= WriteOneReg(0x38,0x00);
		ret |= WriteOneReg(0x3A,0x00);
	}
	else if(chip_reg==0x30)
	{
		ret |= WriteOneReg(0x38,0x30);
		ret |= WriteOneReg(0x3A,0x30);
	}
	ret |= WriteOneReg(0x39,0x00);
	ret |= WriteOneReg(0x3B,0x80);
	ret |= WriteOneReg(0x3C,0x80);
	ret |= WriteOneReg(0x3D,0x80);
	ret |= WriteOneReg(0x3E,0x00);
	ret |= WriteOneReg(0x3F,0x00);
	ret |= WriteOneReg(0x07,0x0E);
	ret |= WriteOneReg(0x07,0x0F);
	VI5300_CHECK_RET(ret);

	return ret;
}

uint8_t VI5300_Wait_For_CPU_Ready(void)
{
	VI5300_Status Status = VI5300_OK;
	uint8_t stat;
	int retry = 0;

	do 
	{
		HAL_Delay(1);//delay 1ms
		Status = ReadOneReg(0x02, &stat);
	}while((retry++ < 20)&&(stat & 0x01));
	if(retry >= 20)
	{
		//printf("CPU Busy stat = %d\n", stat);
		return 1;
	}
	return Status;
}

/********************************************************/
/*******************FOR VI5300 V3.0 20210129**********************/
VI5300_Status VI5300_Set_Digital_Clock_Dutycycle(void)
{
    VI5300_Status ret = VI5300_OK;
    
    ret |= WriteOneReg(VI5300_REG_PW_CTRL, 0x0F);
	ret |= WriteOneReg(VI5300_REG_PW_CTRL, 0x0E);
	if(chip_reg==0x00)
	{
		HAL_Delay(1);
		ret |= WriteOneReg(0xE9, 0x24);
		ret |= WriteOneReg(0xEE, 0x00);      //ADJUST TEMP DRIFT
		ret |= WriteOneReg(0xF5, 0x00);	
	}
	VI5300_CHECK_RET(ret);
	HAL_Delay(4);//默认4ms ，延时不准可以增加
    
    return ret;  
}
/*******************FOR VI5300 V3.0 20210129**********************/
/********************************************************/
 uint8_t reg_sys_cfg = 0;
VI5300_Status VI5300_WriteFirmwarePreConfig(void)
{
    VI5300_Status ret = VI5300_OK;

    VI5300_Set_Digital_Clock_Dutycycle();       // set rco config add 20210129
    
    ret |= WriteOneReg(VI5300_REG_PW_CTRL, 0x08);
    ret |= WriteOneReg(VI5300_REG_PW_CTRL, 0x0a);
    ret |= WriteOneReg(VI5300_REG_MCU_CFG, 0x06); //0x02  
    ret |= ReadOneReg (VI5300_REG_SYS_CFG, &reg_sys_cfg); 
    ret |= WriteOneReg(VI5300_REG_SYS_CFG, reg_sys_cfg | (0x01<<0));
	if(chip_reg==0x00)
	{
		ret |= WriteOneReg(0x38, 0x30);
		ret |= WriteOneReg(0x3A, 0x30);
	}
    ret |= WriteOneReg(VI5300_REG_CMD, 0x01);
    ret |= WriteOneReg(VI5300_REG_SIZE, 0x02); 
    ret |= WriteOneReg(VI5300_REG_SCRATCH_PAD_BASE+0x00, 0x00);
    ret |= WriteOneReg(VI5300_REG_SCRATCH_PAD_BASE+0x01, 0x00);
    VI5300_CHECK_RET(ret);

    return VI5300_OK;
}

VI5300_Status VI5300_WriteFirmwarePostConfig(void)
{
    VI5300_Status ret = VI5300_OK;

    ret |= WriteOneReg(VI5300_REG_SYS_CFG, 0x0c);//reg_sys_cfg & ~(0x01 << 0)
    ret |= WriteOneReg(VI5300_REG_MCU_CFG, 0x06);
    ret |= WriteOneReg(0x3B, 0xA0);
    ret |= WriteOneReg(0x3B, 0x80);
	if(chip_reg==0x00)
	{
		ret |= WriteOneReg(0xE9, 0x24);
		ret |= WriteOneReg(0xEE, 0x00);         
		ret |= WriteOneReg(0xF5, 0x00);
	}
    ret |= WriteOneReg(VI5300_REG_MCU_CFG, 0x07);  //0x03 
    ret |= WriteOneReg(VI5300_REG_PW_CTRL, 0x02); 
    ret |= WriteOneReg(VI5300_REG_PW_CTRL, 0x00); 
    VI5300_CHECK_RET(ret);  

    return VI5300_OK;
}

VI5300_Status VI5300_WriteFirmware32Byte(uint8_t len, uint8_t *data)
{
    VI5300_Status ret = VI5300_OK;
    uint8_t cmd_size[2];

    if(len > 32)
        return VI5300_ERROR;
    cmd_size[0] = VI5300_WRITEFW_CMD;
    cmd_size[1] = len;
    ret |= I2C_WriteXBytes(VI5300_REG_CMD, cmd_size, 2);
    ret |= I2C_WriteXBytes(VI5300_REG_SCRATCH_PAD_BASE, data, len);
    VI5300_CHECK_RET(ret);

    return VI5300_OK; 
}

VI5300_Status VI5300_DownloadFirmware(uint8_t *buf, uint16_t size)
{
    VI5300_Status ret = VI5300_OK;
    uint8_t page = 0;

    ret |= VI5300_WriteFirmwarePreConfig();
    while(size >= 32)
    {
        ret |= VI5300_WriteFirmware32Byte(32, buf + page * 32);
        size -= 32;
        page++;
    }
    if(size > 0)
    {
        ret |= VI5300_WriteFirmware32Byte(size, buf + page * 32);
    }
    ret |= VI5300_WriteFirmwarePostConfig();
    VI5300_CHECK_RET(ret);
    return VI5300_OK;
}


/*****************
download firmware
******************/

void VI5300_Init_Firmware(void)
{
	VI5300_DownloadFirmware((uint8_t*)Firmware_Ranging, LoadFirmware());  
}
/***********add the function of setting integral counts and frame counts 20210105*********/
VI5300_Status VI5300_Integral_Counts_Write(uint32_t inte_counts)
{
	VI5300_Status ret = VI5300_OK;
	uint8_t buf[4]={0};
	
	//小端模式，从小到大保存		
	buf[0] = (inte_counts)&0xFF;
	buf[1] = (inte_counts>>8)&0xFF;
	buf[2] = (inte_counts>>16)&0xFF;
	/*******************FOR VI5300 V3.0**********************/
    /********************************************************/
    VI5300_Set_Digital_Clock_Dutycycle();       // set rco config add 20210129
    /*******************FOR VI5300 V3.0**********************/
    /********************************************************/
	ret |= WriteOneReg(0x0C, 0x01);
	ret |= WriteOneReg(0x0D, 0x03);
	ret |= WriteOneReg(0x0E, 0x01);
	ret |= WriteOneReg(0x0F, buf[0]);	
	ret |= WriteOneReg(0x10, buf[1]);
	ret |= WriteOneReg(0x11, buf[2]);
	ret |= WriteOneReg(0x0A, 0x09);
	VI5300_CHECK_RET(ret);

	return ret;
}
VI5300_Status VI5300_Delay_Count_Write(uint16_t delay_count)
{
	VI5300_Status ret = VI5300_OK;
	uint8_t buf[2]={0};
	
		//大端模式
	buf[0] = (delay_count>>8)&0xFF;
	buf[1] = (delay_count)&0xFF;
	VI5300_Set_Digital_Clock_Dutycycle();       // set rco config add 20210129
	ret |= WriteOneReg(0x0C, 0x01);
	ret |= WriteOneReg(0x0D, 0x02);
	ret |= WriteOneReg(0x0E, 0x04);
	//大端模式
	ret |= WriteOneReg(0x0F, buf[0]);
	ret |= WriteOneReg(0x10, buf[1]);
	ret |= WriteOneReg(0x0A, 0x09);
	VI5300_CHECK_RET(ret);
	
	return ret;
}
VI5300_Status VI5300_Set_Integralcounts_Frame(uint8_t fps, uint32_t intecoutns)
{
	VI5300_Status ret = VI5300_OK;
	uint32_t inte_time;
	uint32_t fps_time;
	uint32_t delay_time;
	uint16_t delay_counts;

	//inte_time = intecoutns *1463/10;
	inte_time = intecoutns *3333/10;						  
	fps_time = 1000000000/fps;
	delay_time = fps_time - inte_time -1600000;
	delay_counts = (uint16_t)(delay_time/40900);

	ret |= VI5300_Integral_Counts_Write(intecoutns);
	ret |= VI5300_Delay_Count_Write(delay_counts);
	VI5300_CHECK_RET(ret);

	return ret;
}
/***********add the function of setting integral counts and frame counts 20210105*********/
#ifdef VI5300_XTALK_CALIBRATION
/***********add the function of starting xtalk calibration 20210105*** ******/
//在测距之前执行xtalk标定，标定结束之后将结果存储，在每次测距之前将调用VI5300_Config_XTalk_Parameter（）
//函数将xtalk参数xtalk_cal.postion配置进芯片，芯片不下电，无需重新配置。

/***********add the function of xtalk  calibration 20210119**** *****/
VI5300_Status VI5300_Config_XTalk_Parameter(uint8_t xtalk_para)
{
		VI5300_Status ret = VI5300_OK;
		
		VI5300_Wait_For_CPU_Ready();
		VI5300_Set_Digital_Clock_Dutycycle();       // set rco config add 20210129

		ret |= WriteOneReg(0x0C, 0x01);
		ret |= WriteOneReg(0x0D, 0x01);
		ret |= WriteOneReg(0x0E, 0x00);
		ret |= WriteOneReg(0x0F, xtalk_para);
		ret |= WriteOneReg(0x0A, 0x09);
		VI5300_CHECK_RET(ret);
		
		return ret;
}
/***********add the function of starting xtalk calibration 20210105**** *****/

VI5300_Status VI5300_Start_XTalk_Calibration(VI5300_XTALK_Calib_Data *xtalk)
{
		VI5300_Status ret = VI5300_OK;
		uint8_t status,retry=0;
		uint8_t cg_buf[3];
		
		VI5300_Wait_For_CPU_Ready();
		VI5300_Set_Digital_Clock_Dutycycle();       // set rco config add 20210129

		#ifdef VI5300_TEMP_CALIBRATION
		do
		{
				VI5300_Temp_Enable(0);
				VI5300_Check_Temp_Enable(&status);
		}while((status!=0)&&(retry++<5));
		HAL_Delay(5);
		#endif
		WriteOneReg(0x0A, 0x0D);
		HAL_Delay(400);
		while(1) 
		{
				ReadOneReg(VI5300_RET_INT_STATUS, &status);
				if((status & 0x01) == 0x01)
				{
					//缓存都是小端模式
						I2C_ReadXBytes(VI5300_REG_SCRATCH_PAD_BASE, cg_buf, 3);
						xtalk->xtalk_cal = (int8_t)cg_buf[0];
						xtalk->xtalk_peak = (uint16_t)((((uint16_t)cg_buf[2])<<8) |(( (uint16_t)cg_buf[1])));
						break;
				}
				HAL_Delay(50);
		}
		#ifdef VI5300_TEMP_CALIBRATION
		do
		{
				VI5300_Temp_Enable(1);
				VI5300_Check_Temp_Enable(&status);
		}while((status!=1)&&(retry++<5));
		HAL_Delay(5);
		#endif
		return ret;
}
#endif
#ifdef VI5300_TEMP_CALIBRATION
VI5300_Status VI5300_Temp_Enable(uint8_t enable)
{
	VI5300_Status ret = VI5300_OK;  

	ret |= WriteOneReg(0x0C, 0x01);
	ret |= WriteOneReg(0x0D, 0x01);
	ret |= WriteOneReg(0x0E, 0x0E);
	ret |= WriteOneReg(0x0F, enable);
	ret |= WriteOneReg(0x0A, 0x09);
	VI5300_CHECK_RET(ret);
	HAL_Delay(5);
	return ret;
}
VI5300_Status VI5300_Check_Temp_Enable(uint8_t *enable)
{
	VI5300_Status ret = VI5300_OK;
	uint8_t tmp=0;

	ret |= WriteOneReg(0x0C, 0x00);
	ret |= WriteOneReg(0x0D, 0x01);
	ret |= WriteOneReg(0x0E, 0x0E);
	ret |= WriteOneReg(0x0A, 0x09);
	HAL_Delay(5);
	ret |= ReadOneReg(0x0c, &tmp);
	VI5300_CHECK_RET(ret);
	*enable = tmp;
	return ret;
}
VI5300_Status VI5300_Config_RefTof_Parameter(int16_t refTof)
{
	VI5300_Status ret = VI5300_OK;
	uint8_t tmp_tof;

	VI5300_Wait_For_CPU_Ready();
	VI5300_Set_Digital_Clock_Dutycycle();
	ret |= WriteOneReg(0x0C, 0x01);//write 
	ret |= WriteOneReg(0x0D, 0x02);	
	ret |= WriteOneReg(0x0E, 0x17);
	tmp_tof=refTof>>8;
	ret |= WriteOneReg(0x0F,tmp_tof);
	tmp_tof=refTof&0xFF;
	ret |= WriteOneReg(0x10,tmp_tof); 
	ret |= WriteOneReg(0x0A, 0x09);
	VI5300_CHECK_RET(ret);

	return ret;
}
VI5300_Status VI5300_Check_RefTof_Parameter(int16_t *refTof)
{
	VI5300_Status ret = VI5300_OK;
	uint8_t tmp=0;

	VI5300_Wait_For_CPU_Ready();
	VI5300_Set_Digital_Clock_Dutycycle();
	ret |= WriteOneReg(0x0C, 0x00);//read
	ret |= WriteOneReg(0x0D, 0x02);
	ret |= WriteOneReg(0x0E, 0x17);
	ret |= WriteOneReg(0x0A, 0x09);
	HAL_Delay(5);
	ret |= ReadOneReg(0x0c, &tmp);
	*refTof=tmp<<8;
	ret |= ReadOneReg(0xd, &tmp);
	VI5300_CHECK_RET(ret);  
	*refTof |= tmp;

	return ret;
}
void VI5300_Start_RefTof_Calibration(VI5300_OFFSET_Calib_Data* OFFSETCalibData)
{
		uint8_t retry=0;
		uint8_t stat=0xff,buf[32];	
		int16_t milimeter;
		uint32_t peak2,peak_tmp;
		uint32_t integral_times;
		int32_t bias;
		int16_t i_reftof=0;
		int16_t total = 0;
		int cnt = 0;
		#ifdef VI5300_OFFSET_CALIBRATION
		int16_t ioffset = 0;
		#endif
	

		do
		{
			VI5300_Temp_Enable(0);
			HAL_Delay(2);
			VI5300_Check_Temp_Enable(&stat);
		}while((stat!=0)&&(retry++<5));
		
		HAL_Delay(5);
		VI5300_Start_Continuous_Measure(); 
		while(1)
	  {
				HAL_Delay(30);
				VI5300_Get_Interrupt_State(&stat);
				if((stat & 0x01) == 0x01)
				{
						I2C_ReadXBytes(VI5300_REG_SCRATCH_PAD_BASE, buf, 32);
						i_reftof +=(short)((((short)buf[21])<<8) |(( (short)buf[20])));
						milimeter = (int16_t)((((int16_t)buf[13])<<8) |(( (int16_t)buf[12])));

						peak2 = (uint32_t)((((uint32_t)buf[11])<<24) |( ((uint32_t)buf[10])<<16)|(((uint32_t)buf[9])<<8)|( (uint32_t)buf[8]));

						integral_times = (uint32_t)((((uint32_t)buf[25])<<24) |( ((uint32_t)buf[24])<<16)|( ((uint32_t)buf[23])<<8)|( (uint32_t)buf[22]));
						integral_times = integral_times & 0x00ffffff;
						if(peak2 > 65536) {
								peak_tmp = (peak2 * 256) / integral_times * 256;
						} else {
								peak_tmp = peak2 * 65536 / integral_times;
						}

						peak_tmp = peak_tmp >> 12;
						bias = (int32_t)(PILEUP_A / (PILEUP_B - peak_tmp * PILEUP_D) - PILEUP_C) / PILEUP_D;
						if(bias < 0)
						{
							bias = 0;
						}
						milimeter = milimeter + (int16_t)bias;
						total += milimeter;
						cnt++;
						}
						if(cnt>=20)
						break;
		}
		retry=0;
		VI5300_Stop_Continuous_Measure();
		OFFSETCalibData->RefTof =i_reftof/20;
		#ifdef VI5300_OFFSET_CALIBRATION
		ioffset = total / 20;
		OFFSETCalibData->offset_cal = ioffset - OFFSET_POSITION;   //此处100是指在10cm处做offset标定，在多远距离做标定减多少，单位mm
		#endif
		stat=0;
		do
		{
				VI5300_Temp_Enable(1);
				HAL_Delay(2);
				VI5300_Check_Temp_Enable(&stat);
		}while((stat!=1)&&(retry++<5));
}
void VI5300_Config_RefTof(int16_t reftof)
{
		int16_t R_reftof;
		uint8_t retry=0;
		
		do
		{
				VI5300_Config_RefTof_Parameter(reftof);
				HAL_Delay(2);
				VI5300_Check_RefTof_Parameter(&R_reftof);
		}while((R_reftof!=reftof)&&retry++<5);
		HAL_Delay(2);
}
#endif
uint8_t VI5300_Stop_Continuous_Measure(void)
{
	VI5300_Status ret = VI5300_OK;

	ret = WriteCommand(0x1F);
	HAL_Delay(10);
	VI5300_CHECK_RET(ret);

	return ret;
}
VI5300_Status VI5300_Get_Interrupt_State(uint8_t *status)
{
    return ReadOneReg(VI5300_RET_INT_STATUS, status);
}

VI5300_Status VI5300_Start_Continuous_Measure(void)
{    
	VI5300_Status ret = VI5300_OK;
	uint8_t stat = 0;

	ret|= VI5300_Get_Interrupt_State(&stat);
	VI5300_Wait_For_CPU_Ready();
	VI5300_Set_Digital_Clock_Dutycycle();
	ret |= WriteCommand(0x0F);
	VI5300_CHECK_RET(ret);

	return VI5300_OK;
}
VI5300_Status VI5300_Start_Single_Measure(void)
{    
	VI5300_Status ret = VI5300_OK;
	uint8_t stat = 0;

	ret |= VI5300_Get_Interrupt_State(&stat);
	VI5300_Wait_For_CPU_Ready();
	VI5300_Set_Digital_Clock_Dutycycle();
	ret |= WriteCommand(0x0E);
	VI5300_CHECK_RET(ret);

	return VI5300_OK;
}

VI5300_Status VI5300_GetRawRangingData(VI5300_Dist_TypeDef *result)
{
	uint8_t buf[32],i;
	int16_t milimeter;
	uint16_t noise,noise_level;
	uint32_t peak1;
	uint32_t peak2,peak1_r,noise_r;
	uint32_t integral_times;
	uint32_t peak_tmp;
	uint32_t lower;
	uint32_t upper;
	int32_t bias;
	uint32_t confidence;
	uint32_t s_noise[10] = {19, 83, 163, 691, 1243, 2539, 5091, 10395, 20427, 33235};
	uint32_t s_lower[10] = {16, 25, 45, 145, 258, 501, 1004, 1980, 3804, 6063};
	uint32_t s_upper[10] = {26, 35, 61, 173, 303, 574, 1148, 2220, 4230, 6705};

	I2C_ReadXBytes(VI5300_REG_SCRATCH_PAD_BASE, buf, 32);

	/***********add the function of pileup and confidence 20210105  小端模式*********/
	milimeter = (short)((((short)buf[13])<<8) |(( (short)buf[12])));
	peak1 = (uint32_t)((((uint32_t)buf[31])<<24) |((uint32_t)buf[30]<<16)|((uint32_t)buf[29]<<8)|((uint32_t)buf[28]));
	noise_level = (uint16_t)((((uint16_t)buf[27])<<8) |(((uint16_t) buf[26])));
	peak2 = (uint32_t)((((uint32_t)buf[11])<<24) |( ((uint32_t)buf[10])<<16)|(((uint32_t)buf[9])<<8)|( (uint32_t)buf[8]));

	integral_times = (uint32_t)((((uint32_t)buf[25])<<24) |( ((uint32_t)buf[24])<<16)|( ((uint32_t)buf[23])<<8)|( (uint32_t)buf[22]));
    integral_times = integral_times & 0x00ffffff;
		
    peak_tmp = peak2 * 16 /integral_times;
		//printf("rawtof=%6d,peak1=%d,noise=%d,peak2=%d,integral_times=%6d\r\n",milimeter,peak1,noise_level,peak2,integral_times);
    bias = (int32_t)(PILEUP_A / (PILEUP_B - peak_tmp * PILEUP_D) - PILEUP_C) / PILEUP_D;
    if(bias < 0)
	{
			bias = 0;
	}
	milimeter = milimeter + (int16_t)bias;
	
	#ifdef VI5300_OFFSET_CALIBRATION
	milimeter = milimeter - offset_data.offset_cal;//减去offset
	#endif
	////update by 2022/03/07  
	if(peak1 > 8000000)
		peak1_r = peak1 * 256 / integral_times * 4;
	else if(peak1 > 4000000)
		peak1_r = peak1 * 512 / integral_times * 2;
	else
		peak1_r = peak1 * 1024 / integral_times;

	noise_r = noise_level * 65536 /integral_times* 2;

	for(i = 0; i < 9; i++)
	{
		if(noise_r < s_noise[i + 1])
		{
			lower = (s_lower[i + 1] - s_lower[i]) * abs(noise_r - s_noise[i]) / (s_noise[i + 1] - s_noise[i]) + s_lower[i];
			upper = (s_upper[i + 1] - s_upper[i]) * abs(noise_r - s_noise[i]) / (s_noise[i + 1] - s_noise[i]) + s_upper[i];
			break;
		} else if(noise_r >= s_noise[9]){
			lower = (s_lower[9] - s_lower[8]) * abs(noise_r - s_noise[8]) / (s_noise[9] - s_noise[8]) + s_lower[8];
			upper = (s_upper[9] - s_upper[8]) * abs(noise_r - s_noise[8]) / (s_noise[9] - s_noise[8]) + s_upper[8];
			break;
		}
	}
	if(peak1_r < lower) {
		confidence = 0;
	} else if(peak1_r > upper) {
		confidence = 100;
	} else {
		confidence = 100 * (peak1_r - lower) / (upper - lower);
	}

	//For YK use only, the filter will affect the peak strength, add this process to rejudge the confidence
	if(peak1 < PEAK_THRESHOLD && peak1/noise_level < SNR_THRESHOLD )
	{
		confidence = 0;
	}
	
	if(confidence >= 90)
	{
			result->status = STATUS_TOF_CONFIDENT;
	}
	else if(confidence >= 30)
	{
			result->status = STATUS_TOF_SEMI_CONFIDENT;
	}
	else
	{
			result->status = STATUS_TOF_NOT_CONFIDENT;
	}
	if(milimeter < 50 && peak1 < 800000)
	{
		//confidence =0;
			result->status = STATUS_TOF_NOT_CONFIDENT;
	}

	result->millimeter = milimeter;
	result->peak       = peak1;
	result->noise      = noise;
	result->confidence = confidence;
	//printf("rawtof=%6d,peak1=%d,noise=%d,peak2=%d,integral_times=%6d,confidence=%6d,peak1_r =%6d, lower=%6d,upper==%6d\r\n",milimeter,peak1,noise_level,peak2,integral_times,confidence,peak1_r,lower,upper);
	return VI5300_OK;
}

VI5300_Status VI5300_Get_Measure_Data(VI5300_Dist_TypeDef *result)
{
	VI5300_Status ret = VI5300_OK;

	uint8_t int_status = 0;
	ret = VI5300_Get_Interrupt_State(&int_status);
	VI5300_CHECK_RET(ret);

	if((int_status & 0x01) == 0x00)
	{
		return VI5300_RANGING;
	}
	ret = VI5300_GetRawRangingData(result);
	VI5300_CHECK_RET(ret);

	return VI5300_OK;
}
VI5300_Status VI5300_Interrupt_Enable(void)
{
		VI5300_Status ret = VI5300_OK;
		int loop = 0;
		uint8_t enable = 0;

		do
		{
				ret |= ReadOneReg(0x04,&enable);
				enable |=  0x01;
				ret |= WriteOneReg(0x04, enable);
				ret |= ReadOneReg(0x04, &enable);
				loop++;
		} while((loop < 5)&& (!(enable & 0x01)));
		if(loop >= 5)
		{
				//printf("Enable interrupt Failed!");  
				ret = VI5300_ERROR;
		}
		return ret;
}

void VI5300_init(void)
{
		uint8_t stat = 0;
		uint8_t retry=0;;
	
		for(retry = 0; retry < 5; retry++)
		{
				ReadOneReg(0x38, &chip_reg38);
				if(chip_reg38 == 0x30)
				{
						ReadOneReg(0x3a, &chip_reg3a);
						if(chip_reg3a == 0x30)
						{
							chip_reg=0x30;                                                                              
							break;
						} 
				} 
				else if(chip_reg38 == 0x00)
				{
						ReadOneReg(0x3a, &chip_reg3a);
						if(chip_reg3a == 0x00)
						{
							chip_reg=0x00;                                                                                  
							break;
						} 
				}
		}
		VI5300_Chip_Register_Init_Reg();
		VI5300_Wait_For_CPU_Ready();
		retry=0;
		do{
				VI5300_Init_Firmware();
				HAL_Delay(10);//delay 10ms
				ReadOneReg(0x08, &stat);
		}while((stat!=0x66)&&(retry++<5));
		if(retry>=5)
		{
			//printf("Download firmware fail!\r\n");
				return ;
		}
		HAL_Delay(150);
		
		VI5300_Interrupt_Enable();
		//VI5300_Set_Integralcounts_Frame(30, 131072);
		//VI5300_Set_Integralcounts_Frame(10, 400000);
		//VI5300_Set_Integralcounts_Frame(80, 74500);
		
		//VI5300_Set_Integralcounts_Frame(80, 32500);
		VI5300_Set_Integralcounts_Frame(60, 45000);
		//VI5300_Set_Integralcounts_Frame(40, 70000);									   
}

