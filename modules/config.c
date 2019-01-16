
/* config.c
*
* Copyright (c) 2014-2015, Tuan PM <tuanpm at live dot com>
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*
* * Redistributions of source code must retain the above copyright notice,
* this list of conditions and the following disclaimer.
* * Redistributions in binary form must reproduce the above copyright
* notice, this list of conditions and the following disclaimer in the
* documentation and/or other materials provided with the distribution.
* * Neither the name of Redis nor the names of its contributors may be used
* to endorse or promote products derived from this software without
* specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
* ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
* INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
* CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
* ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
* POSSIBILITY OF SUCH DAMAGE.
*/
#include "ets_sys.h"
#include "os_type.h"
#include "mem.h"
#include "osapi.h"
#include "user_interface.h"

#include "mqtt.h"
#include "config.h"

#include "../libesphttpd/include/debug.h"
#include "user_config.h"

SYSCFG sysCfg;
//SAVE_FLAG saveFlag;

//MY_FLASH_STR sysCfg;

void ICACHE_FLASH_ATTR
SysCFG_Save()
{
		spi_flash_erase_sector(CFG_LOCATION);
		spi_flash_write((CFG_LOCATION ) * SPI_FLASH_SEC_SIZE,
						(uint32 *)&sysCfg, sizeof(SYSCFG));
}

void ICACHE_FLASH_ATTR
SysCFG_Load()
{

	INFO("\r\nload ...\r\n");
		spi_flash_read((CFG_LOCATION + 0) * SPI_FLASH_SEC_SIZE,
					   (uint32 *)&sysCfg, sizeof(SYSCFG));

/*	if((RLoad == 1)){
		os_memset(&sysCfg, 0x00, sizeof sysCfg);


		sysCfg.cfg_holder = CFG_HOLDER;

		os_sprintf(sysCfg.sta_ssid, "%s", STA_SSID);
		os_sprintf(sysCfg.sta_pwd, "%s", STA_PASS);
		sysCfg.sta_type = STA_TYPE;

		os_sprintf(sysCfg.device_id, MQTT_CLIENT_ID, system_get_chip_id());
//		os_sprintf(sysCfg.mqtt_host, "%s", MQTT_HOST);
		os_sprintf(sysCfg.mqtt_host, "%s", sysCfg.mqtt_server);
		sysCfg.mqtt_port = MQTT_PORT;
		os_sprintf(sysCfg.mqtt_user, "%s", MQTT_USER);
		os_sprintf(sysCfg.mqtt_pass, "%s", MQTT_PASS);

		sysCfg.security = DEFAULT_SECURITY;	// default non ssl

		sysCfg.mqtt_keepalive = MQTT_KEEPALIVE;

		sysCfg.dhcp_sta_enabled = 0;

//		IP4_ADDR(&sysCfg.staipinfo.ip, 192, 168, 104, 201);
//		IP4_ADDR(&sysCfg.staipinfo.gw, 192, 168, 104, 1);
//		IP4_ADDR(&sysCfg.staipinfo.netmask, 255, 255, 255, 0);
		IP4_ADDR(&sysCfg.staipinfo.ip, sta_ip_a, sta_ip_b, sta_ip_c, sta_ip_d);
		IP4_ADDR(&sysCfg.staipinfo.gw, sta_gw_a, sta_gw_b, sta_gw_c, sta_gw_d);
		IP4_ADDR(&sysCfg.staipinfo.netmask, sta_nm_a, sta_nm_b, sta_nm_c, sta_nm_d);


//		IP4_ADDR(&sysCfg.apipinfo.ip, 192, 168, 4, 1);
//		IP4_ADDR(&sysCfg.apipinfo.gw, 192, 168, 4, 1);
//		IP4_ADDR(&sysCfg.apipinfo.netmask, 255, 255, 255, 0);
		IP4_ADDR(&sysCfg.apipinfo.ip, sap_ip_a, sap_ip_b, sap_ip_c, sap_ip_d);
		IP4_ADDR(&sysCfg.apipinfo.gw, sap_gw_a, sap_gw_b, sap_gw_c, sap_gw_d);
		IP4_ADDR(&sysCfg.apipinfo.netmask, sap_nm_a, sap_nm_b, sap_nm_c, sap_nm_d);

		INFO(" default configuration\r\n");

		SysCFG_Save();
	}
*/
}




/*
 * FLASH part
 */



void ICACHE_FLASH_ATTR Default_CFG(void)
{
	INFO("\r\n============= Load !DEFAULT! VAR ===================\r\n");


	os_memset(&sysCfg, 0x00, sizeof sysCfg);



	os_sprintf(sysCfg.sta_ssid, "%s", STA_SSID);
	os_sprintf(sysCfg.sta_pwd, "%s", STA_PASS);
	sysCfg.sta_type = STA_TYPE;

	os_sprintf(sysCfg.device_id, MQTT_CLIENT_ID, system_get_chip_id());

	 os_sprintf(sysCfg.mqtt_host, "%s", default_mqtt_host);

	sysCfg.mqtt_port = MQTT_PORT;
	os_sprintf(sysCfg.mqtt_user, "%s", MQTT_USER);
	os_sprintf(sysCfg.mqtt_pass, "%s", MQTT_PASS);

	sysCfg.security = DEFAULT_SECURITY;	// default non ssl

	sysCfg.mqtt_keepalive = MQTT_KEEPALIVE;

	sysCfg.dhcp_sta_enabled = 0;

	IP4_ADDR(&sysCfg.staipinfo.ip, sta_ip_a, sta_ip_b, sta_ip_c, sta_ip_d);
	IP4_ADDR(&sysCfg.staipinfo.gw, sta_gw_a, sta_gw_b, sta_gw_c, sta_gw_d);
	IP4_ADDR(&sysCfg.staipinfo.netmask, sta_nm_a, sta_nm_b, sta_nm_c, sta_nm_d);


	IP4_ADDR(&sysCfg.apipinfo.ip, sap_ip_a, sap_ip_b, sap_ip_c, sap_ip_d);
	IP4_ADDR(&sysCfg.apipinfo.gw, sap_gw_a, sap_gw_b, sap_gw_c, sap_gw_d);
	IP4_ADDR(&sysCfg.apipinfo.netmask, sap_nm_a, sap_nm_b, sap_nm_c, sap_nm_d);

	INFO(" default configuration\r\n");








	os_sprintf(sysCfg.tempOn_time, "%s", "45");
	os_sprintf(sysCfg.hostname, "ESP_IoT_02001");
	os_sprintf(sysCfg.ntp, "0.ua.pool.ntp.org");
	sysCfg.timezone = 2;
	sysCfg.ntp_flag=1;
	sysCfg.dst_flag=1;
	sysCfg.dst_active=1;



	 sysCfg.mqtt_task_enabled = 1;
	 sysCfg.try=0;

	 SysCFG_Save();
}


/*
 * End FLASH part
 */

