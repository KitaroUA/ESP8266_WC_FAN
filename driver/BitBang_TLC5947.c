/*
 * BitBang_TLC5947.c
 *
 *  Created on: 27 груд. 2018 р.
 *      Author: Kitaro
 */


#include <driver\BitBang_TLC5947.h>

void ICACHE_FLASH_ATTR  BitBang_TLC5947(uint16_t n, uint8_t c, uint8_t d, uint8_t l) {
  numdrivers = n;
  _clk = c;
  _dat = d;
  _lat = l;

  //pwmbuffer = (uint16_t *)calloc(2, 24*n);
  pwmbuffer = (uint16_t *)malloc(2 * 24*n);
  memset(pwmbuffer, 0, 2*24*n);
}

void ICACHE_FLASH_ATTR  BitBang_TLC5947_write(void) {
extern  uint8  DXpin_array[];


//  digitalWrite(_lat, LOW);
	GPIO_OUTPUT_SET(DXpin_array[_lat], 0);
  // 24 channels per TLC5974
  for (int16_t c=24*numdrivers - 1; c >= 0 ; c--) {
    // 12 bits per channel, send MSB first
    for (int8_t b=11; b>=0; b--) {
//      digitalWrite(_clk, LOW);
    	GPIO_OUTPUT_SET(DXpin_array[_clk], 0);

        if ((pwmbuffer[0] & (1 << b)) != 0)
//        if ((pwmbuffer[c] & (1 << b)) != 0)
      {
//        digitalWrite(_dat, HIGH);
      	  GPIO_OUTPUT_SET(DXpin_array[_dat], 1);
      }
      else
      {
//        digitalWrite(_dat, LOW);
	 	  GPIO_OUTPUT_SET(DXpin_array[_dat], 0);
      }

//      digitalWrite(_clk, HIGH);
  	  GPIO_OUTPUT_SET(DXpin_array[_clk], 1);

    }
  }
  //digitalWrite(_clk, LOW);
	  GPIO_OUTPUT_SET(DXpin_array[_clk], 0);
	  GPIO_OUTPUT_SET(DXpin_array[_dat], 0);

//  digitalWrite(_lat, HIGH);
//  digitalWrite(_lat, LOW);
	GPIO_OUTPUT_SET(DXpin_array[_lat], 1);
	GPIO_OUTPUT_SET(DXpin_array[_lat], 0);

}



void ICACHE_FLASH_ATTR  BitBang_TLC5947_setPWM(uint16_t chan, uint16_t pwm) {
  if (pwm > 4095) pwm = 4095;
  if (chan > 24*numdrivers) return;
  pwmbuffer[chan] = pwm;
}


bool ICACHE_FLASH_ATTR  BitBang_TLC5947_begin() {
  if (!pwmbuffer) return false;
extern  uint8 DXgpio_array[];
extern uint32 DXmux_array[];
//  pinMode(_clk, OUTPUT);
//  pinMode(_dat, OUTPUT);
//  pinMode(_lat, OUTPUT);
//PIN_FUNC_SELECT(RELAY_MUX, RELAY_FUNC);
  PIN_FUNC_SELECT(DXmux_array[_clk], DXgpio_array[_clk]);
  PIN_FUNC_SELECT(DXmux_array[_dat], DXgpio_array[_dat]);
  PIN_FUNC_SELECT(DXmux_array[_lat], DXgpio_array[_lat]);
//  digitalWrite(_lat, LOW);
  GPIO_OUTPUT_SET(_lat, 0);


  return true;
}

