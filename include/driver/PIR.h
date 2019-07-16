/*
 * PIR.h
 *
 *  Created on: 17 січ. 2019 р.
 *      Author: Kitaro
 */

#ifndef INCLUDE_DRIVER_PIR_H_
#define INCLUDE_DRIVER_PIR_H_

#include "user_config.h"
//================================ DEBUG
//#define PIR_debug
//================================ DEBUG

uint8_t PIR_flags;
#define PIR_idle 0x0
#define PIR_timer_template				0b11111110
#define PIR_movement_detected 			0b00000001
#define PIR_main_running		 		0b00000010
#define PIR_movement_detected_approved	0b00000100

//#define PIR_pre_latch 				0b00000001
//#define PIR_movement_detected 		0b00000011
//#define PIR_movement_stoped_latch	0b00000111



#define PIR_timer0 10000


//============ for load control
uint8_t FAN_timer1;
uint8_t FAN_timer2;
uint8_t FAN_timer1_startval;
uint16_t FAN_timer2_upperval;




void  ICACHE_FLASH_ATTR PIR_main_proc(void);

os_timer_t PIR_movement_timer;
void  ICACHE_FLASH_ATTR PIR_movement_set_falg(void);
void  ICACHE_FLASH_ATTR PIR_movement_timer_check(void);




#endif /* INCLUDE_DRIVER_PIR_H_ */
