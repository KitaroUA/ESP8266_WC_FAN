/*
 * hd44780.c
 *
 *  Created on: 8 лют. 2017 р.
 *      Author: Kitaro
 */


#include "driver/hd44780.h"
// #include "asm2c.h"

uint8 i2c_LCD_CBF=0; 				// Флаги кольцевого буфера, на етапі ініціалізації ЖКІ - номер етапа ініціалізації.
uint8 i2c_LCD_Init_State=0;

uint8 i2c_LCD_CBD[i2c_LCD_CBL];
uint8 i2c_LCD_CBRP;				// Переменная, содержащая маркер чтения из кольцевого буфера, передачи на ЖКИ, указывает на !!_следующую_!! ячейку
uint8 i2c_LCD_CBWP;				// Переменная, содержащая маркер записи в кольцевой буфер

uint8 i2c_LCD_CBExch;				// Вспомогательная переменная для обмена данными между процедурами
uint8 i2c_LCD_CBData_Out;			// Переменная для вывода в порт
//uint8 i2c_LCD_CB_CGRAM_Counter;    // Счетчик для процедуры записи символов в ЖКИ
//uint8 LCD2PCF_send_buffer[4];

uint8 i2c_LCD_light_control=0;
uint8 LCD2PCF_send_buffer [4];


#define HI2PCA(X) (X&0b11110000)
#define LO2PCA(X) ((X&0b00001111)<<4)



void ICACHE_FLASH_ATTR i2c_LCD_Write_Task (void)	//	Таймерна процедура, основа для RTOS, перевіряє наявність даних в КБ, готових для видачі в індикатор, встановлює сама себе далі на викоонання
{

	if (((i2c_LCD_CBF)& ((1<<i2c_LCD_CBF_Start)|(1<<i2c_LCD_Initialized))) == ((1<<i2c_LCD_CBF_Start)|(1<<i2c_LCD_Initialized))) // Якщо передача даних активна
	{
		i2c_POP_LCD_CB();
//		SetTimerTask(hd44780_timer,i2c_LCD_Write_Task,i2c_LCD_Write_Task_Work,0);
	}
	else
	{
		SetTimerTask(hd44780_timer,i2c_LCD_Write_Task,i2c_LCD_Write_Task_Idle,0);
	}
}

//===============================================================
//========				i2c_LCD

void  ICACHE_FLASH_ATTR i2c_LCD_byte_sent (void)
{
	i2c_Do &= i2c_Free;                  		// Освобождаем шину

	if(i2c_Do & (i2c_ERR_NA|i2c_ERR_BF))   		// Ошибки при четнии были?
	{	//	Повторне читання даних з КБ
		SetTimerTask(hd44780_timer, i2c_LCD_Send_byte, 2, 0);
	}
	else
	{
		i2c_LCD_CBRP ++;
		if (i2c_LCD_CBRP == i2c_LCD_CBL) {i2c_LCD_CBRP=0;}	//	Скидання в "0" маркеру читання, якщо досягли кінця КБ

		if (i2c_LCD_CBWP == i2c_LCD_CBRP)
		{
			i2c_LCD_CBF &=(~(1<<i2c_LCD_CBF_Start)); // Знімаєм прапорець - передача активна
			i2c_LCD_CBF |=(1<<i2c_LCD_CBF_Empty);	 // Ставим прапорець - буфер порожній
		}
		SetTimerTask(hd44780_timer,i2c_LCD_Write_Task,i2c_LCD_Write_Task_Work,0);
			//	Постановка конвеєра на роботу далі.
	}

}
//void  ICACHE_FLASH_ATTR i2c_LCD_Send_byte (uint8 buffer, uint8 flag)
void  ICACHE_FLASH_ATTR i2c_LCD_Send_byte (void)
{

//uint8 t1;
	/*
LCD2PCF_send_buffer[1]=0;
LCD2PCF_send_buffer[3]=0;

if (flag == 1)
{		// Data
	 	LCD2PCF_send_buffer[1]=(1<<i2c_LCD_RS);
	 	LCD2PCF_send_buffer[3]=(1<<i2c_LCD_RS);
}
if (i2c_LCD_light_control != 0)
{
		LCD2PCF_send_buffer[1] |= (1<<i2c_LCD_Light);
		LCD2PCF_send_buffer[3] |= (1<<i2c_LCD_Light);
}
	LCD2PCF_send_buffer[1] |= HI2PCA(buffer);
	LCD2PCF_send_buffer[0] = LCD2PCF_send_buffer[1]|(1<<i2c_LCD_E);

	LCD2PCF_send_buffer[3] |= LO2PCA(buffer);
	LCD2PCF_send_buffer[2] = LCD2PCF_send_buffer[3]|(1<<i2c_LCD_E);
*/

//	i2c_PCF8574_Write(0x4e,LCD2PCF_send_buffer,4);
	if (!i2c_Write_Buffer(0x4e, LCD2PCF_send_buffer, 4, i2c_LCD_byte_sent) )      // Читаем
	{
//		SetTimerTask(hd44780_timer, i2c_POP_LCD_CB, 2, 0);
		SetTimerTask(hd44780_timer, i2c_LCD_Send_byte, 2, 0);
	}

}



