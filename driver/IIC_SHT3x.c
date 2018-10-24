#include "driver/IIC_SHT3X.h"
#include <math.h>


#define HI(X) (X>>8)
#define LO(X) (X & 0xFF)

uint8 i2c_send_buffer[i2c_Buffer_Lenght];
uint8 i2c_SHT3X_Flags = 0;	// 0bxxx1xxxxx - Закінчилась ініціалізація одного модуля
							// 0bx1xxxxxxx - Дані отримані


//void DoNothing(void)	// Функция пустышка, затыкать несуществующие ссылки
//{
//}



void ICACHE_FLASH_ATTR i2c_SHT3X_Test(void)
{
	uint8 ans;
	uint8 delay;
	delay = (int)((rand()/RAND_MAX)*50);	//
	ans = i2c_Test_Write(i2c_SHT3X_Address);
	i2c_Do &= i2c_Free;
	if(ans == 1)
	{//Якщо датчик відгукнувся
		i2c_SHT3X_Flags |= i2c_SHT3X_Init_Mask;
		SetTimerTask(SHT3X_timer_write,i2c_SHT3X_Start_Measuring,i2c_SHT3X_First_Start+delay,0);
	}
	else
	{
		if (ans == 0)
		{// Шина зайнята, рестарт
			SetTimerTask(SHT3X_timer_write,i2c_SHT3X_Test,3,0);
		}
		if (ans == 255)
		{//Якщо датчик НЕ відгукнувся
		i2c_SHT3X_Flags &= (~i2c_SHT3X_Init_Mask);
		os_printf("\r\n No SHT3X found.... \r\n");
		SetTimerTask(SHT3X_timer_write,i2c_SHT3X_Test,i2c_SHT3X_Rescan,0);
		}
	}
}









uint8 ICACHE_FLASH_ATTR i2c_SHT3X_Crc(uint8 *data,uint8 nbrOfBytes)
{
	// CRC
	//const u16t POLYNOMIAL = 0x131; //P(x)=x^8+x^5+x^4+1 = 100110001
	//const u16t POLYNOMIAL = 0x31; //P(x)=x^8+x^5+x^4+1 = 100110001

	uint8 byteCtr,bit,crc;

	crc = 0xFF;

	//calculates 8-Bit checksum with given polynomial
	for (byteCtr = 0; byteCtr < nbrOfBytes; ++byteCtr)
	{
		crc ^= (data[byteCtr]);
		for (bit = 8; bit > 0; --bit)
		{
			if (crc & 0x80) crc = (crc << 1) ^ 0x31;
				else 		crc = (crc << 1);
		}
	}
	return(crc);
}


void ICACHE_FLASH_ATTR i2c_SHT3X_Start_Measuring (void)
{
	i2c_send_buffer[0] = i2c_SHT3X_Command_MSB;
	i2c_send_buffer[1] = i2c_SHT3X_Command_LSB;
				if (!i2c_Write_Buffer(i2c_SHT3X_Address,i2c_send_buffer,2,&i2c_SHT3X_Start_Measuring_End))	// Если байт не записался
				{
					SetTimerTask(SHT3X_timer_write,i2c_SHT3X_Start_Measuring,i2c_SHT3X_Init_Restart,0);			// Повторить попытку через 50мс
				}
}

void ICACHE_FLASH_ATTR i2c_SHT3X_Start_Measuring_End (void)
{
	i2c_Do &= i2c_Free;			// Освобождаем шину
				
				if(i2c_Do & (i2c_ERR_NA|i2c_ERR_BF))	// Если запись не удалась
				{
					if (!(i2c_SHT3X_errors & i2c_SHT3X_errors_NA))
					{
								SetTimerTask(SHT3X_timer_write,i2c_SHT3X_Start_Measuring,i2c_SHT3X_Init_Error_Restart,0);	// повторяем попытку
						if(i2c_Do & (i2c_ERR_NA))
						{
							i2c_SHT3X_errors++;
						}
					}

				}
				else
				{
					SetTimerTask(SHT3X_timer_read,i2c_SHT3X_Start_Reading,100,0);
					i2c_SHT3X_errors = 0;
				}

}




void ICACHE_FLASH_ATTR i2c_SHT3X_Start_Reading (void)
{
			if (!i2c_Read_Buffer(i2c_SHT3X_Address, 6, i2c_SHT3X_Readed) )      // Читаем
			{
				SetTimerTask(SHT3X_timer_read,i2c_SHT3X_Start_Reading,i2c_SHT3X_Read_Restart,0);                  		// Если процесс не пошел (шина занята), то повтор через 50мс.
			}
	
}

void ICACHE_FLASH_ATTR i2c_SHT3X_Readed (void)
{
	i2c_Do &= i2c_Free;                  		// Освобождаем шину
	
	if(i2c_Do & (i2c_ERR_NA|i2c_ERR_BF))   		// Ошибки при четнии были?
	{
					if (!(i2c_SHT3X_errors & i2c_SHT3X_errors_NA))
					{
						SetTimerTask(SHT3X_timer_read,i2c_SHT3X_Start_Reading,i2c_SHT3X_Read_Restart,0);     		 // Тогда повтор
						if(i2c_Do & (i2c_ERR_NA))
						{
							i2c_SHT3X_errors++;
						}
					}

	}
	else
	{
		i2c_SHT3X_errors = 0;
//		os_printf("\r\n Temp= %x %x %x \r\n", i2c_Buffer[0], i2c_Buffer[1], i2c_Buffer[2]);
//		os_printf("\r\n Hum= %x %x %x \r\n", i2c_Buffer[3], i2c_Buffer[4], i2c_Buffer[5]);



		uint8 crc_buffer[2];
		crc_buffer[0] = i2c_Buffer[0];
		crc_buffer[1] = i2c_Buffer[1];
		if(i2c_Buffer[2] == i2c_SHT3X_Crc(crc_buffer,2))
		{
	 		i2c_SHT3X_Temp = ((((i2c_Buffer[0] * 256.0) + i2c_Buffer[1]) * 175) / 65535.0) - 45;
//			os_printf("Temperature=%02d.%d\r\n", (int)i2c_SHT3X_Temp, (int)( (i2c_SHT3X_Temp-((int)i2c_SHT3X_Temp))*10.0) );
//			os_printf("\r\n Temp= %x %x %x \r\n", i2c_Buffer[0], i2c_Buffer[1], i2c_Buffer[2]);
		}
		else
		{
			os_printf("\r\nSHT3X Temperature CRC error\r\n");
		}

		crc_buffer[0] = i2c_Buffer[3];
		crc_buffer[1] = i2c_Buffer[4];
		if(i2c_Buffer[5] == i2c_SHT3X_Crc(crc_buffer,2))
		{
	 		i2c_SHT3X_Hum = ((((i2c_Buffer[3] * 256.0) + i2c_Buffer[4]) * 100) / 65535.0);
//			os_printf("Humidity=%02d\r\n", (int)i2c_SHT3X_Hum);
//			os_printf("\r\n Temp= %x %x %x \r\n", i2c_Buffer[0], i2c_Buffer[1], i2c_Buffer[2]);
		}
		else
		{
			os_printf("\r\nSHT3X Humidity CRC error\r\n");
		}






		SetTimerTask(SHT3X_timer_write,i2c_SHT3X_Start_Measuring,i2c_SHT3X_Read_Reread,0);
	}
	
}







