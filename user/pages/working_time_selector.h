/*
 * temperature_selector.h
 *
 *  Created on: 13 вер. 2017 р.
 *      Author: Kitaro
 */

#ifndef USER_PAGES_working_time_selector_H_
#define USER_PAGES_working_time_selector_H_

#include "cgi.h"


//Temperature_selector_struct Temperature_selector_array[number_of_temperature_channels] ;



//uint8 Temperature_display_array[2] ; // 0 Channel - in aqua temperature, 1 channel - local ambient temperature = № off temperature sensor


int tpl_working_time_selector(HttpdConnData *connData, char *token, void **arg);
int cgi_working_time_selector_1(HttpdConnData *connData);

#endif /* USER_PAGES_working_time_selector_H_ */
