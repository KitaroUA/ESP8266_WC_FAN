/*
 * set_ip.h
 *
 *  Created on: 2 січ. 2018 р.
 *      Author: Kitaro
 */

#ifndef USER_PAGES_SET_IP_H_
#define USER_PAGES_SET_IP_H_
#include "cgi.h"

int tpl_set_ip(HttpdConnData *connData, char *token, void **arg);
int cgi_set_ip_1(HttpdConnData *connData);
int cgi_set_ip_2(HttpdConnData *connData);



#endif /* USER_PAGES_SET_IP_H_ */
