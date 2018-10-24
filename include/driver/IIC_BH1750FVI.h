#ifndef I2C_BH1750FVI_H
#define I2C_BH1750FVI_H


#include "user_interface.h"
#include <os_type.h>
#include "driver/my_i2c.h"
#include "math.h"
#include "user_config.h"


//uint8 i2c_BH1750FVI_WriteByte(uint8 SAddr,uint8 Byte[], uint8 ByteNumbers, IIC_F WhatDo);
//uint8 i2c_BH1750FVI_ReadByte(uint8 SAddr, IIC_F WhatDo);


extern void i2c_BH1750FVI_Write(void);
extern void i2c_BH1750FVI_Write1Byte(uint8 SAddr);

void i2c_BH1750FVI_Write1(void);
void i2c_BH1750FVI_Writed(void);  
            		
//====================================================================================================
#define i2c_BH1750FVI_Write_Restart 5		//	�������� �� �������� ������, ���� ���� �������.
#define i2c_BH1750FVI_Write_Error_Restart 5	//	�������� �� �������� ������, ���� ���� �������
#define i2c_BH1750FVI_Read_Restart 5		//	�������� �� �������� ������, ���� ���� �������.
#define i2c_BH1750FVI_Read_Error_Restart 5	//	�������� �� �������� ������, ���� ���� �������
//====================================================================================================

void i2c_BH1750FVI_Read(void);
void i2c_BH1750FVI_Read1(void);
void i2c_BH1750FVI_Readed(void);

//	�� ���� 
#define i2c_BH1750FVI_buffer_lenght 5					//	����. ������� ������, ��� ���� �������� BH1750FVI �� ���� ������ ���������
//uint8 i2c_BH1750FVI_buffer[i2c_BH1750FVI_buffer_lenght];
uint8 i2c_BH1750FVI_buffer_data;
uint8 i2c_BH1750FVI_Address;
uint16 i2c_BH1750FVI_Illuminance;
uint8 i2c_BH1750FVI_L;
uint8 i2c_BH1750FVI_H;
uint8 i2c_BH1750FVI_errors;	//	��� ������ ��������� ������� � ��������; ����� ��� ��� - �������� ������� i2c_ERR_NA, ��������� �� - ��������� - ������� ��������, ��� ��������.
#define i2c_BH1750FVI_errors_NA	0b00001000


//extern uint8 i2c_BH1750FVI_count;	// ʳ������ ������ BH1750FVI
//extern uint8 i2c_BH1750FVI_Addresses[8];	// ����� ����� ������ BH1750FVI, ����������� ���� ���� 2 �����

os_timer_t i2c_BH1750FVI_timer;





#endif
