/*
 * temperature_selector.c
 *
 *  Created on: 13 вер. 2017 р.
 *      Author: Kitaro
 */


#include <esp8266.h>
#include "pages/temperature_selector.h"
#include "io.h"
#include "user_config.h"
//#include "string.h"

uint8 temperature_options_current;










int ICACHE_FLASH_ATTR tpl_temperature_selector(HttpdConnData *connData, char *token, void **arg)
{

	char cj[80];
	char buff[128];
	if (token==NULL) return HTTPD_CGI_DONE;

	if (os_strcmp(token, "s_opt")==0)
	{
		os_sprintf(buff,"%d",number_of_temperature_options_channels);
	}
	memset(&cj[0], 0, sizeof(cj));

	if (os_strcmp(token, "r_opt")==0)
	{
		os_sprintf(buff,"%d",number_of_relay_channels);
	}
	memset(&cj[0], 0, sizeof(cj));

	if (os_strcmp(token, "p_opt")==0)
	{
		os_sprintf(buff,"%d",number_of_PWM_channels);
	}
	memset(&cj[0], 0, sizeof(cj));

	if (os_strcmp(token, "t_opt")==0)
	{

	}
	memset(&cj[0], 0, sizeof(cj));



















	httpdSend(connData, buff, -1);
	return HTTPD_CGI_DONE;
}


int ICACHE_FLASH_ATTR cgi_temperature_selector_1(HttpdConnData *connData) {
	int len;
	char buff[1024];

	if (connData->conn==NULL) {
		//Connection aborted. Clean up.
		return HTTPD_CGI_DONE;
	}




INFO("\r\n\r\n");

//	httpdRedirect(connData, "/temperature_selector/temperature_selector.tpl");
	return HTTPD_CGI_DONE;
}








int ICACHE_FLASH_ATTR var_temperature_selector(HttpdConnData *connData, char *token, void **arg)
{
	char cj[80];
	char buff[128];
	if (token==NULL) return HTTPD_CGI_DONE;
	INFO("\r\n\r\n getArgs:   ");
	INFO(connData->getArgs);
	INFO("\r\n\r\n");
	os_strcpy(cj,connData->getArgs);
	char * pch;
	pch = strtok (cj,"val=");
	temperature_options_current = atoi (pch);
	if ((temperature_options_current < 0)| (temperature_options_current>number_of_temperature_options_channels))
	{//Error, reload page
		httpdRedirect(connData, "/temperature_selector/temperature_selector.tpl");
		return HTTPD_CGI_DONE;
	}
	INFO("ch:%d",temperature_options_current);
	INFO("\r\n\r\n");






















	httpdSend(connData, buff, -1);
	return HTTPD_CGI_DONE;
}

