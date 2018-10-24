/*
 * set_temp_on.h
 *
 *  Created on: 2 жовт. 2017 р.
 *      Author: Kitaro
 */

#ifndef USER_PAGES_SET_TEMP_ON_H_
#define USER_PAGES_SET_TEMP_ON_H_

#include "cgi.h"

int tpl_set_temp_on(HttpdConnData *connData, char *token, void **arg);
int cgi_set_temp_on(HttpdConnData *connData);


#endif /* USER_PAGES_SET_TEMP_ON_H_ */
