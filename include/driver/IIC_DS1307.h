/*
 * IIC_DS1307.h
 *
 *  Created on: 2 ���. 2017 �.
 *      Author: Kitaro
 */

#ifndef DS1307_H
#define DS1307_H

#include "user_interface.h"
#include <os_type.h>
#include "driver/my_i2c.h"
#include "user_config.h"


#define DS1307_ADDR 0xD0

#define DS1307_CTRL_OUTPUT         0x80
#define DS1307_CTRL_SQUAREWAVE     0x10
#define DS1307_CTRL_SQWAVE_32768HZ 0x03
#define DS1307_CTRL_SQWAVE_8192HZ  0x02
#define DS1307_CTRL_SQWAVE_4096HZ  0x01
#define DS1307_CTRL_SQWAVE_1HZ     0x00

#define DS1307_OUTPUT_LEVEL_1 0x80
#define DS1307_OUTPUT_LEVEL_0 0x00

#define DS1307_ADDR_TIME    0x00
#define DS1307_ADDR_CONTROL 0x07

#define DS1307_SET     0
#define DS1307_CLEAR   1
#define DS1307_REPLACE 2

#define DS1307_12HOUR_FLAG 0x40
#define DS1307_12HOUR_MASK 0x1f
#define DS1307_PM_FLAG     0x20








//uint8_t DS1307_send_buffer[DS1307_Buffer_Lenght];
//sec, min, hour in BCD format
uint8_t DS1307_Time[3];
// Day, month, year (0-99), weekday (0..6)
uint8_t DS1307_Date[4];
uint16_t DS1307_Year_Base;
uint8_t DS1307_Date_to_Write[7]; // ����� ��� ������ ���� � ��������. �������, �������, ������, ����, �����, ��.
//uint8_t DS1307_count;	// ʳ������ ������ DS1307

//uint8_t DS1307_Flags;	// 0bxxx1xxxxx - ���������� ����������� ������ ������
	   							// 0bx1xxxxxxx - ��� �������



void ICACHE_FLASH_ATTR DS1307_Init(void);
static void ICACHE_FLASH_ATTR DS1307_Inited(void);

void ICACHE_FLASH_ATTR DS1307_Read (void);
static void ICACHE_FLASH_ATTR DS1307_Readed (void);

void ICACHE_FLASH_ATTR DS1307_Set_Date(void);
static void ICACHE_FLASH_ATTR DS1307_Date_Set(void);




int16_t DS1307_GetVal(uint8 mode);
bool DS1307_SoftReset(void);

os_timer_t DS1307_timer_read;
os_timer_t DS1307_timer_set;

//void ICACHE_FLASH_ATTR DS1307_Read (void);
//void ICACHE_FLASH_ATTR DS1307_Set_Date(void);



#endif
