/*
 * IIC_PCF8574.h
 *
 *  Created on: 8 ���. 2017 �.
 *      Author: Kitaro
 */

#ifndef INCLUDE_DRIVER_IIC_PCF8574_H_
#define INCLUDE_DRIVER_IIC_PCF8574_H_


#include "user_interface.h"
#include <os_type.h>
#include "driver/my_i2c.h"

void i2c_PCF8574_Write(uint8 SAddr, uint8 Byte[], uint8 ByteNumbers);
void i2c_PCF8574_Write1Byte(uint8 SAddr, uint8 Byte);

void i2c_PCF8574_Write1(void);
void i2c_PCF8574_Writed(void);

//====================================================================================================
#define i2c_PCF8574_Write_Restart 4		//	�������� �� �������� ������, ���� ���� �������.
#define i2c_PCF8574_Write_Error_Restart 4	//	�������� �� �������� ������, ���� ���� �������
#define i2c_PCF8574_Read_Restart 4		//	�������� �� �������� ������, ���� ���� �������.
#define i2c_PCF8574_Read_Error_Restart 4	//	�������� �� �������� ������, ���� ���� �������
//====================================================================================================

void i2c_PCF8574_Read(void);
void i2c_PCF8574_Read1(void);
void i2c_PCF8574_Readed(void);

//	�� ����
#define i2c_PCF8574_buffer_lenght 5					//	����. ������� ������, ��� ���� �������� PCF8574 �� ���� ������ ���������
uint8 i2c_PCF8574_buffer[i2c_PCF8574_buffer_lenght];
uint8 i2c_PCF8574_buffer_data;
uint8 i2c_PCF8574_Address;

//uint8 i2c_PCF8574_count;	// ʳ������ ������ PCF8574
//uint8 i2c_PCF8574_Addresses[8];	// ����� ����� ������ PCF8574, ����������� ���� ���� 2 �����

os_timer_t i2c_PCF8574_timer_set;
uint8 i2c_PCF8574_enabled; //0 - skip PCF8574 tasks, 1 - send data to PCF8574


#endif /* INCLUDE_DRIVER_IIC_PCF8574_H_ */
