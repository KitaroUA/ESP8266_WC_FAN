/*
 * ntp.h
 *
 *  Created on: 30 жовт. 2017 р.
 *      Author: Kitaro
 */

#ifndef USER_PAGES_PAGE_NTP_H_
#define USER_PAGES_PAGE_NTP_H_



#include "cgi.h"



/*
 * set_ntp page
 */
int tpl_set_ntp(HttpdConnData *connData, char *token, void **arg);
int cgi_set_ntp(HttpdConnData *connData);
int cgi_apply_ntp(HttpdConnData *connData);
int cgi_PC_Time(HttpdConnData *connData);


#endif /* USER_PAGES_PAGE_NTP_H_ */