void  ICACHE_FLASH_ATTR i2c_LCD_init(void)
{
	LCD2PCF_send_buffer[1]=0;
	LCD2PCF_send_buffer[3]=0;

switch (i2c_LCD_Init_State)
{
	case 0:
	{																	// Нульова фаза, переведдення портів в режим виводу, первинна затримка після подачі
																		// живлення на індікатор
																		// Затримка > 40 мсек.
					i2c_LCD_CBF |= (1<<i2c_LCD_CBF_Empty);              // встановлення флагу "кільцевий буфер вільний"
					i2c_LCD_CBRP = 0;									// Скидання маркерів читання та запису
					i2c_LCD_CBWP = 0;

	i2c_LCD_Init_State ++;
	SetTimerTask(hd44780_timer,i2c_LCD_init,i2c_LCD_init_Preinit,0);
	break;
	}

	case 1:									// Перша фаза ініціалізації, перший крок
	{										// перевод індікатора в режим 4-bit
			/*CBI(i2c_LCD_Contr_Port,i2c_LCD_RS)*/;
			LCD2PCF_send_buffer[1] &= (~(1<<i2c_LCD_RS));
			LCD2PCF_send_buffer[1] &=i2c_LCD_Data_Mask;
			LCD2PCF_send_buffer[1] |=0b00110000;
			LCD2PCF_send_buffer[0] = LCD2PCF_send_buffer[1]|(1<<i2c_LCD_E);
			i2c_PCF8574_Write(0x4e,LCD2PCF_send_buffer,2);
// 			Strob_E();
	i2c_LCD_Init_State ++;
	SetTimerTask(hd44780_timer,i2c_LCD_init,i2c_LCD_init_Delay,0);
	break;
	}
	case 2:									// Перша фаза ініціалізації, другий крок
	{
			LCD2PCF_send_buffer[1] &=i2c_LCD_Data_Mask;
			LCD2PCF_send_buffer[1] |=0b00110000;
			LCD2PCF_send_buffer[0] = LCD2PCF_send_buffer[1]|(1<<i2c_LCD_E);
					i2c_PCF8574_Write(0x4e,LCD2PCF_send_buffer,2);
	i2c_LCD_Init_State ++;
	SetTimerTask(hd44780_timer,i2c_LCD_init,i2c_LCD_init_Delay,0);
	break;
	}
	case 3:									// Друга фаза ініціалізації, перший крок 001 DL N F, DL=0 розрядність шини данних 4
	{										// перевод індікатора в режим 4-bit
			LCD2PCF_send_buffer[1] &=i2c_LCD_Data_Mask;
			LCD2PCF_send_buffer[1] |=0b00100000;
			LCD2PCF_send_buffer[0] = LCD2PCF_send_buffer[1]|(1<<i2c_LCD_E);
			i2c_PCF8574_Write(0x4e,LCD2PCF_send_buffer,2);
	i2c_LCD_Init_State ++;
	SetTimerTask(hd44780_timer,i2c_LCD_init,i2c_LCD_init_Delay,0);
	break;
	}
	case 4:									// Друга фаза ініціалізації, другий крок
	{
			LCD2PCF_send_buffer[1] &=i2c_LCD_Data_Mask;
			LCD2PCF_send_buffer[1] |=0b00100000;
			LCD2PCF_send_buffer[0] = LCD2PCF_send_buffer[1]|(1<<i2c_LCD_E);
			i2c_PCF8574_Write(0x4e,LCD2PCF_send_buffer,2);
	i2c_LCD_Init_State ++;
	SetTimerTask(hd44780_timer,i2c_LCD_init,i2c_LCD_init_Delay,0);
	break;
	}
	case 5:
	{										// Друга фаза ініціалізації, третій крок
			LCD2PCF_send_buffer[1] &=i2c_LCD_Data_Mask;		// Встановлення бітів N,F, N=1 (2-Line), F=0 font type 5x8 dots
			LCD2PCF_send_buffer[1] |=0b10000000;
			LCD2PCF_send_buffer[0] = LCD2PCF_send_buffer[1]|(1<<i2c_LCD_E);
			i2c_PCF8574_Write(0x4e,LCD2PCF_send_buffer,2);
	i2c_LCD_Init_State ++;
	SetTimerTask(hd44780_timer,i2c_LCD_init,i2c_LCD_init_Delay,0);
	break;
}
	case 6:									// Третя фаза ініціалізації
			{										// 0b00001 D C B, D=1 (Увімкнути дісплей), C=0 (курсор не вмикати), B=0 (курсор не блимає)
			LCD2PCF_send_buffer[1] &=i2c_LCD_Data_Mask;
			LCD2PCF_send_buffer[0] = LCD2PCF_send_buffer[1]|(1<<i2c_LCD_E);
			LCD2PCF_send_buffer[3] &=i2c_LCD_Data_Mask;
			LCD2PCF_send_buffer[3] |=0b11000000;
			LCD2PCF_send_buffer[2] = LCD2PCF_send_buffer[3]|(1<<i2c_LCD_E);
			i2c_PCF8574_Write(0x4e,LCD2PCF_send_buffer,4);
			i2c_LCD_Init_State ++;
			SetTimerTask(hd44780_timer,i2c_LCD_init,i2c_LCD_init_Delay,0);
			break;
			}
	case 7:									// Четверта фаза ініціалізації
			{										// Очистити дисплей
			LCD2PCF_send_buffer[1] &=i2c_LCD_Data_Mask;
			LCD2PCF_send_buffer[0] = LCD2PCF_send_buffer[1]|(1<<i2c_LCD_E);
			i2c_PCF8574_Write(0x4e,LCD2PCF_send_buffer,2);
			i2c_LCD_Init_State ++;
			SetTimerTask(hd44780_timer,i2c_LCD_init,i2c_LCD_init_Delay,0);
			break;
			}
	case 8:
			{
			LCD2PCF_send_buffer[1] &=i2c_LCD_Data_Mask;
			LCD2PCF_send_buffer[1] |=0b00010000;
			LCD2PCF_send_buffer[0] = LCD2PCF_send_buffer[1]|(1<<i2c_LCD_E);
			i2c_PCF8574_Write(0x4e,LCD2PCF_send_buffer,2);
			i2c_LCD_Init_State ++;
			SetTimerTask(hd44780_timer,i2c_LCD_init,i2c_LCD_init_Delay,0);
			break;
			}
	case 9:								// П'ята фаза ініціалізації
			{									// 0b000001 I/D SH, I/D=1 (курсор рухається праворуч), SH=0 (дисплей не зсувається)
			LCD2PCF_send_buffer[1] &=i2c_LCD_Data_Mask;
			LCD2PCF_send_buffer[0] = LCD2PCF_send_buffer[1]|(1<<i2c_LCD_E);
			LCD2PCF_send_buffer[3] &=i2c_LCD_Data_Mask;
			LCD2PCF_send_buffer[3] |=0b01100000;
			LCD2PCF_send_buffer[2] = LCD2PCF_send_buffer[3]|(1<<i2c_LCD_E);
			i2c_PCF8574_Write(0x4e,LCD2PCF_send_buffer,4);
			i2c_LCD_Init_State ++;
			SetTimerTask(hd44780_timer,i2c_LCD_init,i2c_LCD_init_Delay,0);
			break;
			}
	case 10:
			{
				i2c_LCD_CBF |= (1<<i2c_LCD_Initialized);
				i2c_LCD_light_control=1;
				SetTimerTask(hd44780_timer,i2c_LCD_Write_Task,10,0);
				break;
			}
	}
}



