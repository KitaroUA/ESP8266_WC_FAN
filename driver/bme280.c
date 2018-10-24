/*
The driver for the sensor BME280

Copyright (c) 2016, Cosmin Plasoianu
Copyright (c) 2016, Mikhail Grigorev
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE
*/

#include "driver/bme280.h"

#define ID_REG		0xD0
#define PRESS_MSB_REG	0xF7
#define CALIB_00_REG	0x88
#define CALIB_26_REG	0xE1
#define CONFIG_REG	0xF5
#define CTRL_MEAS_REG	0xF4
#define STATUS_REG	0xF3
#define CTRL_HUM_REG	0xF2

static struct {
	uint16 dig_T1;
	uint16 dig_T2;
	uint16 dig_T3;

	uint16 dig_P1;
	sint16 dig_P2;
	sint16 dig_P3;
	sint16 dig_P4;
	sint16 dig_P5;
	sint16 dig_P6;
	sint16 dig_P7;
	sint16 dig_P8;
	sint16 dig_P9;

	uint8  dig_H1;
	sint16 dig_H2;
	uint8  dig_H3;
	sint16 dig_H4;
	sint16 dig_H5;
	sint8  dig_H6;
} CalibrationParam;

sint32 t_fine;		// t_fine carries fine temperature as global value

uint8 BME280_os_t;
uint8 BME280_os_p;
uint8 BME280_os_h;
uint8 BME280_filter;
uint8 BME280_mode;
uint8 BME280_t_sb;


/**********************************************************************
Return: 0 	  - Everything OK
		non 0 - Failed
Parameters:	os_t - Temperature Oversampling
			os_p - Pressure Oversampling
			os_h - Humidity Oversampling
			filter - Filter coefficient
			mode - Mode (Sleep/Forced/Normal)
			t_sb - Standby time between conversions
**********************************************************************/

void ICACHE_FLASH_ATTR BME280_Start_Init()
{
BME280_Init(BME280_OS_T_16, BME280_OS_P_16, BME280_OS_H_16,	BME280_FILTER_16, BME280_MODE_NORMAL, BME280_TSB_05);
//INFO("\r\nS1\r\n");
}

void ICACHE_FLASH_ATTR BME280_Init(uint8 os_t, uint8 os_p, uint8 os_h, uint8 filter, uint8 mode, uint8 t_sb)
{
BME280_os_t=os_t;
BME280_os_p=os_p;
BME280_os_h=os_h;
BME280_filter=filter;
BME280_mode=mode;
BME280_t_sb=t_sb;

//INFO("\r\nS2\r\n");
	if (!i2c_Read_Addr_Buffer(BME280_I2C_ADDR, ID_REG, 1, BME280_Init_S1) )      // ������
	{
		os_timer_disarm (&BME280_timer_read);
		os_timer_setfn (&BME280_timer_read, (os_timer_func_t *)BME280_Init, NULL);
		os_timer_arm(&BME280_timer_read, 77, 0);	// ��������� ������� ����� 2��
	}
}


void ICACHE_FLASH_ATTR BME280_Init_S1()
	{
//	INFO("\r\nstep1:%x\r\n",i2c_Buffer[0]);
//	INFO("\r\nS1\r\n");
	i2c_Do &= i2c_Free;			// ����������� ����

	if (i2c_Buffer[0] == 0x58)
	{

		os_timer_disarm (&BME280_timer_read);
	//	os_timer_setfn (&BME280_timer_read, (os_timer_func_t *)BME280_Init_S4, NULL);
		os_timer_setfn (&BME280_timer_read, (os_timer_func_t *)BME280_Init_S2, NULL);
		os_timer_arm(&BME280_timer_read, 1000, 0);	// ��������� ������� ����� 2��
		INFO("\r\nFound BMP280\r\n");
		BMP_sensor_type = 1;
	}
	else
	{
	if (i2c_Buffer[0] == 0x60)
	{
	os_timer_disarm (&BME280_timer_read);
//	os_timer_setfn (&BME280_timer_read, (os_timer_func_t *)BME280_Init_S4, NULL);
	os_timer_setfn (&BME280_timer_read, (os_timer_func_t *)BME280_Init_S2, NULL);
	os_timer_arm(&BME280_timer_read, 1000, 0);	// ��������� ������� ����� 2��
	INFO("\r\nFound BME280\r\n");
	BMP_sensor_type = 2;
	}

	else
	{
		INFO ("No BMx280 Sensors Found ");
	}
	}
	}

