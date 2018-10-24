/*
 * IIC_DS3231.h
 *
 *  Created on: 2 ���. 2017 �.
 *      Author: Kitaro
 */

#ifndef DS3231_H
#define DS3231_H

#include "user_interface.h"
#include <os_type.h>
#include "driver/my_i2c.h"
#include "user_config.h"


#define DS3231_Buffer_Lenght 7
//uint8_t DS3231_send_buffer[DS3231_Buffer_Lenght];
//sec, min, hour in BCD format
uint8_t DS3231_Time[3];
// Day, month, year (0-99), weekday (0..6)
uint8_t DS3231_Date[4];
uint16_t DS3231_Year_Base;
uint8_t DS3231_Date_to_Write[7]; // ����� ��� ������ ���� � ��������. �������, �������, ������, ����, �����, ��.
uint8_t DS3231_count;	// ʳ������ ������ DS3231

uint8_t DS3231_Flags;	// 0bxxx1xxxxx - ���������� ����������� ������ ������
	   							// 0bx1xxxxxxx - ��� �������
#define DS3231_Address_Mask	0b00000001
#define DS3231_Init1_Mask		0b00010000
#define DS3231_InitAll_Mask	0b00100000
#define DS3231_Read_Complete	0b01000000

#define DS3231_control_register_address 0x0E
#define DS3231_init_1 0x00
#define DS3231_init_2 0x08
#define DS3231_date_register_address 0x00

void ICACHE_FLASH_ATTR DS3231_Init(void);
static void ICACHE_FLASH_ATTR DS3231_Inited(void);

void ICACHE_FLASH_ATTR DS3231_Read (void);
static void ICACHE_FLASH_ATTR DS3231_Readed (void);

void ICACHE_FLASH_ATTR DS3231_Set_Date(void);
static void ICACHE_FLASH_ATTR DS3231_Date_Set(void);




int16_t DS3231_GetVal(uint8 mode);
bool DS3231_SoftReset(void);

os_timer_t DS3231_timer_read;
os_timer_t DS3231_timer_set;

//void ICACHE_FLASH_ATTR DS3231_Read (void);
//void ICACHE_FLASH_ATTR DS3231_Set_Date(void);



#endif
