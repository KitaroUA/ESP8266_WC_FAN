/*
 * slider.h
 *
 *  Created on: 2 січ. 2019 р.
 *      Author: Kitaro
 */

#ifndef USER_PAGES_SLIDER_H_
#define USER_PAGES_SLIDER_H_




#include "cgi.h"



/*
 * set_ntp page
 */
int tpl_slider(HttpdConnData *connData, char *token, void **arg);
int cgi_set_slider(HttpdConnData *connData);


#endif /* USER_PAGES_SLIDER_H_ */
