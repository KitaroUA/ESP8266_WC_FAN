#ifndef I2C_SHT3X_H
#define I2C_SHT3X_H


#include "user_interface.h"
#include <os_type.h>
#include "driver/my_i2c.h"
#include "math.h"
#include "user_config.h"



// extern uint8 i2c_DS1621_WriteByte(uint8 SAddr, uint8 Byte[], uint8 ByteNumbers, IIC_F WhatDo);
// extern uint8 i2c_SHT3X_ReadByte(uint8 SAddr, IIC_F WhatDo);


void StartWrite2SHT3X(void);
void Writed2SHT3X(void);
// void i2c_init_SHT3X (void);	//	Процедура ініціалізації датчиків, далі 4 підпроцедури
#define i2c_SHT3X_Init_Restart 100
#define i2c_SHT3X_Init_Error_Restart 50

// void i2c_SHT3X_Init_stage1 (void);
// void i2c_SHT3X_Init_stage2 (void);
// void i2c_SHT3X_Inited_Stage1 (void);
// void i2c_SHT3X_Inited_Stage2 (void);
void i2c_SHT3X_Read(void);	//	Процедура читання температури з датчика

#define i2c_SHT3X_First_Start 975	//	Затримка перед стартом конвеєру
#define i2c_SHT3X_Read_Restart 100	//	Затримка на повторну спробу, якщо шина зайнята
#define i2c_SHT3X_Read_Reread 5000	//	Затримка на повторне читання, читання циклічне, 10 сек (5 сек вологість, 5сек температура, 5 сек вологість....)
#define i2c_SHT3X_Rescan 10000	//	Затримка на повторне читання, читання циклічне, 10 сек (5 сек вологість, 5сек температура, 5 сек вологість....)

//	Мої змінні 
#define i2c_Buffer_Lenght 3
uint8 i2c_send_buffer[i2c_Buffer_Lenght];
//extern uint8 i2c_SHT3X_Temp[2];	//	Змінна в якій після читання буде лежати температура
//uint8 i2c_SHT3X_count;	// Кількість модулів SHT3X
//uint8 i2c_SHT3X_Addresses[8];	// Масив адресів модулів SHT3X, максимально може бути 8 модулів

uint8 i2c_SHT3X_Flags;	// 0bxxx1xxxxx - Закінчилась ініціалізація одного модуля
							// 0bx1xxxxxxx - Дані отримані
#define i2c_SHT3X_Address_Mask 0b00001111
#define i2c_SHT3X_Init_Mask 0b00100000
#define i2c_SHT3X_Read_Complete 0b01000000
#define i2c_SHT3X_Phase 0b00000001	//	Фаза читання датчику в конвеєрі.





#define i2c_SHT3X_Address 0x8A
#define i2c_SHT3X_Command_MSB 0x24
#define i2c_SHT3X_Command_LSB 0x00

void i2c_SHT3X_Start_Measuring (void);
void i2c_SHT3X_Start_Measuring_End (void);


void i2c_SHT3X_Start_Reading (void);
void i2c_SHT3X_Readed (void);


void i2c_SHT3X_Test(void);

float i2c_SHT3X_Temp;
float i2c_SHT3X_Hum;

uint8 i2c_SHT3X_errors;	//	Тут будуть лічильники помилок і прапорці; перші три біта - лічильник помилок i2c_ERR_NA, четвертий біт - прапорець - пристрій відстутній, або зламався.
#define i2c_SHT3X_errors_NA	0b00001000

os_timer_t SHT3X_timer_read;
os_timer_t SHT3X_timer_write;



#endif
