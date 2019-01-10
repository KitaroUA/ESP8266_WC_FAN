/*
 * IIC_DS3231.c
 *
 *  Created on: 2 ���. 2017 �.
 *      Author: Kitaro
 */

#include "driver/IIC_DS3231.h"




//uint8_t DS3231_send_buffer[DS3231_Buffer_Lenght];

uint8_t DS3231_Time[3];
uint8_t DS3231_Date[4];
uint16_t DS3231_Year_Base = 2000;

uint8_t DS3231_Date_to_Write[7];// ����� ��� ������ ���� � ��������. �������, �������, ������, ���� �����, ����, �����, ��.
uint8_t DS3231_ADDRESS = 0xD0;
uint8_t DS3231_Flags = 0;	// 0bxxx1xxxxx - ���������� ����������� ������ ������
							// 0bx1xxxxxxx - ��� �������





// convert normal decimal to binary coded decimal
static uint8 ICACHE_FLASH_ATTR decToBcd(uint8 dec) {
  return(((dec / 10) * 16) + (dec % 10));
}

// convert binary coded decimal to normal decimal
static uint8 ICACHE_FLASH_ATTR bcdToDec(uint8 bcd) {
  return(((bcd / 16) * 10) + (bcd % 16));
}




void ICACHE_FLASH_ATTR DS3231_Init()
{
uint8 buff[2];
//DS3231_send_buffer[0] = DS3231_init_1;
//DS3231_send_buffer[1] = DS3231_init_2;
buff[0] = DS3231_init_1;
buff[1] = DS3231_init_2;

	if (!i2c_Write_Addr_Buffer(DS3231_ADDRESS,DS3231_control_register_address,buff,2,DS3231_Inited))	// ���� ���� ������
	{

		os_timer_disarm (&DS3231_timer_set);
		os_timer_setfn (&DS3231_timer_set, (os_timer_func_t *)DS3231_Init, NULL);
		os_timer_arm(&DS3231_timer_set, 2, 0);	// ��������� ������� ����� 2��
	}
}


static void ICACHE_FLASH_ATTR DS3231_Inited(void)
	{

		i2c_Do &= i2c_Free;			// ����������� ����

		if(i2c_Do & (i2c_ERR_NA|i2c_ERR_BF))	// ���� ������ �� �������
		{
			os_timer_disarm (&DS3231_timer_set);
			os_timer_setfn (&DS3231_timer_set, (os_timer_func_t *)DS3231_Init, NULL);
			os_timer_arm(&DS3231_timer_set, 2, 0);	// ��������� ������� ����� 2��
		}
		else
		{
			os_timer_disarm (&DS3231_timer_read);
			os_timer_setfn (&DS3231_timer_read, (os_timer_func_t *)DS3231_Read, NULL);
			os_timer_arm(&DS3231_timer_read, 1000, 0);	// ��������� ��������� �������
		}
	}



void ICACHE_FLASH_ATTR DS3231_Read (void)
{
	if (!i2c_Read_Addr_Buffer(DS3231_ADDRESS, 0x00, 7, DS3231_Readed) )      // ������
	{
		os_timer_disarm (&DS3231_timer_read);
		os_timer_setfn (&DS3231_timer_read, (os_timer_func_t *)DS3231_Read, NULL);
		os_timer_arm(&DS3231_timer_read, 2, 0);	// ��������� ������� ����� 2��
	}
}


static void ICACHE_FLASH_ATTR DS3231_Readed (void)
{
	i2c_Do &= i2c_Free;			// ����������� ����

	if(i2c_Do & (i2c_ERR_NA|i2c_ERR_BF))	// ���� ������ �� �������
	{
		os_timer_disarm (&DS3231_timer_read);
		os_timer_setfn (&DS3231_timer_read, (os_timer_func_t *)DS3231_Read, NULL);
		os_timer_arm(&DS3231_timer_read, 2, 0);	// ��������� ������� ����� 2��
	}
	else
	{

		DS3231_Time[0] = i2c_Buffer[0];         		// ����� �������� ���� �� ������ �������� � ����������
		DS3231_Time[1] = i2c_Buffer[1];         		// ����� �������� ���� �� ������ �������� � ����������
		DS3231_Time[2] = i2c_Buffer[2];
		DS3231_Date[0] = i2c_Buffer[4];
		DS3231_Date[1] = i2c_Buffer[5]&0b01111111;		//	������ �� - �������
		DS3231_Date[2] = i2c_Buffer[6];	//	г�, �� 0 �� 3
		DS3231_Date[3] = i2c_Buffer[3];	//	���� ������ �� 0 �� 6


		os_timer_disarm (&DS3231_timer_read);
		os_timer_setfn (&DS3231_timer_read, (os_timer_func_t *)DS3231_Read, NULL);
		os_timer_arm(&DS3231_timer_read, 1000, 0);	// ��������� ��������� �������

	}
}

void ICACHE_FLASH_ATTR DS3231_Set_Date(void)
{
	os_timer_disarm (&DS3231_timer_read); //	��������� ������ ������� ����

/*
	DS3231_Date_to_Write[0] = 0x00;
	DS3231_Date_to_Write[1] = 0x38;
	DS3231_Date_to_Write[2] = 0x12;
	DS3231_Date_to_Write[3] = 0x03;
	DS3231_Date_to_Write[4] = 0x08;
	DS3231_Date_to_Write[5] = 0x02;
	DS3231_Date_to_Write[6] = 0x17;
*/

/*	DS3231_send_buffer[0] = DS3231_Date_to_Write[0];
	DS3231_send_buffer[1] = DS3231_Date_to_Write[1];
	DS3231_send_buffer[2] = DS3231_Date_to_Write[2];
	DS3231_send_buffer[3] = DS3231_Date_to_Write[3];
	DS3231_send_buffer[4] = DS3231_Date_to_Write[4];
	DS3231_send_buffer[5] = DS3231_Date_to_Write[5];
	DS3231_send_buffer[6] = DS3231_Date_to_Write[6];
*/
	if (!i2c_Write_Addr_Buffer(DS3231_ADDRESS,DS3231_date_register_address,DS3231_Date_to_Write,7,DS3231_Date_Set))	// ���� ���� ������
	{
		os_timer_disarm (&DS3231_timer_set);
		os_timer_setfn (&DS3231_timer_set, (os_timer_func_t *)DS3231_Set_Date, NULL);
		os_timer_arm(&DS3231_timer_set, 5, 0);	// ��������� ������� ����� 2��
	}



}




static void ICACHE_FLASH_ATTR DS3231_Date_Set(void)
{

	i2c_Do &= i2c_Free;			// ����������� ����

	if(i2c_Do & (i2c_ERR_NA|i2c_ERR_BF))	// ���� ������ �� �������
	{
		os_timer_disarm (&DS3231_timer_set);
		os_timer_setfn (&DS3231_timer_set, (os_timer_func_t *)DS3231_Set_Date, NULL);
		os_timer_arm(&DS3231_timer_set, 2, 0);	// ��������� ������� ����� 2��
	}
	else
	{
		os_timer_disarm (&DS3231_timer_read);
		os_timer_setfn (&DS3231_timer_read, (os_timer_func_t *)DS3231_Read, NULL);
		os_timer_arm(&DS3231_timer_read, 1000, 0);	// ��������� ��������� �������


	}


}


