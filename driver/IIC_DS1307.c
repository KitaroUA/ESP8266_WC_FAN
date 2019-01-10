/*
 * IIC_DS1307.c
 *
 *  Created on: 2 ���. 2017 �.
 *      Author: Kitaro
 */

#include "driver/IIC_DS1307.h"




//uint8_t DS1307_send_buffer[DS1307_Buffer_Lenght];

uint8_t DS1307_Time[3];
uint8_t DS1307_Date[4];
uint16_t DS1307_Year_Base = 2000;

uint8_t DS1307_Date_to_Write[7];// ����� ��� ������ ���� � ��������. �������, �������, ������, ���� �����, ����, �����, ��.
//uint8_t DS1307_ADDRESS = 0xD0;
uint8_t DS1307_Flags = 0;	// 0bxxx1xxxxx - ���������� ����������� ������ ������
							// 0bx1xxxxxxx - ��� �������





// convert normal decimal to binary coded decimal
static uint8 ICACHE_FLASH_ATTR decToBcd(uint8 dec) {
  return(((dec / 10) * 16) + (dec % 10));
}

// convert binary coded decimal to normal decimal
static uint8 ICACHE_FLASH_ATTR bcdToDec(uint8 bcd) {
  return(((bcd / 16) * 10) + (bcd % 16));
}




void ICACHE_FLASH_ATTR DS1307_Init()
{
uint8 buff[1];
//DS1307_send_buffer[0] = DS1307_init_1;
//DS1307_send_buffer[1] = DS1307_init_2;
buff[0] = 0x00;


	if (!i2c_Write_Addr_Buffer(DS1307_ADDR,DS1307_ADDR_CONTROL,buff,1,DS1307_Inited))	// ���� ���� ������
	{

		os_timer_disarm (&DS1307_timer_set);
		os_timer_setfn (&DS1307_timer_set, (os_timer_func_t *)DS1307_Init, NULL);
		os_timer_arm(&DS1307_timer_set, 2, 0);	// ��������� ������� ����� 2��
	}
}


static void ICACHE_FLASH_ATTR DS1307_Inited(void)
	{

		i2c_Do &= i2c_Free;			// ����������� ����

		if(i2c_Do & (i2c_ERR_NA|i2c_ERR_BF))	// ���� ������ �� �������
		{
			os_timer_disarm (&DS1307_timer_set);
			os_timer_setfn (&DS1307_timer_set, (os_timer_func_t *)DS1307_Init, NULL);
			os_timer_arm(&DS1307_timer_set, 2, 0);	// ��������� ������� ����� 2��
		}
		else
		{
			os_timer_disarm (&DS1307_timer_read);
			os_timer_setfn (&DS1307_timer_read, (os_timer_func_t *)DS1307_Read, NULL);
			os_timer_arm(&DS1307_timer_read, 1000, 0);	// ��������� ��������� �������
		}
	}



void ICACHE_FLASH_ATTR DS1307_Read (void)
{
//	INFO ("\r\nread 1\r\n");
	if (!i2c_Read_Addr_Buffer(DS1307_ADDR, 0x00, 7, DS1307_Readed) )      // ������
	{
//		INFO ("\r\nread 2\r\n");
		os_timer_disarm (&DS1307_timer_read);
		os_timer_setfn (&DS1307_timer_read, (os_timer_func_t *)DS1307_Read, NULL);
		os_timer_arm(&DS1307_timer_read, 2, 0);	// ��������� ������� ����� 2��
	}
}


