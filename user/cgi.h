#ifndef CGI_H
#define CGI_H

#include "httpd.h"


int cgiLed(HttpdConnData *connData);
int cgi_on_time(HttpdConnData *connData);
int cgi_off_time(HttpdConnData *connData);
int cgi_btn_time(HttpdConnData *connData);
int tplLed(HttpdConnData *connData, char *token, void **arg);
int tplshowTime(HttpdConnData *connData, char *token, void **arg);


/*
 * set_index
 */

/*
 * set_min_light page
 */
int tpl_set_min_light(HttpdConnData *connData, char *token, void **arg);
int cgi_set_min_light(HttpdConnData *connData);

/*
 * set_off_time page
 */
int tpl_set_off_time(HttpdConnData *connData, char *token, void **arg);
int cgi_set_off_time(HttpdConnData *connData);

/*
 * set_on_time page
 */
int tpl_set_on_time(HttpdConnData *connData, char *token, void **arg);
int cgi_set_on_time(HttpdConnData *connData);

/*
 * set_temp_off page
 */
int tpl_set_temp_off(HttpdConnData *connData, char *token, void **arg);
int cgi_set_temp_off(HttpdConnData *connData);







#endif
