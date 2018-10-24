/*
 * options.h
 *
 *  Created on: 29 вер. 2017 р.
 *      Author: Kitaro
 */

#ifndef USER_PAGES_OPTIONS_H_
#define USER_PAGES_OPTIONS_H_

//#include "httpd.h"
#include "cgi.h"

int tpl_options(HttpdConnData *connData, char *token, void **arg);
int cgi_options(HttpdConnData *connData);


#endif /* USER_PAGES_OPTIONS_H_ */
