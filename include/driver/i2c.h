/*
    I2C driver for the ESP8266 
    Copyright (C) 2014 Rudy Hardeman (zarya) 

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#ifndef __I2C_H__
#define __I2C_H__

#include "ets_sys.h"
#include "osapi.h"
#include "gpio.h"
#include "user_config.h"

//#define I2C_SLEEP_TIME 10
#define I2C_SLEEP_TIME 2
// SDA on GPIO2
//#define I2C_SDA_MUX PERIPHS_IO_MUX_GPIO2_U
//#define I2C_SDA_FUNC FUNC_GPIO2
//#define I2C_SDA_PIN 2


/* For D1 mini
// SDA on GPI
#define I2C_SDA_MUX D2mux
#define I2C_SDA_FUNC D2f_g
#define I2C_SDA_PIN D2pin


// SCK on GPIO
#define I2C_SCK_MUX D1mux
#define I2C_SCK_FUNC D1f_g
#define I2C_SCK_PIN D1pin
*/

/// For test nodeMCU
// SDA on GPI12
#define I2C_SDA_MUX D2mux
#define I2C_SDA_FUNC D2f_g
#define I2C_SDA_PIN D2pin


// SCK on GPIO14
#define I2C_SCK_MUX D1mux
#define I2C_SCK_FUNC D1f_g
#define I2C_SCK_PIN D1pin



//SCK on GPIO0 (untested)
//#define I2C_SCK_MUX PERIPHS_IO_MUX_GPIO0_U
//#define I2C_SCK_PIN 0
//#define I2C_SCK_FUNC FUNC_GPIO0

#define i2c_read() GPIO_INPUT_GET(GPIO_ID_PIN(I2C_SDA_PIN)); 

void ICACHE_FLASH_ATTR i2c_init(void);
void ICACHE_FLASH_ATTR i2c_start(void);
void ICACHE_FLASH_ATTR i2c_stop(void);
void ICACHE_FLASH_ATTR i2c_send_ack(uint8 state);
uint8 ICACHE_FLASH_ATTR i2c_check_ack(void);
uint8 ICACHE_FLASH_ATTR i2c_readByte(void);
void ICACHE_FLASH_ATTR i2c_writeByte(uint8 data);
void ICACHE_FLASH_ATTR i2c_sda(uint8 state);
void ICACHE_FLASH_ATTR i2c_sck(uint8 state);
#endif