void ICACHE_FLASH_ATTR BME280_Init_S2()
	{

	if (!i2c_Read_Addr_Buffer(BME280_I2C_ADDR, CALIB_00_REG,26, BME280_Init_S3) )      // ������
	{
		os_timer_disarm (&BME280_timer_read);
		os_timer_setfn (&BME280_timer_read, (os_timer_func_t *)BME280_Init_S2, NULL);
		os_timer_arm(&BME280_timer_read, 77, 0);	// ��������� ������� ����� 2��
	}
//	INFO("\r\nS2\r\n");


	}



void ICACHE_FLASH_ATTR BME280_Init_S3()
	{
//	INFO("\r\nstep2:%x\r\n",i2c_Buffer[0]);

	i2c_Do &= i2c_Free;			// ����������� ����


	CalibrationParam.dig_T1 = (i2c_Buffer[1] << 8) | i2c_Buffer[0];
	CalibrationParam.dig_T2 = (i2c_Buffer[3] << 8) | i2c_Buffer[2];
	CalibrationParam.dig_T3 = (i2c_Buffer[5] << 8) | i2c_Buffer[4];

	CalibrationParam.dig_P1 = (i2c_Buffer[7] << 8) | i2c_Buffer[6];
	CalibrationParam.dig_P2 = (i2c_Buffer[9] << 8) | i2c_Buffer[8];
	CalibrationParam.dig_P3 = (i2c_Buffer[11] << 8) | i2c_Buffer[10];
	CalibrationParam.dig_P4 = (i2c_Buffer[13] << 8) | i2c_Buffer[12];
	CalibrationParam.dig_P5 = (i2c_Buffer[15] << 8) | i2c_Buffer[14];
	CalibrationParam.dig_P6 = (i2c_Buffer[17] << 8) | i2c_Buffer[16];
	CalibrationParam.dig_P7 = (i2c_Buffer[19] << 8) | i2c_Buffer[18];
	CalibrationParam.dig_P8 = (i2c_Buffer[21] << 8) | i2c_Buffer[20];
	CalibrationParam.dig_P9 = (i2c_Buffer[23] << 8) | i2c_Buffer[22];

	if (BMP_sensor_type == 2)
	{
	CalibrationParam.dig_H1 = i2c_Buffer[25];

	os_timer_disarm (&BME280_timer_read);
	os_timer_setfn (&BME280_timer_read, (os_timer_func_t *)BME280_Init_S4, NULL);
	os_timer_arm(&BME280_timer_read, 1000, 0);	// ��������� ������� ����� 2��
	}
	if (BMP_sensor_type == 1)
	{

	os_timer_disarm (&BME280_timer_read);
	os_timer_setfn (&BME280_timer_read, (os_timer_func_t *)BME280_Init_S6, NULL);
	os_timer_arm(&BME280_timer_read, 1000, 0);	// ��������� ������� ����� 2��
	}
//	INFO("\r\nS3\r\n");

	}




void ICACHE_FLASH_ATTR BME280_Init_S4()
	{ // only for E type sensor, BM_E_280

	if (!i2c_Read_Addr_Buffer(BME280_I2C_ADDR, CALIB_26_REG, 8, BME280_Init_S5) )      // ������
	{

		os_timer_disarm (&BME280_timer_read);
		os_timer_setfn (&BME280_timer_read, (os_timer_func_t *)BME280_Init_S4, NULL);
		os_timer_arm(&BME280_timer_read, 77, 0);	// ��������� ������� ����� 2��
	}

//	INFO("\r\nS4\r\n");

	}




