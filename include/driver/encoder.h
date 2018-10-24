/*
 * encoder.h
 *
 *  Created on: 7 бер. 2017 р.
 *      Author: Kitaro
 */

#ifndef INCLUDE_DRIVER_ENCODER_H_
#define INCLUDE_DRIVER_ENCODER_H_


#include "driver/gpio16.h"
#include "user_config.h"

#define encoder_pin_1 2	//D2pin
#define encoder_pin_2 3	//D3pin



extern uint8_t pin_num[GPIO_PIN_NUM];




void encoder_gpio_init (void);
void intr_reinit(void);
void intr_callback(unsigned pin, unsigned level);


#endif /* INCLUDE_DRIVER_ENCODER_H_ */
