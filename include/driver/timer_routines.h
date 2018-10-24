/*
 * timer_routines.h
 *
 *  Created on: 11 трав. 2018 р.
 *      Author: Kitaro
 */

#ifndef INCLUDE_DRIVER_TIMER_ROUTINES_H_
#define INCLUDE_DRIVER_TIMER_ROUTINES_H_

#include "user_config.h"

os_timer_t circular_timer;
uint8 one_sec_timer_xor1;
uint8 one_sec_task_state;



void init_circular_timer_proc();
void circular_timer_proc();


#endif /* INCLUDE_DRIVER_TIMER_ROUTINES_H_ */
