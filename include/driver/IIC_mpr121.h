#ifndef I2C_MPR121_H
#define I2C_MPR121_H


#include "user_interface.h"
#include <os_type.h>
#include "driver/my_i2c.h"
#include "math.h"
#include "user_config.h"


// MPR121 Register Defines
#define MPR121_MHD_R	0x2B
#define MPR121_NHD_R	0x2C
#define	MPR121_NCL_R 	0x2D
#define	MPR121_FDL_R	0x2E
#define	MPR121_MHD_F	0x2F
#define	MPR121_NHD_F	0x30
#define	MPR121_NCL_F	0x31
#define	MPR121_FDL_F	0x32
#define	MPR121_ELE0_T	0x41
#define	MPR121_ELE0_R	0x42
#define	MPR121_ELE1_T	0x43
#define	MPR121_ELE1_R	0x44
#define	MPR121_ELE2_T	0x45
#define	MPR121_ELE2_R	0x46
#define	MPR121_ELE3_T	0x47
#define	MPR121_ELE3_R	0x48
#define	MPR121_ELE4_T	0x49
#define	MPR121_ELE4_R	0x4A
#define	MPR121_ELE5_T	0x4B
#define	MPR121_ELE5_R	0x4C
#define	MPR121_ELE6_T	0x4D
#define	MPR121_ELE6_R	0x4E
#define	MPR121_ELE7_T	0x4F
#define	MPR121_ELE7_R	0x50
#define	MPR121_ELE8_T	0x51
#define	MPR121_ELE8_R	0x52
#define	MPR121_ELE9_T	0x53
#define	MPR121_ELE9_R	0x54
#define	MPR121_ELE10_T	0x55
#define	MPR121_ELE10_R	0x56
#define	MPR121_ELE11_T	0x57
#define	MPR121_ELE11_R	0x58
#define	MPR121_FIL_CFG	0x5D
#define	MPR121_ELE_CFG	0x5E
#define MPR121_GPIO_CTRL0	0x73
#define	MPR121_GPIO_CTRL1	0x74
#define MPR121_GPIO_DATA	0x75
#define	MPR121_GPIO_DIR	0x76
#define	MPR121_GPIO_EN		0x77
#define	MPR121_GPIO_SET	0x78
#define	MPR121_GPIO_CLEAR	0x79
#define	MPR121_GPIO_TOGGLE	0x7A
#define	MPR121_ATO_CFG0	0x7B
#define	MPR121_ATO_CFGU	0x7D
#define	MPR121_ATO_CFGL	0x7E
#define	MPR121_ATO_CFGT	0x7F


// Global Constants
#define MPR121_TOU_THRESH	0x06
#define	MPR121_REL_THRESH	0x0A


#define i2c_mpr121_Init_Restart 5
#define i2c_mpr121_Init_Error_Restart 5
#define i2c_mpr121_Read_Restart 5
#define i2c_mpr121_Reenable_delay 20
#define i2c_mpr121_push_time_stage 100

void i2c_mpr121_Init(void);
void i2c_mpr121_Start_Reading (void);

BOOL touchStates[12];
uint8 touchTime[12];
uint8 i2c_mpr121_Init_Stage;
#define i2c_mpr121_Address_1 ((0x5a)<<1)


uint8 i2c_mpr121_buffer_data;


os_timer_t mpr121_timer;
os_timer_t mpr121_timer_int;



#endif
