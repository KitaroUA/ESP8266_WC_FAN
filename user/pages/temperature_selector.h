/*
 * temperature_selector.h
 *
 *  Created on: 13 вер. 2017 р.
 *      Author: Kitaro
 */

#ifndef USER_PAGES_TEMPERATURE_SELECTOR_H_
#define USER_PAGES_TEMPERATURE_SELECTOR_H_

#include "cgi.h"




//Temperature_selector_struct Temperature_selector_array[number_of_temperature_channels] ;



//uint8 Temperature_display_array[2] ; // 0 Channel - in aqua temperature, 1 channel - local ambient temperature = № off temperature sensor


int tpl_temperature_selector(HttpdConnData *connData, char *token, void **arg);
int cgi_temperature_selector_1(HttpdConnData *connData);
int var_temperature_selector(HttpdConnData *connData, char *token, void **arg);

#endif /* USER_PAGES_TEMPERATURE_SELECTOR_H_ */