static void ICACHE_FLASH_ATTR DS1307_Readed (void)
{
//	INFO ("\r\nread 3\r\n");
	i2c_Do &= i2c_Free;			// ����������� ����

	if(i2c_Do & (i2c_ERR_NA|i2c_ERR_BF))	// ���� ������ �� �������
	{
//		INFO ("\r\nread 4\r\n");
		os_timer_disarm (&DS1307_timer_read);
		os_timer_setfn (&DS1307_timer_read, (os_timer_func_t *)DS1307_Read, NULL);
		os_timer_arm(&DS1307_timer_read, 2, 0);	// ��������� ������� ����� 2��
	}
	else
	{

//		INFO ("\r\nread 5\r\n");
		DS1307_Time[0] = i2c_Buffer[0];         		// ����� �������� ���� �� ������ �������� � ����������
		DS1307_Time[1] = i2c_Buffer[1];         		// ����� �������� ���� �� ������ �������� � ����������
		DS1307_Time[2] = i2c_Buffer[2];
		DS1307_Date[0] = i2c_Buffer[4];
		DS1307_Date[1] = i2c_Buffer[5]&0b01111111;		//	������ �� - �������
		DS1307_Date[2] = i2c_Buffer[6];	//	г�, �� 0 �� 3
		DS1307_Date[3] = i2c_Buffer[3];	//	���� ������ �� 0 �� 6

/*
		char ci[21];
		os_sprintf(ci, "%02x:%02x:%02x %02x.%02x.20%02x",  DS1307_Time[2], DS1307_Time[1], DS1307_Time[0], DS1307_Date[0], DS1307_Date[1], DS1307_Date[2]);
//		os_sprintf(ci, "%02x:%02x:%02x",  DS1307_Time[2], DS1307_Time[1], DS1307_Time[0]);
		INFO("%s",ci);
*/

		os_timer_disarm (&DS1307_timer_read);
		os_timer_setfn (&DS1307_timer_read, (os_timer_func_t *)DS1307_Read, NULL);
		os_timer_arm(&DS1307_timer_read, 1000, 0);	// ��������� ��������� �������

	}
}

void ICACHE_FLASH_ATTR DS1307_Set_Date(void)
{
	os_timer_disarm (&DS1307_timer_read); //	��������� ������ ������� ����

/*
	DS1307_Date_to_Write[0] = 0x00;
	DS1307_Date_to_Write[1] = 0x38;
	DS1307_Date_to_Write[2] = 0x12;
	DS1307_Date_to_Write[3] = 0x03;
	DS1307_Date_to_Write[4] = 0x08;
	DS1307_Date_to_Write[5] = 0x02;
	DS1307_Date_to_Write[6] = 0x17;
*/

/*	DS1307_send_buffer[0] = DS1307_Date_to_Write[0];
	DS1307_send_buffer[1] = DS1307_Date_to_Write[1];
	DS1307_send_buffer[2] = DS1307_Date_to_Write[2];
	DS1307_send_buffer[3] = DS1307_Date_to_Write[3];
	DS1307_send_buffer[4] = DS1307_Date_to_Write[4];
	DS1307_send_buffer[5] = DS1307_Date_to_Write[5];
	DS1307_send_buffer[6] = DS1307_Date_to_Write[6];
*/
	if (!i2c_Write_Addr_Buffer(DS1307_ADDR,DS1307_ADDR_TIME,DS1307_Date_to_Write,7,DS1307_Date_Set))	// ���� ���� ������
	{
		os_timer_disarm (&DS1307_timer_set);
		os_timer_setfn (&DS1307_timer_set, (os_timer_func_t *)DS1307_Set_Date, NULL);
		os_timer_arm(&DS1307_timer_set, 5, 0);	// ��������� ������� ����� 2��
	}



}




static void ICACHE_FLASH_ATTR DS1307_Date_Set(void)
{

	i2c_Do &= i2c_Free;			// ����������� ����

	if(i2c_Do & (i2c_ERR_NA|i2c_ERR_BF))	// ���� ������ �� �������
	{
		os_timer_disarm (&DS1307_timer_set);
		os_timer_setfn (&DS1307_timer_set, (os_timer_func_t *)DS1307_Set_Date, NULL);
		os_timer_arm(&DS1307_timer_set, 2, 0);	// ��������� ������� ����� 2��
	}
	else
	{
		os_timer_disarm (&DS1307_timer_read);
		os_timer_setfn (&DS1307_timer_read, (os_timer_func_t *)DS1307_Read, NULL);
		os_timer_arm(&DS1307_timer_read, 1000, 0);	// ��������� ��������� �������


	}


}