void  ICACHE_FLASH_ATTR Push_i2c_LCD_CB (char buffer)
{
if ((i2c_LCD_CBWP!=i2c_LCD_CBRP)||(i2c_LCD_CBF&(1<<i2c_LCD_CBF_Empty)))
	{
		i2c_LCD_CBF &= ~(1<<i2c_LCD_CBF_Empty);				//	Скидаємо флаг - КБ порожній
		i2c_LCD_CBD[i2c_LCD_CBWP]=buffer;
		i2c_LCD_CBWP ++;
		if (i2c_LCD_CBWP == i2c_LCD_CBL) {i2c_LCD_CBWP=0;}	//	Скидання в "0" маркеру читання, якщо досягли кінця КБ
	}
else
	{
			i2c_LCD_CBF |= (1<<i2c_LCD_CBF_Full); // Ставим флаг - буфер полон
			//cli(); //; Тут будет вызов процедуры, которая сообщит об ошибке переполнения кольцевого буфера, сейчас заглушим зависоном
			uint8 i;
			os_printf("Тут будет вызов процедуры, которая сообщит об ошибке переполнения кольцевого буфера HD44780\r\n");
			while (1)
			{
				i++;
			}
//		}
	}
}




void  ICACHE_FLASH_ATTR i2c_POP_LCD_CB(void)
{
//		uint8 t1;
//		t1=i2c_LCD_CBD[i2c_LCD_CBRP];
	uint8 flag = 0;
	//; Разбегаемся в зависимости от полученных данных
		if (i2c_LCD_CBD[i2c_LCD_CBRP] == i2c_LCD_CBComm)   // Проверяем, какой тип данных пишется в порт
		{							// Комманды, после определения типа управляющего байта, смещаем маркер, вызываем процедуру снова
			i2c_LCD_CBRP ++;									//	Пропускаємо службову інформацію з КБ
			if (i2c_LCD_CBRP == i2c_LCD_CBL) {i2c_LCD_CBRP=0;}	//	Скидання в "0" маркеру читання, якщо досягли кінця КБ
			flag = 0;
//			i2c_LCD_Send_byte(i2c_LCD_CBD[i2c_LCD_CBRP],0);
//			i2c_LCD_CBRP ++;
//			if (i2c_LCD_CBRP == i2c_LCD_CBL) {i2c_LCD_CBRP=0;}	//	Скидання в "0" маркеру читання, якщо досягли кінця КБ
		}
		else
		{
			if (i2c_LCD_CBD[i2c_LCD_CBRP] == i2c_LCD_CBData)   // Проверяем, какой тип данных пишется в порт
			{						  	// Комманды, после определения типа управляющего байта, смещаем маркер, вызываем процедуру снова
				i2c_LCD_CBRP ++;									//	Пропускаємо службову інформацію з КБ
				if (i2c_LCD_CBRP == i2c_LCD_CBL) {i2c_LCD_CBRP=0;}	//	Скидання в "0" маркеру читання, якщо досягли кінця КБ
				flag = 1;
//				i2c_LCD_Send_byte(i2c_LCD_CBD[i2c_LCD_CBRP],1);
//				i2c_LCD_CBRP ++;
//				if (i2c_LCD_CBRP == i2c_LCD_CBL) {i2c_LCD_CBRP=0;}	//	Скидання в "0" маркеру читання, якщо досягли кінця КБ
			}
			else
			{
				flag = 1;
//				i2c_LCD_Send_byte(i2c_LCD_CBD[i2c_LCD_CBRP],1);
//				i2c_LCD_CBRP ++;
//				if (i2c_LCD_CBRP == i2c_LCD_CBL) {i2c_LCD_CBRP=0;}	//	Скидання в "0" маркеру читання, якщо досягли кінця КБ
			}
		}
		LCD2PCF_send_buffer[1]=0;
		LCD2PCF_send_buffer[3]=0;

		if (flag == 1)
		{		// Data
			 	LCD2PCF_send_buffer[1]=(1<<i2c_LCD_RS);
			 	LCD2PCF_send_buffer[3]=(1<<i2c_LCD_RS);
		}
		if (i2c_LCD_light_control != 0)
		{
				LCD2PCF_send_buffer[1] |= (1<<i2c_LCD_Light);
				LCD2PCF_send_buffer[3] |= (1<<i2c_LCD_Light);
		}
			LCD2PCF_send_buffer[1] |= HI2PCA(i2c_LCD_CBD[i2c_LCD_CBRP]);
			LCD2PCF_send_buffer[0] = LCD2PCF_send_buffer[1]|(1<<i2c_LCD_E);

			LCD2PCF_send_buffer[3] |= LO2PCA(i2c_LCD_CBD[i2c_LCD_CBRP]);
			LCD2PCF_send_buffer[2] = LCD2PCF_send_buffer[3]|(1<<i2c_LCD_E);
			i2c_LCD_Send_byte();
/*
			i2c_LCD_CBRP ++;
			if (i2c_LCD_CBRP == i2c_LCD_CBL) {i2c_LCD_CBRP=0;}	//	Скидання в "0" маркеру читання, якщо досягли кінця КБ

			if (i2c_LCD_CBWP == i2c_LCD_CBRP)
			{
				i2c_LCD_CBF &=(~(1<<i2c_LCD_CBF_Start)); // Знімаєм прапорець - передача активна
				i2c_LCD_CBF |=(1<<i2c_LCD_CBF_Empty);	 // Ставим прапорець - буфер порожній
			}
			*/
}


