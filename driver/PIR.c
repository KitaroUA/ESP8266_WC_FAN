/*
 * PIR.c
 *
 *  Created on: 17 січ. 2019 р.
 *      Author: Kitaro
 */


#include "driver/PIR.h"
uint8_t PIR_flags = 0;
uint8_t FAN_timer1 = 0;
uint8_t FAN_timer2 = 0;


void  ICACHE_FLASH_ATTR PIR_movement_set_falg()
{
#ifdef PIR_debug
	INFO("\r\nset flag, %c%c%c%c%c%c%c%c\r\n",BYTE_TO_BINARY(PIR_flags&PIR_movement_detected));
	INFO("\r\nPIR PIN %d \r\n", GPIO_INPUT_GET(PIR_PIN));
#endif
	PIR_flags |= PIR_movement_detected;
	SetTimerTask (PIR_movement_timer, PIR_movement_timer_check, PIR_timer0, 0);
}


void  ICACHE_FLASH_ATTR PIR_movement_timer_check()
{
#ifdef PIR_debug
	INFO ("\r\npir timer check\r\n");
#endif
	if (GPIO_INPUT_GET(PIR_PIN) == 1)
	{
#ifdef PIR_debug
	INFO ("\r\nextend movement timer\r\n");
#endif
		SetTimerTask (PIR_movement_timer, PIR_movement_timer_check, PIR_timer0, 0);
	}
	else	//	Руху нема
	{
		PIR_flags &= !(PIR_movement_detected);
#ifdef PIR_debug
		INFO ("pir timer reset flag");
#endif
	}
}



void  ICACHE_FLASH_ATTR PIR_main_proc()
{
	if ((PIR_flags&PIR_main_running) == 0)
	{	//	Перший вхід в процес

	}

}
