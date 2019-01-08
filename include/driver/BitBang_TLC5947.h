/*
 * BitBang_TLC5947.h
 *
 *  Created on: 27 груд. 2018 р.
 *      Author: Kitaro
 */

#ifndef INCLUDE_DRIVER_BITBANG_TLC5947_H_
#define INCLUDE_DRIVER_BITBANG_TLC5947_H_


#include <osapi.h>
#include <c_types.h>
#include "user_config.h"
#include <platform.h>


  void BitBang_TLC5947(uint16_t n, uint8_t c, uint8_t d, uint8_t l);

  bool BitBang_TLC5947_begin(void);

  void BitBang_TLC5947_setPWM(uint16_t chan, uint16_t pwm);
  void BitBang_TLC5947_write(void);



  uint16_t *pwmbuffer;

  uint16_t numdrivers;
  uint8_t _clk, _dat, _lat;



#endif /* INCLUDE_DRIVER_BITBANG_TLC5947_H_ */