// Процедура перетворення кірілічних кодів ASCII символів в коди індікатора HD44780
char  ICACHE_FLASH_ATTR i2c_LCD_Code_converter(char buffer) // Процедура перетворення кірілічних кодів ASCII символів в коди індікатора HD44780
{
//Заменяем/перекодируем номер символа, для нормального отображения кирилицы
        if ((buffer == 0xB2)||(buffer == 0xB3)||(buffer == 0xA8)||(buffer >= 0xC0))
        {
                switch (buffer)
                {
                        case 0xB2: return(0x49); break;//І
						case 0xB3: return(0x69); break;//і


						case 0xC0: return(0x41); break;//А
                        case 0xC1: return(0xA0); break;//Б
                        case 0xC2: return(0x42); break;//В
                        case 0xC3: return(0xA1); break;//Г
                        case 0xC4: return(0xE0); break;//Д
                        case 0xC5: return(0x45); break;//Е
                        case 0xA8: return(0xA2); break;//Ё
                        case 0xC6: return(0xA3); break;//Ж
                        case 0xC7: return(0xA4); break;//З
                        case 0xC8: return(0xA5); break;//И
                        case 0xC9: return(0xA6); break;//Й
                        case 0xCA: return(0x4B); break;//К
                        case 0xCB: return(0xA7); break;//Л
                        case 0xCC: return(0x4D); break;//М
                        case 0xCD: return(0x48); break;//H
                        case 0xCE: return(0x4F); break;//O
                        case 0xCF: return(0xA8); break;//П
                        case 0xD0: return(0x50); break;//P
                        case 0xD1: return(0x43); break;//C
                        case 0xD2: return(0x54); break;//T
                        case 0xD3: return(0xA9); break;//У
                        case 0xD4: return(0xAA); break;//Ф
                        case 0xD5: return(0x58); break;//X
                        case 0xD6: return(0xE1); break;//Ц
                        case 0xD7: return(0xAB); break;//Ч
                        case 0xD8: return(0xAC); break;//Ш
                        case 0xD9: return(0xE2); break;//Щ
                        case 0xDA: return(0xAD); break;//Ъ
                        case 0xDB: return(0xAE); break;//Ы
                        case 0xDC: return(0x62); break;//Ь
                        case 0xDD: return(0xAF); break;//Э
                        case 0xDE: return(0xB0); break;//Ю
                        case 0xDF: return(0xB1); break;//Я
                        case 0xE0: return(0x61); break;//а
                        case 0xE1: return(0xB2); break;//б
                        case 0xE2: return(0xB3); break;//в
                        case 0xE3: return(0xB4); break;//г
                        case 0xE4: return(0xE3); break;//д
                        case 0xE5: return(0x65); break;//е
                        case 0xB8: return(0xB5); break;//ё
                        case 0xE6: return(0xB6); break;//ж
                        case 0xE7: return(0xB7); break;//з
                        case 0xE8: return(0xB8); break;//и
                        case 0xE9: return(0xB9); break;//й
                        case 0xEA: return(0xBA); break;//к
                        case 0xEB: return(0xBB); break;//л
                        case 0xEC: return(0xBC); break;//м
                        case 0xED: return(0xBD); break;//н
                        case 0xEE: return(0x6F); break;//о
                        case 0xEF: return(0xBE); break;//п
                        case 0xF0: return(0x70); break;//р
                        case 0xF1: return(0x63); break;//с
                        case 0xF2: return(0xBF); break;//т
                        case 0xF3: return(0x79); break;//у
                        case 0xF4: return(0xE4); break;//ф
                        case 0xF5: return(0x78); break;//х
                        case 0xF6: return(0xE5); break;//ц
                        case 0xF7: return(0xC0); break;//ч
                        case 0xF8: return(0xC1); break;//ш
                        case 0xF9: return(0xE6); break;//щ
                        case 0xFA: return(0xC2); break;//ъ
                        case 0xFB: return(0xC3); break;//ы
                        case 0xFC: return(0xC4); break;//ь
                        case 0xFD: return(0xC5); break;//э
                        case 0xFE: return(0xC6); break;//ю
                        case 0xFF: return(0xC7); break;//я
                }

       }
				 return(buffer); // Якщо не попало в if , то повертаємо вхідні данні
}


