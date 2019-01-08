/*
 * encoder.c
 *
 *  Created on: 7 ���. 2017 �.
 *      Author: Kitaro
 */



#include <esp8266.h>



#include "driver/encoder.h"



// GPIO_PIN_INTR_NEGEDGE - down
// GPIO_PIN_INTR_POSEDGE - up
// GPIO_PIN_INTR_ANYEDGE - both
// GPIO_PIN_INTR_LOLEVEL - low level
// GPIO_PIN_INTR_HILEVEL - high level
// GPIO_PIN_INTR_DISABLE - disable interrupt
/*
const char *gpio_type_desc[] =
{
	    "GPIO_PIN_INTR_DISABLE (DISABLE INTERRUPT)",
	    "GPIO_PIN_INTR_POSEDGE (UP)",
	    "GPIO_PIN_INTR_NEGEDGE (DOWN)",
	    "GPIO_PIN_INTR_ANYEDGE (BOTH)",
	    "GPIO_PIN_INTR_LOLEVEL (LOW LEVEL)",
	    "GPIO_PIN_INTR_HILEVEL (HIGH LEVEL)"
};
*/
void ICACHE_FLASH_ATTR intr_reinit(void);


os_timer_t intr_reinit_timer;

/*


void ICACHE_FLASH_ATTR intr_callback(unsigned pin, unsigned level)
{
	os_printf("INTERRUPT: GPIO%d = %d\r\n %d \r\n", pin_num[pin], level, pin);
	if(pin == encoder_pin_1) {
		if (gpio_intr_deattach(pin))
			os_printf("GPIO%d deattach\r\n", pin_num[pin]);
		SetTimerTask(intr_reinit_timer,intr_reinit,5,0);
	}
//	os_printf("INTERRUPT: GPIO%d = %d\r\n %d \r\n", pin_num[pin], level, pin);
	if(pin == encoder_pin_2) {
		if (gpio_intr_deattach(pin))
			os_printf("GPIO%d deattach\r\n", pin_num[pin]);
		SetTimerTask(intr_reinit_timer,intr_reinit,5,0);
	}
}





void ICACHE_FLASH_ATTR encoder_gpio_init (void)
{
	GPIO_INT_TYPE gpio_type;
	uint8_t gpio_pin;

	// Test GPIO interrupt
	// Pin number 3 = GPIO0

	gpio_pin = encoder_pin_1;
	gpio_type = GPIO_PIN_INTR_ANYEDGE;
	if (set_gpio_mode(gpio_pin, GPIO_PULLUP, GPIO_INT)) {
		os_printf("GPIO%d set interrupt mode\r\n", pin_num[gpio_pin]);
		if (gpio_intr_init(gpio_pin, gpio_type)) {
			os_printf("GPIO%d enable %s mode\r\n", pin_num[gpio_pin], gpio_type_desc[gpio_type]);
			gpio_intr_attach(intr_callback);
		} else {
			os_printf("Error: GPIO%d not enable %s mode\r\n", pin_num[gpio_pin], gpio_type_desc[gpio_type]);
		}
	} else {
		os_printf("Error: GPIO%d not set interrupt mode\r\n", pin_num[gpio_pin]);
	}

	gpio_pin = encoder_pin_2;
	gpio_type = GPIO_PIN_INTR_ANYEDGE;
	if (set_gpio_mode(gpio_pin, GPIO_PULLUP, GPIO_INT)) {
		os_printf("GPIO%d set interrupt mode\r\n", pin_num[gpio_pin]);
		if (gpio_intr_init(gpio_pin, gpio_type)) {
			os_printf("GPIO%d enable %s mode\r\n", pin_num[gpio_pin], gpio_type_desc[gpio_type]);
			gpio_intr_attach(intr_callback);
		} else {
			os_printf("Error: GPIO%d not enable %s mode\r\n", pin_num[gpio_pin], gpio_type_desc[gpio_type]);
		}
	} else {
		os_printf("Error: GPIO%d not set interrupt mode\r\n", pin_num[gpio_pin]);
	}
*/
/*
}


void ICACHE_FLASH_ATTR intr_reinit(void)
{
	uint8 gpio_pin = encoder_pin_1;
	GPIO_INT_TYPE gpio_type;
	gpio_type = GPIO_PIN_INTR_ANYEDGE;


if (gpio_intr_init(gpio_pin, gpio_type))
	{
//	os_printf("GPIO%d enable %s mode\r\n", pin_num[gpio_pin], gpio_type_desc[gpio_type]);
		gpio_intr_attach(intr_callback);
	}

	gpio_pin = encoder_pin_2;
	gpio_type = GPIO_PIN_INTR_ANYEDGE;

	if (gpio_intr_init(gpio_pin, gpio_type))
	{
//	os_printf("GPIO%d enable %s mode\r\n", pin_num[gpio_pin], gpio_type_desc[gpio_type]);
		gpio_intr_attach(intr_callback);
	}*/

//}




