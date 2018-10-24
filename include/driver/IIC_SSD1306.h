/*
 * IIC_SSD1306.h
 *
 *  Created on: 22 ����. 2017 �.
 *      Author: Kitaro
 */

#ifndef I2C_SSD1306_H
#define I2C_SSD1306_H


#include "user_interface.h"
#include <os_type.h>
#include "driver/my_i2c.h"
#include "user_config.h"






extern void i2c_SSD1306_Write(void);
extern void i2c_SSD1306_Write1Byte(uint8_t SAddr);

void i2c_SSD1306_Write1(void);
void i2c_SSD1306_Writed(void);

//====================================================================================================
#define i2c_SSD1306_Write_Restart 2		//	�������� �� �������� ������, ���� ���� �������.
#define i2c_SSD1306_Write_Error_Restart 2	//	�������� �� �������� ������, ���� ���� �������
#define i2c_SSD1306_Read_Restart 2		//	�������� �� �������� ������, ���� ���� �������.
#define i2c_SSD1306_Read_Error_Restart 2	//	�������� �� �������� ������, ���� ���� �������
//====================================================================================================



#define i2c_SSD1306_Max_X					128	//128 Pixels
#define i2c_SSD1306_Max_Y					32	//64  Pixels
#define i2c_SSD1306_Y_Sprite_Size			8
#define i2c_SSD1306_frame_Size				32
#define i2c_SSD1306_OLED_ADDRESS			0x78

// registers
#define i2c_SSD1306_COMMAND_MODE			0x80
#define i2c_SSD1306_DATA_MODE				0x40

// commands
#define i2c_SSD1306_CMD_DISPLAY_OFF			0xAE
#define i2c_SSD1306_CMD_DISPLAY_ON			0xAF
#define i2c_SSD1306_CMD_NORMAL_DISPLAY		0xA6
#define i2c_SSD1306_CMD_RESUME_DISPLAY		0xA4
#define i2c_SSD1306_CMD_INVERSE_DISPLAY		0xA7
#define i2c_SSD1306_CMD_SET_BRIGHTNESS		0x81

#define i2c_SSD1306_MEMORYMODE				0x20
#define i2c_SSD1306_COLUMNADDR				0x21
#define i2c_SSD1306_PAGEADDR				0x22

#define i2c_SSD1306_RIGHT_SCROLL			0x26
#define i2c_SSD1306_LEFT_SCROLL				0x27
#define i2c_SSD1306_SET_VERTICAL_SCROLL_AREA 0xA3
#define i2c_SSD1306_VERTICAL_RIGHT_SCROLL	0x29
#define i2c_SSD1306_VERTICAL_LEFT_SCROLL	0x2A
#define i2c_SSD1306_CMD_ACTIVATE_SCROLL		0x2F
#define i2c_SSD1306_CMD_DEACTIVATE_SCROLL	0x2E
#define i2c_SSD1306_CMD_Charge_Pump			0x8D

#define i2c_SSD1306_HORIZONTAL_ADDRESSING	0x00
#define i2c_SSD1306_PAGE_ADDRESSING			0x02

#define i2c_SSD1306_Scroll_Left				0x00
#define i2c_SSD1306_Scroll_Right			0x01
#define i2c_SSD1306_Scroll_Up				0x02
#define i2c_SSD1306_Scroll_Down				0x03

#define i2c_SSD1306_Scroll_2Frames			0x07
#define i2c_SSD1306_Scroll_3Frames			0x04
#define i2c_SSD1306_Scroll_4Frames			0x05
#define i2c_SSD1306_Scroll_5Frames			0x00
#define i2c_SSD1306_Scroll_25Frames			0x06
#define i2c_SSD1306_Scroll_64Frames			0x01
#define i2c_SSD1306_Scroll_128Frames		0x02
#define i2c_SSD1306_Scroll_256Frames		0x03


#define i2c_SSD1306_buffer_lenght 32					//	����. ������� ������, ��� ���� �������� SSD1306 �� ���� ������ ���������
uint8_t i2c_SSD1306_buffer[i2c_SSD1306_buffer_lenght];
uint8_t i2c_SSD1306_buffer_data;


#define i2c_SSD1306_CBL 300
uint8_t i2c_SSD1306_CBD[i2c_SSD1306_CBL];


void i2c_SSD1306_Init(void);
uint8_t i2c_SSD1306_Init_Stage;

void i2c_SSD1306_PowerOff(void);
void i2c_SSD1306_PowerOn(void);
void i2c_SSD1306_SetNormalDisplay(void);
void i2c_SSD1306_SetInverseDisplay(void);
void i2c_SSD1306_SetPageMode(void);
void i2c_SSD1306_SetBrightness(uint8_t Brightness);
void i2c_SSD1306_clearDisplay(void);
void i2c_SSD1306_SetCursorXY(uint8_t x, uint8_t y); // x = 0...127, y = 0...7
uint8 i2c_SSD1306_global_X, i2c_SSD1306_global_Y;
void i2c_SSD1306_SetStr5CursorXY(uint8_t x,uint8_t y);	//	���������� ��������� ������ ��� ����� 5�8, � � ����� 0...21, y � ����� 0..7
												//	����� ������� 5 ������ + 1 ������ �� ����� �� ��������

typedef enum
{
	SSDFONT_1X = 1,      // ������� ������ ������ 5x7
	SSDFONT_2X = 2       // ����������� ������ ������

} SSDLcdFontSize;
uint8_t i2c_SSD1306_Send_Char ( SSDLcdFontSize size, uint8_t ch , uint8 stage);
uint8_t i2c_SSD1306_Send_String ( SSDLcdFontSize size, uint8_t dataArray[] );
uint8_t ICACHE_FLASH_ATTR i2c_SSD1306_Send_Addr_String ( uint8_t x,uint8_t y, SSDLcdFontSize size, uint8_t dataArray[] );


void i2c_SSD1306_CB_POP (void);
void i2c_SSD1306_CB_POPED (void);

void i2c_SSD1306_CB_PUSH (uint8_t buffer[], uint8_t buffer_lenght);
//void i2c_SSD1306_CB_PUSH (uint8_t command,uint8_t buffer);

void ICACHE_FLASH_ATTR i2c_SSD1306_LCD_Update (void);
uint8_t ICACHE_FLASH_ATTR i2c_SSD1306_Send_GB_Addr_String ( uint8_t x,uint8_t y, SSDLcdFontSize size, uint8_t dataArray[] );
uint16_t global_pointer;
uint8_t ssd1306_global_buffer[i2c_SSD1306_Max_X * (i2c_SSD1306_Max_Y / 8)]; //	Screen buffer


os_timer_t SSD1306_timer;

os_timer_t SSD1306_CB_timer;
os_timer_t SSD1306_frame_CB_timer;




#endif /* INCLUDE_DRIVER_IIC_SSD1306_H_ */
