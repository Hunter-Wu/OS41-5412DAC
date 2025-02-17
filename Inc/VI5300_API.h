#ifndef __VI5300_API_H__
#define __VI5300_API_H__

//#include "stm32f1xx_hal.h"
//#include "stm32f1xx.h"
#include "stdio.h"
#include "VI5300_Config.h"



#define API_VERSION  V08  //

#define STATUS_TOF_CONFIDENT      0
#define STATUS_TOF_SEMI_CONFIDENT 1
#define STATUS_TOF_NOT_CONFIDENT  255

#define VI5300_DEVICE_ADDR          0xD8
#define VI5300_REG_MCU_CFG          0x00
#define VI5300_RET_INT_STATUS       0x03
#define VI5300_REG_SYS_CFG          0x01
#define VI5300_REG_PW_CTRL          0x07
#define VI5300_REG_CMD              0x0a
#define VI5300_REG_SIZE             0x0b
#define VI5300_REG_SCRATCH_PAD_BASE 0x0c 

#define VI5300_WRITEFW_CMD          0x03
#define VI5300_USER_CFG_CMD         0x09
#define VI5300_START_RANG_CMD       0x0E

#define VI5300_CFG_SUBCMD           0x01
#define VI5300_OTPW_SUBCMD          0x02
#define VI5300_OTPR_SUBCMD          0x03

#define PILEUP_A (9231000)
#define PILEUP_B (4896)
#define PILEUP_C (1922)
#define PILEUP_D (10)


#define XSHUT_Pin1 GPIO_PIN_0
#define XSHUT_Pin2 GPIO_PIN_1
#define XSHUT_Pin3 GPIO_PIN_4
#define XSHUT_GPIO_Port GPIOA

//#define  VI5300_XTALK_CALIBRATION  //有盖板(coverglass)可以打开
//#define  VI5300_OFFSET_CALIBRATION  //offset 标定，默认60cm
#define  VI5300_TEMP_CALIBRATION   //must be open 

#ifdef VI5300_OFFSET_CALIBRATION 
#define OFFSET_POSITION      600    //默认60cm位置标定
#endif
//#define CHANGE_MODEL_ADDRESS 
#ifdef CHANGE_MODEL_ADDRESS  //change model address to control much models
 //The address must be even numbers
 //IIC 地址必须是双数
#define VI5300_MODEL1_ADDR  0xD0 
#define VI5300_MODEL2_ADDR  0xD2
#define VI5300_MODEL3_ADDR  0xD4
#endif


#define HAL_Delay(uint32_t) FL_DelayMs(uint32_t)

#define VI5300_CHECK_RET(a)   if(a != VI5300_OK) return a
	
extern uint8_t gSalve;

typedef struct 
{
    int16_t millimeter;
    uint32_t peak;
    uint16_t noise;
		uint32_t confidence;
		uint16_t status;
    
}VI5300_Dist_TypeDef;

typedef struct 
{
    uint8_t xtalk_cal;
    uint16_t xtalk_peak;
    
} VI5300_XTALK_Calib_Data;

typedef struct OFFSET_CALIB
{
   int16_t offset_cal;
	 int16_t RefTof;
    
} VI5300_OFFSET_Calib_Data;
typedef struct 
{
		uint8_t Cali_Flag;
    uint8_t xtalk_cal;
		int16_t offset_cal;
		int16_t RefTof;
}VI5300_CALIBRATION_DATA;

typedef enum 
{
    VI5300_OK       = 0x00,
    VI5300_RANGING  = 0x01,
    VI5300_BUSY     = 0x02,
    VI5300_BUS_BUSY = 0x03,
    VI5300_SLEEP    = 0x04,
    VI5300_BOOTING  = 0x05,
    VI5300_ERROR    = 0x06
} VI5300_Status;

typedef enum 
{
    VI5300_DATA_VALID = 0x00,
    VI5300_RAW_DATA   = 0x01,
    VI5300_DATA_ERROR = 0x02
} VI5300_RangingDataStatus;
typedef enum NEXTACTION
{
    VI5300_GO_ON = 0x00,
    VI5300_STOP  = 0x01
} VI5300_NextAction;

extern VI5300_OFFSET_Calib_Data  offset_data;
extern VI5300_XTALK_Calib_Data  xtalk_data;
extern VI5300_CALIBRATION_DATA  cali_data;

extern void VI5300_init(void);
extern void VI5300_Chip_PowerON1(void);
extern VI5300_Status VI5300_Interrupt_Enable(void);
extern VI5300_Status VI5300_Set_Digital_Clock_Dutycycle(void);
extern VI5300_Status VI5300_WriteFirmwarePreConfig(void);
extern VI5300_Status VI5300_WriteFirmwarePostConfig(void);
extern VI5300_Status VI5300_WriteFirmware32Byte(uint8_t len, uint8_t *data);
extern VI5300_Status VI5300_DownloadFirmware( uint8_t *buf, uint16_t size);
extern void VI5300_Init_Firmware(void);
extern VI5300_Status VI5300_Integral_Counts_Write(uint32_t inte_counts);
extern VI5300_Status VI5300_Set_Integralcounts_Frame(uint8_t fps, uint32_t intecoutns);
#ifdef VI5300_XTALK_CALIBRATION
extern VI5300_Status VI5300_Start_XTalk_Calibration(VI5300_XTALK_Calib_Data  *xtalk);
extern VI5300_Status VI5300_Config_XTalk_Parameter(uint8_t xtalk_para);
#endif//VI5300_XTALK_CALIBRATION
extern uint8_t VI5300_Stop_Continuous_Measure(void);
extern VI5300_Status VI5300_Get_Interrupt_State(uint8_t *status);
extern VI5300_Status VI5300_Start_Continuous_Measure(void);
extern VI5300_Status VI5300_Start_Offset_Calibration(void);
extern VI5300_Status VI5300_GetRawRangingData(VI5300_Dist_TypeDef *result);
extern VI5300_Status VI5300_Get_Measure_Data(VI5300_Dist_TypeDef *result);
extern uint8_t VI5300_Device_Adress_Modify(uint16_t pin,uint8_t addr_val);
uint8_t VI5300_Device_Check(void);
#ifdef VI5300_TEMP_CALIBRATION
extern VI5300_Status VI5300_Temp_Enable(uint8_t enable);
extern VI5300_Status VI5300_Check_Temp_Enable(uint8_t *enable);
extern void VI5300_Start_RefTof_Calibration(VI5300_OFFSET_Calib_Data* OFFSETCalibData);
extern uint8_t VI5300_Config_RefTof_Parameter(int16_t refTof);
extern void VI5300_Config_RefTof(int16_t reftof);
#endif//VI5300_TEMP_CALIBRATION
#endif