void ICACHE_FLASH_ATTR BME280_Init_S5()
	{
//	INFO("\r\niS5\r\n");
//	INFO("\r\nstep3:%x\r\n",i2c_Buffer[0]);

	i2c_Do &= i2c_Free;			// ����������� ����

	CalibrationParam.dig_H2 = (i2c_Buffer[1] << 8) | i2c_Buffer[0];
//	INFO("\r\nH2:%d\r\n", CalibrationParam.dig_H2);
	CalibrationParam.dig_H3 = i2c_Buffer[2];
//	INFO("\r\nH3:%d\r\n", CalibrationParam.dig_H3);
	CalibrationParam.dig_H4 = (i2c_Buffer[3] << 4) | (i2c_Buffer[4] & 0x0F);
//	INFO("\r\nH4:%d\r\n", CalibrationParam.dig_H4);
	CalibrationParam.dig_H5 = (i2c_Buffer[5] << 4) | (i2c_Buffer[4] >> 4);
//	INFO("\r\nH5:%d\r\n", CalibrationParam.dig_H5);
	CalibrationParam.dig_H6 = i2c_Buffer[6];
//	INFO("\r\nH6:%d\r\n", CalibrationParam.dig_H6);

	os_timer_disarm (&BME280_timer_read);
	os_timer_setfn (&BME280_timer_read, (os_timer_func_t *)BME280_Init_S6, NULL);
	os_timer_arm(&BME280_timer_read, 1000, 0);	// ��������� ������� ����� 2��

//	INFO("\r\nS5\r\n");

	}

void ICACHE_FLASH_ATTR BME280_Init_S6()
	{
	uint8 buff[1];
	buff[0] = (BME280_t_sb << 5) | ((BME280_filter & 0x07) << 2);				//config
	void (*ToDo)(void);
	if (BMP_sensor_type == 2)
	{
	ToDo = &BME280_Init_S7;
	}
	if (BMP_sensor_type == 1)
	{
	ToDo = &BME280_Init_S8;
	}

	if (!i2c_Write_Addr_Buffer(BME280_I2C_ADDR, CONFIG_REG,buff,1,ToDo))	// ���� ���� ������
	{
		os_timer_disarm (&BME280_timer_read);
		os_timer_setfn (&BME280_timer_read, (os_timer_func_t *)BME280_Init_S6, NULL);
		os_timer_arm(&BME280_timer_read, 2, 0);	// ��������� ������� ����� 2��
	}
//	INFO("\r\nS6\r\n");

	}

void ICACHE_FLASH_ATTR BME280_Init_S7()
	{// only for E type sensor, BM_E_280
	i2c_Do &= i2c_Free;			// ����������� ����

	uint8 buff[1];
	buff[0]= BME280_os_h & 0x07;				//start humidity reading


	if (!i2c_Write_Addr_Buffer(BME280_I2C_ADDR, CTRL_HUM_REG,buff,1,BME280_Init_S8))	// ���� ���� ������
	{

		os_timer_disarm (&BME280_timer_read);
		os_timer_setfn (&BME280_timer_read, (os_timer_func_t *)BME280_Init_S7, NULL);
		os_timer_arm(&BME280_timer_read, 2, 0);	// ��������� ������� ����� 2��
	}
//	INFO("\r\nS7\r\n");

	}



void ICACHE_FLASH_ATTR BME280_Init_S8()
	{
	i2c_Do &= i2c_Free;			// ����������� ����

	uint8 buff[1];
	buff[0]= (BME280_os_t << 5) | ((BME280_os_p & 0x07) << 2) | (BME280_mode & 0x03);				//config


	if (!i2c_Write_Addr_Buffer(BME280_I2C_ADDR, CTRL_MEAS_REG ,buff,1,BME280_Init_S9))	// ���� ���� ������
	{

		os_timer_disarm (&BME280_timer_read);
		os_timer_setfn (&BME280_timer_read, (os_timer_func_t *)BME280_Init_S8, NULL);
		os_timer_arm(&BME280_timer_read, 2, 0);	// ��������� ������� ����� 2��
	}
//	INFO("\r\nS8\r\n");

	}