void  ICACHE_FLASH_ATTR i2c_LCD_Send_String(uint8 row, uint8 col, char str_out[])
{
volatile uint8 adress = 0;
uint8 i = 0;
// cli();
	switch(row)														//	Формуємо команду з адресою знакомісця, куди починати писати рядок
	{
		case 0: adress = i2c_LCD_Send_String_FirstRow; break;
		case 1: adress = i2c_LCD_Send_String_SecondRow; break;
		case 2: adress = i2c_LCD_Send_String_ThirdRow; break;
		case 3: adress = i2c_LCD_Send_String_FourthRow; break;
		default: adress = i2c_LCD_Send_String_FirstRow; break;
	}
	if ((col >=0)&(col <=19))
	{
			adress += col;
	}
	else
	{
			adress += 0;
	}

	adress |= i2c_LCD_Send_String_Set_DDRAM_Address;

	Push_i2c_LCD_CB(i2c_LCD_CBComm);								//	Записуємо в КБ команду - далі йдуть команди.


	Push_i2c_LCD_CB(adress);

	Push_i2c_LCD_CB(i2c_LCD_CBData);								//	Записуємо в КБ команду  - далі йдуть дані.
	while ((str_out[i] != '\0')&((col+i)<i2c_LCD_Columns))											//	Пхаємо в КБ до кінця рядка, або поки не вийдемо за межі екрана...
	{
		Push_i2c_LCD_CB(i2c_LCD_Code_converter(str_out[i]));
		i++;
	}
	i2c_LCD_CBF |= (1<<i2c_LCD_CBF_Start);							//	Ставимо флаг, що в КБ є дані.
// sei();
}


