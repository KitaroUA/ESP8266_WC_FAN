/*
 * set_ip.c
 *
 *  Created on: 2 січ. 2018 р.
 *      Author: Kitaro
 */


#include <esp8266.h>
#include "pages/set_ip.h"
#include "io.h"
#include "user_config.h"
#include "ip_addr.h"



int ICACHE_FLASH_ATTR tpl_set_ip(HttpdConnData *connData, char *token, void **arg)
{
	char cj[80];
	char buff[128];
	if (token==NULL) return HTTPD_CGI_DONE;

//----------------------
//	Station ip config
//----------------------

	if (os_strcmp(token, "sta_ip_addr")==0)
	{
		os_sprintf(cj,"%d.%d.%d.%d", 		 ip4_addr1(&sysCfg.staipinfo.ip),\
											 ip4_addr2(&sysCfg.staipinfo.ip),\
											 ip4_addr3(&sysCfg.staipinfo.ip),\
											 ip4_addr4(&sysCfg.staipinfo.ip) );
		os_strcpy(buff,cj);
	}
	memset(&cj[0], 0, sizeof(cj));

	if (os_strcmp(token, "sta_ip_mask")==0)
	{
		os_sprintf(cj,"%d.%d.%d.%d",		 ip4_addr1(&sysCfg.staipinfo.netmask),\
											 ip4_addr2(&sysCfg.staipinfo.netmask),\
											 ip4_addr3(&sysCfg.staipinfo.netmask),\
											 ip4_addr4(&sysCfg.staipinfo.netmask) );
		os_strcpy(buff,cj);
	}
	memset(&cj[0], 0, sizeof(cj));


	if (os_strcmp(token, "sta_ip_gw")==0)
	{
		os_sprintf(cj,"%d.%d.%d.%d", 		 ip4_addr1(&sysCfg.staipinfo.gw),\
											 ip4_addr2(&sysCfg.staipinfo.gw),\
											 ip4_addr3(&sysCfg.staipinfo.gw),\
											 ip4_addr4(&sysCfg.staipinfo.gw) );
		os_strcpy(buff,cj);
	}
	memset(&cj[0], 0, sizeof(cj));


//--------------------
//	Soft AP ip config
//--------------------
	if (os_strcmp(token, "sap_ip_addr")==0)
	{
		os_sprintf(cj,"%d.%d.%d.%d", 		 ip4_addr1(&sysCfg.apipinfo.ip),\
											 ip4_addr2(&sysCfg.apipinfo.ip),\
											 ip4_addr3(&sysCfg.apipinfo.ip),\
											 ip4_addr4(&sysCfg.apipinfo.ip) );
		os_strcpy(buff,cj);
	}
	memset(&cj[0], 0, sizeof(cj));

	if (os_strcmp(token, "sap_ip_mask")==0)
	{
		os_sprintf(cj,"%d.%d.%d.%d",		 ip4_addr1(&sysCfg.apipinfo.netmask),\
											 ip4_addr2(&sysCfg.apipinfo.netmask),\
											 ip4_addr3(&sysCfg.apipinfo.netmask),\
											 ip4_addr4(&sysCfg.apipinfo.netmask) );
		os_strcpy(buff,cj);
	}
	memset(&cj[0], 0, sizeof(cj));


	if (os_strcmp(token, "sap_ip_gw")==0)
	{
		os_sprintf(cj,"%d.%d.%d.%d", 		 ip4_addr1(&sysCfg.apipinfo.gw),\
											 ip4_addr2(&sysCfg.apipinfo.gw),\
											 ip4_addr3(&sysCfg.apipinfo.gw),\
											 ip4_addr4(&sysCfg.apipinfo.gw) );
		os_strcpy(buff,cj);
	}
	memset(&cj[0], 0, sizeof(cj));


	if (os_strcmp(token, "dhcp_checked")==0)
	{
		if (sysCfg.dhcp_sta_enabled == 1)
		{
			os_sprintf(cj,"checked");
		}
		else
		{
			os_sprintf(cj," ");
		}
		os_strcpy(buff,cj);
	}
	memset(&cj[0], 0, sizeof(cj));

	httpdSend(connData, buff, -1);
	return HTTPD_CGI_DONE;
}