void ICACHE_FLASH_ATTR BME280_Init_S9()
	{
	i2c_Do &= i2c_Free;			// ����������� ����

os_timer_disarm(&BME280_timer_read);
os_timer_setfn(&BME280_timer_read, (os_timer_func_t *)BME280_ReadAll, NULL);
os_timer_arm(&BME280_timer_read, 5000, 1);
//INFO("\r\nS9\r\n");

	}








// Returns temperature in DegC, resolution is 0.01 DegC. Output value of �5123� equals 51.23 DegC.
sint32  ICACHE_FLASH_ATTR BME280_CompensateT(sint32 adc_T)
{
	sint32 var1, var2, T;

	var1 = ((((adc_T>>3) - ((sint32)CalibrationParam.dig_T1<<1))) * ((sint32)CalibrationParam.dig_T2)) >> 11;
	var2 = (((((adc_T>>4) - ((sint32)CalibrationParam.dig_T1)) * ((adc_T>>4) - ((sint32)CalibrationParam.dig_T1))) >> 12) * ((sint32)CalibrationParam.dig_T3)) >> 14;
	t_fine = var1 + var2;
	T = (t_fine * 5 + 128) >> 8;

	return T;
}

// Returns pressure in Pa as unsigned 32 bit integer in Q24.8 format (24 integer bits and 8 fractional bits).
// Output value of �24674867� represents 24674867/256 = 96386.2 Pa = 963.862 hPa
uint32  ICACHE_FLASH_ATTR BME280_CompensateP(sint32 adc_P)
{
	sint64 var1, var2, p;

	var1 = ((sint64)t_fine) - 128000;
	var2 = var1 * var1 * (sint64)CalibrationParam.dig_P6;
	var2 = var2 + ((var1*(sint64)CalibrationParam.dig_P5)<<17);
	var2 = var2 + (((sint64)CalibrationParam.dig_P4)<<35);
	var1 = ((var1 * var1 * (sint64)CalibrationParam.dig_P3)>>8) + ((var1 * (sint64)CalibrationParam.dig_P2)<<12);
	var1 = (((((sint64)1)<<47) + var1)) * ((sint64)CalibrationParam.dig_P1)>>33;

	if (var1 == 0)
		return 0; 	//avoid exception caused by division by zero

	p = 1048576 - adc_P;
	p = (((p << 31) - var2) * 3125) / var1;
	var1 = (((sint64)CalibrationParam.dig_P9) * (p>>13) * (p>>13)) >> 25;
	var2 = (((sint64)CalibrationParam.dig_P8) * p) >> 19;
	p = ((p + var1 + var2) >> 8) + (((sint64)CalibrationParam.dig_P7)<<4);

	return (uint32)p;
}

// Returns humidity in %RH as unsigned 32 bit integer in Q22.10 format (22 integer and 10 fractional bits).
// Output value of �47445� represents 47445/1024 = 46.333 %RH
uint32  ICACHE_FLASH_ATTR BME280_CompensateH(sint32 adc_H)
{
	sint32 v_x1_u32r;

	v_x1_u32r = (t_fine - ((sint32)76800));
	v_x1_u32r = (((((adc_H << 14) - (((sint32)CalibrationParam.dig_H4) << 20) - (((sint32)CalibrationParam.dig_H5) * v_x1_u32r)) +
		((sint32)16384)) >> 15) * (((((((v_x1_u32r * ((sint32)CalibrationParam.dig_H6)) >> 10) * (((v_x1_u32r *
		((sint32)CalibrationParam.dig_H3)) >> 11) + ((sint32)32768))) >> 10) + ((sint32)2097152)) *
		((sint32)CalibrationParam.dig_H2) + 8192) >> 14));
	v_x1_u32r = (v_x1_u32r - (((((v_x1_u32r >> 15) * (v_x1_u32r >> 15)) >> 7) * ((sint32)CalibrationParam.dig_H1)) >> 4));
	v_x1_u32r = (v_x1_u32r < 0 ? 0 : v_x1_u32r);
	v_x1_u32r = (v_x1_u32r > 419430400 ? 419430400 : v_x1_u32r);

	return (uint32)(v_x1_u32r>>12);
}










