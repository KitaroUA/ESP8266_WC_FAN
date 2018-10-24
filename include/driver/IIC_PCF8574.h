/*
 * IIC_PCF8574.h
 *
 *  Created on: 8 лют. 2017 р.
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
#define i2c_PCF8574_Write_Restart 4		//	Затримка на повторну спробу, якщо шина зайнята.
#define i2c_PCF8574_Write_Error_Restart 4	//	Затримка на повторну спробу, якщо були помилки
#define i2c_PCF8574_Read_Restart 4		//	Затримка на повторну спробу, якщо шина зайнята.
#define i2c_PCF8574_Read_Error_Restart 4	//	Затримка на повторну спробу, якщо були помилки
//====================================================================================================

void i2c_PCF8574_Read(void);
void i2c_PCF8574_Read1(void);
void i2c_PCF8574_Readed(void);

//	Мої змінні
#define i2c_PCF8574_buffer_lenght 5					//	Макс. довжина мачиву, яку може обробити PCF8574 за один виклик процедури
uint8 i2c_PCF8574_buffer[i2c_PCF8574_buffer_lenght];
uint8 i2c_PCF8574_buffer_data;
uint8 i2c_PCF8574_Address;

//uint8 i2c_PCF8574_count;	// Кількість модулів PCF8574
//uint8 i2c_PCF8574_Addresses[8];	// Маств адрес модулів PCF8574, максимально може бути 2 модулі

os_timer_t i2c_PCF8574_timer_set;


#endif /* INCLUDE_DRIVER_IIC_PCF8574_H_ */