//Тестова процедуре - переводить символ в його код
void  ICACHE_FLASH_ATTR i2c_LCD_Code_display(uint8 row, uint8 col, char buffer)  //Тестова процедуре - переводить символ в його код
{
	char ci3[3]; // Змінна на вихід
	char hulfbuffer;

	hulfbuffer = (buffer>>4)&0b00001111;


                switch (hulfbuffer)
                {
	                case 0x00: ci3[0]='0'; break;
	                case 0x01: ci3[0]='1'; break;
	                case 0x02: ci3[0]='2'; break;
	                case 0x03: ci3[0]='3'; break;
	                case 0x04: ci3[0]='4'; break;
	                case 0x05: ci3[0]='5'; break;
	                case 0x06: ci3[0]='6'; break;
	                case 0x07: ci3[0]='7'; break;
	                case 0x08: ci3[0]='8'; break;
	                case 0x09: ci3[0]='9'; break;
	                case 0x0A: ci3[0]='A'; break;
	                case 0x0B: ci3[0]='B'; break;
	                case 0x0C: ci3[0]='C'; break;
	                case 0x0D: ci3[0]='D'; break;
	                case 0x0E: ci3[0]='E'; break;
	                case 0x0F: ci3[0]='F'; break;

				}

	hulfbuffer = buffer&0b00001111;


	switch (hulfbuffer)
	{
		case 0x00: ci3[1]='0'; break;
		case 0x01: ci3[1]='1'; break;
		case 0x02: ci3[1]='2'; break;
		case 0x03: ci3[1]='3'; break;
		case 0x04: ci3[1]='4'; break;
		case 0x05: ci3[1]='5'; break;
		case 0x06: ci3[1]='6'; break;
		case 0x07: ci3[1]='7'; break;
		case 0x08: ci3[1]='8'; break;
		case 0x09: ci3[1]='9'; break;
		case 0x0A: ci3[1]='A'; break;
		case 0x0B: ci3[1]='B'; break;
		case 0x0C: ci3[1]='C'; break;
		case 0x0D: ci3[1]='D'; break;
		case 0x0E: ci3[1]='E'; break;
		case 0x0F: ci3[1]='F'; break;

	}

	ci3[2] = 0;
i2c_LCD_Send_String(row,col,ci3);
}