/**********************************************************************
Return: 0 	  - Everything OK
		non 0 - Failed
Parameters:	t - Pointer to variable in which to write the temperature
			p - Pointer to variable in which to write the pressure
			h - Pointer to variable in which to write the humidity
**********************************************************************/
void  ICACHE_FLASH_ATTR BME280_ReadAll()
{


	if (!i2c_Read_Addr_Buffer(BME280_I2C_ADDR, PRESS_MSB_REG, 8, BME280_ReadAll_End) )      // ������
	{
		os_timer_disarm (&BME280_timer_read);
		os_timer_setfn (&BME280_timer_read, (os_timer_func_t *)BME280_ReadAll, NULL);
		os_timer_arm(&BME280_timer_read, 77, 0);	// ��������� ������� ����� 2��
//		INFO("Cycle2");
	}


}




void  ICACHE_FLASH_ATTR BME280_ReadAll_End()
{
	i2c_Do &= i2c_Free;			// ����������� ����

	sint32 UncT, UncP;
	sint32 UncH = 0;

	UncP = ((uint32)i2c_Buffer[0] << 16) | ((uint16)i2c_Buffer[1] << 8) | i2c_Buffer[2];
	UncP >>= 4;

	UncT = ((uint32)i2c_Buffer[3] << 16) | ((uint16)i2c_Buffer[4] << 8) | i2c_Buffer[5];
	UncT >>= 4;
	if (BMP_sensor_type == 2)
	{
	UncH = ((uint16)i2c_Buffer[6] << 8) | i2c_Buffer[7];
	}

	bme280_temperature = BME280_CompensateT(UncT);
	bme280_pressure = BME280_CompensateP(UncP);
	if (BMP_sensor_type == 2)
	{
	bme280_humidity = BME280_CompensateH(UncH);
	}

	if (BMP_sensor_type == 2)
	{
	INFO( "\r\nTemperature: %d.%02u C, Humidity: %u.%02u rH, Pressure: %u.%02u hPa",
			bme280_temperature / 100, bme280_temperature % 100,							//C
			bme280_humidity >> 10, ((bme280_humidity & 0x000003FF) * 100) >> 10,			//rH
				(bme280_pressure >> 8) / 100, (bme280_pressure >> 8) % 100);					//hPa
	}
	if (BMP_sensor_type == 1)
	{
	INFO( "\r\nTemperature: %d.%02u C, Pressure: %u.%02u hPa",
			bme280_temperature / 100, bme280_temperature % 100,							//C
			(bme280_pressure >> 8) / 100, (bme280_pressure >> 8) % 100);					//hPa
	}

	os_timer_disarm(&BME280_timer_read);
	os_timer_setfn(&BME280_timer_read, (os_timer_func_t *)BME280_ReadAll, NULL);
	os_timer_arm(&BME280_timer_read, 10000, 1);
//	INFO("\r\nRE\r\n");

}































/**********************************************************************
Return: 0 	  - Everything OK
		non 0 - Failed
Parameters:	mode - Mode (Sleep/Forced/Normal)
**********************************************************************/
uint8  ICACHE_FLASH_ATTR BME280_SetMode(uint8 mode)
{
	uint8 RegVal[1] = {0};

	mode &= 0x03;
	i2c_Read_Addr_Buffer(BME280_I2C_ADDR, CTRL_MEAS_REG, 1, NULL);
//	I2C_ReadData(BME280_I2C_ADDR, CTRL_MEAS_REG, &RegVal, 1);
	RegVal[0] &= 0xFC;
	RegVal[0] |= mode;

//	return I2C_WriteData(BME280_I2C_ADDR, CTRL_MEAS_REG, &RegVal, 1);
	return i2c_Write_Addr_Buffer(BME280_I2C_ADDR, CTRL_MEAS_REG, RegVal, 1, NULL);
}






































































































