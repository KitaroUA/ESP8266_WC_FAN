/*
 * options.c
 *
 *  Created on: 29 вер. 2017 р.
 *      Author: Kitaro
 */

#include <esp8266.h>
#include "pages/options.h"
#include "io.h"
#include "user_config.h"



int ICACHE_FLASH_ATTR tpl_options(HttpdConnData *connData, char *token, void **arg)
{
	char buff[128];
	if (token==NULL) return HTTPD_CGI_DONE;
	char cj[80];

	if (os_strcmp(token, "curTime")==0)
	{
		/*
			if (DS3231_Date[0] == 0)
			{	//���� ������� ���� ������� - ������ �� ��������
				os_strcpy(buff, "");
			}
			else
			{
				os_sprintf(cj, "%02x:%02x:%02x %02x.%02x.20%02x",  DS3231_Time[2], DS3231_Time[1], DS3231_Time[0], DS3231_Date[0], DS3231_Date[1], DS3231_Date[2]);
				os_strcpy(buff, cj);
			}*/
	}
	memset(&cj[0], 0, sizeof(cj));








	if (os_strcmp(token,"v_tempOn")==0)
	{
		os_strcpy(cj, "\"");
		os_strcat(cj, mFlag.tempOn_time);
		os_strcat(cj, "\"");
		os_sprintf(buff, cj);
	}
	memset(&cj[0], 0, sizeof(cj));










	httpdSend(connData, buff, -1);

	return HTTPD_CGI_DONE;
}





int ICACHE_FLASH_ATTR cgi_options(HttpdConnData *connData)
{
	int len;
	char buff[1024];
	if (connData->conn==NULL) {
		//Connection aborted. Clean up.
		return HTTPD_CGI_DONE;
	}
	len=httpdFindArg(connData->post->buff, "switch_1", buff, sizeof(buff));
	if (len!=0)
	{
	}
	memset(&buff[0], 0, sizeof(buff));


	httpdRedirect(connData, "/options/options.tpl");
	return HTTPD_CGI_DONE;
}