struct ip_addr ICACHE_FLASH_ATTR input_to_ip (char* in_str )
{
char * pch;
struct ip_addr ip_addr_fn;
uint8_t ip_a,ip_b,ip_c,ip_d;

pch = strtok (in_str,".");
ip_a = atoi (pch);
//INFO(pch);
//INFO("\r\n\r\n");
pch = strtok (NULL, ".");
ip_b = atoi (pch);
//INFO(pch);
//INFO("\r\n\r\n");
pch = strtok (NULL, ".");
ip_c = atoi (pch);
//INFO(pch);
//INFO("\r\n\r\n");
pch = strtok (NULL, ".");
ip_d = atoi (pch);
//INFO(pch);
//INFO("\r\n\r\n");
IP4_ADDR(&ip_addr_fn, ip_a,ip_b,ip_c,ip_d);
return ip_addr_fn;
}
int ICACHE_FLASH_ATTR cgi_set_ip_1(HttpdConnData *connData)
{
	int len;
	char buff[1024];
	struct ip_addr ip_addr_fn;
	if (connData->conn==NULL) {
		//Connection aborted. Clean up.
		return HTTPD_CGI_DONE;

	}
//--------------- ip addr
	len=httpdFindArg(connData->post->buff, "sta_ip_addr", buff, sizeof(buff));
	if (len!=0)
	{
//		INFO("\r\n");
		sysCfg.staipinfo.ip = input_to_ip(buff);
		INFO("station ip address: %d.%d.%d.%d", ip4_addr1(&sysCfg.staipinfo.ip),\
												ip4_addr2(&sysCfg.staipinfo.ip),\
												ip4_addr3(&sysCfg.staipinfo.ip),\
												ip4_addr4(&sysCfg.staipinfo.ip) );

//		INFO("\r\n");
	}
//--------------- ip addr mask
	len=httpdFindArg(connData->post->buff, "sta_ip_mask", buff, sizeof(buff));
	if (len!=0)
	{
//		INFO("\r\n");
		sysCfg.staipinfo.netmask = input_to_ip(buff);
		INFO("station ip address mask: %d.%d.%d.%d", ip4_addr1(&sysCfg.staipinfo.netmask),\
												ip4_addr2(&sysCfg.staipinfo.netmask),\
												ip4_addr3(&sysCfg.staipinfo.netmask),\
												ip4_addr4(&sysCfg.staipinfo.netmask) );
//		INFO("\r\n");
	}
//--------------- ip addr gw
	len=httpdFindArg(connData->post->buff, "sta_ip_gw", buff, sizeof(buff));
	if (len!=0)
	{
//		INFO("\r\n");
		sysCfg.staipinfo.gw = input_to_ip(buff);
		INFO("station gw: %d.%d.%d.%d", ip4_addr1(&sysCfg.staipinfo.gw),\
												ip4_addr2(&sysCfg.staipinfo.gw),\
												ip4_addr3(&sysCfg.staipinfo.gw),\
												ip4_addr4(&sysCfg.staipinfo.gw) );
//		INFO("\r\n");
	}



	//--------------- ip addr
		len=httpdFindArg(connData->post->buff, "sap_ip_addr", buff, sizeof(buff));
		if (len!=0)
		{
//			INFO("\r\n");
			sysCfg.apipinfo.ip = input_to_ip(buff);
			INFO("Soft AP ip address: %d.%d.%d.%d", ip4_addr1(&sysCfg.apipinfo.ip),\
													ip4_addr2(&sysCfg.apipinfo.ip),\
													ip4_addr3(&sysCfg.apipinfo.ip),\
													ip4_addr4(&sysCfg.apipinfo.ip) );
//			INFO("\r\n");
		}
	//--------------- ip addr mask
		len=httpdFindArg(connData->post->buff, "sap_ip_mask", buff, sizeof(buff));
		if (len!=0)
		{
//			INFO("\r\n");
			sysCfg.apipinfo.netmask = input_to_ip(buff);
			INFO("Soft AP ip address mask: %d.%d.%d.%d", ip4_addr1(&sysCfg.apipinfo.netmask),\
														 ip4_addr2(&sysCfg.apipinfo.netmask),\
														 ip4_addr3(&sysCfg.apipinfo.netmask),\
														 ip4_addr4(&sysCfg.apipinfo.netmask) );
//			INFO("\r\n");
		}
	//--------------- ip addr gw
		len=httpdFindArg(connData->post->buff, "sap_ip_gw", buff, sizeof(buff));
		if (len!=0)
		{
//			INFO("\r\n");
			sysCfg.apipinfo.gw = input_to_ip(buff);
			INFO("Soft AP ip address gw: %d.%d.%d.%d", ip4_addr1(&sysCfg.apipinfo.gw),\
														 ip4_addr2(&sysCfg.apipinfo.gw),\
														 ip4_addr3(&sysCfg.apipinfo.gw),\
														 ip4_addr4(&sysCfg.apipinfo.gw) );
//			INFO("\r\n");
		}

//--------------- DGCP
	len=httpdFindArg(connData->post->buff, "DHCP", buff, sizeof(buff));
	if (len!=0)
	{
		INFO("\r\n");
		if (os_strcmp(buff, "true")==0)
		{
			sysCfg.dhcp_sta_enabled = 1;
			INFO("DHCP enabled\r\n");
		}
		else
		{
			sysCfg.dhcp_sta_enabled = 0;
			INFO("DHCP disabled\r\n");
		}

		INFO("DHCP:%s",buff);
		INFO("\r\n");
	}


	SysCFG_Save();
	httpdRedirect(connData, "/set_ip/set_ip.tpl");
	return HTTPD_CGI_DONE;
}


int ICACHE_FLASH_ATTR cgi_set_ip_2(HttpdConnData *connData)
{
	int len;
	char buff[1024];
	struct ip_addr ip_addr_fn;
	if (connData->conn==NULL) {
		//Connection aborted. Clean up.
		return HTTPD_CGI_DONE;

	}

	WIFI_Connect(sysCfg.sta_ssid, sysCfg.sta_pwd, NULL);

	INFO("\r\nApply IP\r\n");

	httpdRedirect(connData, "/set_ip/set_ip.tpl");
	return HTTPD_CGI_DONE;
}
