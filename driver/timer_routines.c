/*
 * timer_routines.c
 *
 *  Created on: 10 трав. 2018 р.
 *      Author: Kitaro
 */

#include "driver/timer_routines.h"

uint8 one_sec_task_state = 0;

void every_second_task();
void every_5_second_task();



static uint8 ICACHE_FLASH_ATTR bcdToDec(uint8 bcd) {
  return(((bcd / 16) * 10) + (bcd % 16));
}
static uint8 ICACHE_FLASH_ATTR decToBcd(uint8 dec) {
  return(((dec / 10) * 16) + (dec % 10));
}

void ICACHE_FLASH_ATTR init_circular_timer_proc(void)
{
	circular_timer_proc();
}

void ICACHE_FLASH_ATTR circular_timer_proc(void)
{

	//		uint8 sec = bcdToDec(DS3231_Time[0]);
	//		uint8 min = bcdToDec(DS3231_Time[1]);
	//		uint8 hr  = bcdToDec(DS3231_Time[2]);
			uint8 sec = bcdToDec(0);
			uint8 min = bcdToDec(0);
			uint8 hr  = bcdToDec(0);

		every_second_task();

		if(sec%5 == 0)	//	Every 5 sec task
		{
			every_5_second_task();
		}


			if ( (temporary_light_off_timer != 0))
			{
				temporary_light_off_timer = temporary_light_off_timer-1;
	//			os_printf("                                        temp_off_timer = %d \r\n", temp_off_timer);
			}
			if ( (temporary_light_on_timer != 0))
			{
				temporary_light_on_timer = temporary_light_on_timer-1;
	//			os_printf("                                        temp_off_timer = %d \r\n", temp_off_timer);
			}


	/*
			   ets_intr_lock(); // These seem to be OK
			   for (uint8 pixel = 0; pixel < PIXEL_COUNT; pixel++)
					ws2812_sendPixel((one_sec_task_state*255)/10,((10-one_sec_task_state)*255)/10,0);
			   ets_intr_unlock(); // These seem to be OK
			   ws2812_show();  // latch the colors
	*/


	//		ws2812_sendPixel(0,128,0);
	//		ws2812_sendPixel(0,0,128);
















		if (wifi_station_get_connect_status() == 5)
		{

		}

		if (sec == 0)	//	Task on every minute start
		{



		}


		// Перевірка кожну годину
	//	if ( (DS3231_Time[1] == 0) & (DS3231_Time[0] == 0))
	//	if ( (DS3231_Time[1] != 0) & (DS3231_Time[0] != 0))
		if ( (1 == 0) & (0 == 0))
		{
			INFO ("\n ------------------an hour------------------------- \r\n");



			// Check is DST time
/*
			if ( ((DS3231_Time[1] == 3)&(mFlag.dst_active=0)) | ((DS3231_Time[0] == 4)&(mFlag.dst_active=1)))
			{
			struct tm *dt;
			time_t timestamp = 1522921929;
	//		timestamp = 1522921929;
			dt = gmtime(&timestamp);
			dt->tm_sec  = bcdToDec(DS3231_Time[0]);
			dt->tm_min  = bcdToDec(DS3231_Time[1]);
			dt->tm_hour = bcdToDec(DS3231_Time[2]);
			dt->tm_mday = bcdToDec(DS3231_Date[0]);
			dt->tm_mon  = bcdToDec(DS3231_Date[1]&0b01111111) - 1;
			dt->tm_year = bcdToDec(DS3231_Date[2])+100;
			dt->tm_wday = DS3231_Date[3];
			if (dt->tm_wday == 7)
			{
				dt->tm_wday = 0; //� NTP ���� ����� �� 0 �� 6 ��������� � �����, ��������� � ����� �� 1 �� 7
			}


			char cj[80];
			check_dst(dt);
			os_sprintf(cj, "gmt: %d %02d:%02d:%02d %02d.%02d.%04d, %d",  dt->tm_isdst, dt->tm_hour, dt->tm_min, dt->tm_sec, dt->tm_mday, dt->tm_mon + 1, dt->tm_year + 1900, dt->tm_wday);
			os_printf(cj);
			os_printf("\r\n");
			if(dt->tm_isdst != mFlag.dst_active)
			{
				if (dt->tm_isdst != 0)
				{
					INFO ("it`s summer time!!!! :)");
					dt->tm_hour += 1;
				}
				else
				{
					INFO ("it`s winter time :(");
					dt->tm_hour -= 1;

				}
				mFlag.dst_active = dt->tm_isdst;
				AddCFG_Save();
				mktime(dt);


				os_sprintf(cj, "new time: %d %02d:%02d:%02d %02d.%02d.%04d, %d",  dt->tm_isdst, dt->tm_hour, dt->tm_min, dt->tm_sec, dt->tm_mday, dt->tm_mon + 1, dt->tm_year + 1900, dt->tm_wday);
				os_printf(cj);
				os_printf("\r\n");




				DS3231_Date_to_Write[0] = decToBcd(dt->tm_sec);
				DS3231_Date_to_Write[1] = decToBcd(dt->tm_min);
				DS3231_Date_to_Write[2] = decToBcd(dt->tm_hour);
				DS3231_Date_to_Write[3] = decToBcd(dt->tm_wday);
				DS3231_Date_to_Write[4] = decToBcd(dt->tm_mday);
				DS3231_Date_to_Write[5] = decToBcd(dt->tm_mon + 1);
				DS3231_Date_to_Write[6] = decToBcd(dt->tm_year - 100);
				DS3231_Set_Date();
*/
			/*

			}
			}
			// End check is DST time

			*/



		}




		one_sec_task_state++;
		if (one_sec_task_state >=10)
		{
			one_sec_task_state = 0;

		}


	//	one_sec_timer_xor1 ^= 0xff;
		one_sec_timer_xor1 ^= 0b00000010;
		uptime++;
		SetTimerTask (circular_timer, circular_timer_proc, 1000, 0);
}






void ICACHE_FLASH_ATTR every_second_task()
{





















}




void ICACHE_FLASH_ATTR every_5_second_task ()
{





}

