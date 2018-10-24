#ifndef __DS18B20_H__
#define __DS18B20_H__

#include "ets_sys.h"
#include "osapi.h"
#include "gpio.h"
#include "user_config.h"

#define DS18B20_MUX	 D3mux
#define DS18B20_FUNC D3f_g
#define DS18B20_PIN	 D3pin

#define DS1820_WRITE_SCRATCHPAD 	0x4E
#define DS1820_READ_SCRATCHPAD      0xBE
#define DS1820_COPY_SCRATCHPAD 		0x48
#define DS1820_READ_EEPROM 			0xB8
#define DS1820_READ_PWRSUPPLY 		0xB4
#define DS1820_SEARCHROM 			0xF0
#define DS1820_SKIP_ROM             0xCC
#define DS1820_READROM 				0x33
#define DS1820_MATCHROM 			0x55
#define DS1820_ALARMSEARCH 			0xEC
#define DS1820_CONVERT_T            0x44



#define OW_MAX_DEVICES 20
uint8_t devices[OW_MAX_DEVICES][8];
int ds18b20_temperature_arrey[OW_MAX_DEVICES];	//degree C, * 10000 just keeps us as an integer
int ds18b20_amount_of_devices;
int ds18b20_current_device;

static uint16_t oddparity[16] = {0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0};

void ds_init();
int ds_search(uint8_t *addr);
void select(const uint8_t rom[8]);
void skip();
void reset_search();
uint8_t reset(void);
void write(uint8_t v, int power);
void write_bit(int v);
uint8_t read();
int read_bit(void);
uint8_t crc8(const uint8_t *addr, uint8_t len);
uint16_t crc16(const uint16_t *data, const uint16_t  len);
void ds18b20_search (void);




void ICACHE_FLASH_ATTR ds18b20p1(void *arg);
void ICACHE_FLASH_ATTR ds18b20p2(void *arg);



os_timer_t ds18b20_timer;




#endif
