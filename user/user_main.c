/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * Jeroen Domburg <jeroen@spritesmods.com> wrote this file. As long as you retain 
 * this notice you can do whatever you want with this stuff. If we meet some day, 
 * and you think this stuff is worth it, you can buy me a beer in return. 
 * ----------------------------------------------------------------------------
 */

/*
This is example code for the esphttpd library. It's a small-ish demo showing off 
the server, including WiFi connection management capabilities, some IO and
some pictures of cats.
*/



#include "user_config.h"

#include <esp8266.h>
#include "httpd.h"
#include "io.h"
#include "httpdespfs.h"
#include "cgi.h"
#include "cgiwifi.h"
#include "cgiflash.h"
#include "stdio.h"
#include "auth.h"
#include "espfs.h"
#include "captdns.h"
#include "webpages-espfs.h"
#include "cgiwebsocket.h"





#define HI(X) (X>>8)
#define LO(X) (X & 0xFF)

#define DSDEBUG 0


//The example can print out the heap use every 3 seconds. You can use this to catch memory leaks.
//#define SHOW_HEAP_USE

//Function that tells the authentication system what users/passwords live on the system.
//This is disabled in the default build; if you want to try it, enable the authBasic line in
//the builtInUrls below.
int ICACHE_FLASH_ATTR myPassFn(HttpdConnData *connData, int no, char *user, int userLen, char *pass, int passLen) {
	if (no==0) {
		os_strcpy(user, "admin");
		os_strcpy(pass, "s3cr3t");
		return 1;
//Add more users this way. Check against incrementing no for each user added.
//	} else if (no==1) {
//		os_strcpy(user, "user1");
//		os_strcpy(pass, "something");
//		return 1;
	}
	return 0;
}

static ETSTimer websockTimer;

//Broadcast the uptime in seconds every second over connected websockets
static void ICACHE_FLASH_ATTR websockTimerCb(void *arg) {
	static int ctr=0;
	char buff[128];
	ctr++;
	os_sprintf(buff, "Up for %d minutes %d seconds!\n", ctr/60, ctr%60);
	cgiWebsockBroadcast("/websocket/ws.cgi", buff, os_strlen(buff), WEBSOCK_FLAG_NONE);
}

//On reception of a message, send "You sent: " plus whatever the other side sent
void myWebsocketRecv(Websock *ws, char *data, int len, int flags) {
	int i;
	char buff[128];
	os_sprintf(buff, "You sent: ");
	for (i=0; i<len; i++) buff[i+10]=data[i];
	buff[i+10]=0;
	cgiWebsocketSend(ws, buff, os_strlen(buff), WEBSOCK_FLAG_NONE);
}

//Websocket connected. Install reception handler and send welcome message.
void ICACHE_FLASH_ATTR myWebsocketConnect(Websock *ws) {
	ws->recvCb=myWebsocketRecv;
	cgiWebsocketSend(ws, "Hi, Websocket!", 14, WEBSOCK_FLAG_NONE);
}

//On reception of a message, echo it back verbatim
void ICACHE_FLASH_ATTR myEchoWebsocketRecv(Websock *ws, char *data, int len, int flags) {
	os_printf("EchoWs: echo, len=%d\n", len);
	cgiWebsocketSend(ws, data, len, flags);
}

//Echo websocket connected. Install reception handler.
void ICACHE_FLASH_ATTR myEchoWebsocketConnect(Websock *ws) {
	os_printf("EchoWs: connect\n");
	ws->recvCb=myEchoWebsocketRecv;
}


#ifdef ESPFS_POS
CgiUploadFlashDef uploadParams={
	.type=CGIFLASH_TYPE_ESPFS,
	.fw1Pos=ESPFS_POS,
	.fw2Pos=0,
	.fwSize=ESPFS_SIZE,
};
#define INCLUDE_FLASH_FNS
#endif
#ifdef OTA_FLASH_SIZE_K
CgiUploadFlashDef uploadParams={
	.type=CGIFLASH_TYPE_FW,
	.fw1Pos=0x1000,
	.fw2Pos=((OTA_FLASH_SIZE_K*1024)/2)+0x1000,
	.fwSize=((OTA_FLASH_SIZE_K*1024)/2)-0x1000,
	.tagName=OTA_TAGNAME
};
#define INCLUDE_FLASH_FNS
#endif

/*
This is the main url->function dispatching data struct.
In short, it's a struct with various URLs plus their handlers. The handlers can
be 'standard' CGI functions you wrote, or 'special' CGIs requiring an argument.
They can also be auth-functions. An asterisk will match any url starting with
everything before the asterisks; "*" matches everything. The list will be
handled top-down, so make sure to put more specific rules above the more
general ones. Authorization things (like authBasic) act as a 'barrier' and
should be placed above the URLs they protect.
*/
HttpdBuiltInUrl builtInUrls[]={
	{"/", cgiRedirect, "/index.tpl"},
	{"/index.tpl", cgiEspFsTemplate, tpl_index},
	{"/index.cgi", cgi_index, NULL},
	{"/index_get_data.cgi", cgi_index_get_data, NULL},

/*
 * 	Options page
 */

	{"/options/options.tpl", cgiEspFsTemplate, tpl_options},
	{"/options/options.cgi", cgi_options, NULL},





/*
 * set_ntp page
 */
//	{"/set_ntp", cgiRedirect, "/set_ntp/set_ntp.tpl"},
//	{"/set_ntp/", cgiRedirect, "/set_ntp/set_ntp.tpl"},
	{"/set_ntp/set_ntp.tpl", cgiEspFsTemplate, tpl_set_ntp},
	{"/set_ntp/set_ntp.cgi", cgi_set_ntp, NULL},
	{"/set_ntp/apply_ntp.cgi", cgi_apply_ntp, NULL},
	{"/set_ntp/PC_Time.cgi", cgi_PC_Time, NULL},


/*
 * set_off_time page
 */
//	{"/set_off_time", cgiRedirect, "/set_off_time/set_off_time.tpl"},
//	{"/set_off_time/", cgiRedirect, "/set_off_time/set_off_time.tpl"},
	{"/set_off_time/set_off_time.tpl", cgiEspFsTemplate, tpl_set_off_time},
	{"/set_off_time/set_off_time.cgi", cgi_set_off_time, NULL},


/*
 * set_on_time page
 */
//	{"/set_on_time", cgiRedirect, "/set_on_time/set_on_time.tpl"},
//	{"/set_on_time/", cgiRedirect, "/set_on_time/set_on_time.tpl"},
	{"/set_on_time/set_on_time.tpl", cgiEspFsTemplate, tpl_set_on_time},
	{"/set_on_time/set_on_time.cgi", cgi_set_on_time, NULL},


/*
 * set_temp_off page
 */
//	{"/set_temp_off", cgiRedirect, "/set_temp_off/set_temp_off.tpl"},
//	{"/set_temp_off/", cgiRedirect, "/set_temp_off/set_temp_off.tpl"},
	{"/set_temp_off/set_temp_off.tpl", cgiEspFsTemplate, tpl_set_temp_off},
	{"/set_temp_off/set_temp_off.cgi", cgi_set_temp_off, NULL},


/*
 * set_temp_on page
 */
	//	{"/set_temp_off", cgiRedirect, "/set_temp_off/set_temp_off.tpl"},
	//	{"/set_temp_off/", cgiRedirect, "/set_temp_off/set_temp_off.tpl"},
		{"/set_temp_on/set_temp_on.tpl", cgiEspFsTemplate, tpl_set_temp_on},
		{"/set_temp_on/set_temp_on.cgi", cgi_set_temp_on, NULL},



/*
 * Temperature selector page
 */
	//	{"/set_temp_off", cgiRedirect, "/set_temp_off/set_temp_off.tpl"},
	//	{"/set_temp_off/", cgiRedirect, "/set_temp_off/set_temp_off.tpl"},
		{"/temperature_selector/temperature_selector.tpl", cgiEspFsTemplate, tpl_temperature_selector},
		{"/temperature_selector/Temperature_options_1.cgi", cgi_temperature_selector_1, NULL},
//		{"/temperature_selector/temperature_selector_1.js", cgiEspFsTemplate, js_temperature_selector_1},
		{"/temperature_selector/temperature_selector.var", cgiEspFsTemplate, var_temperature_selector},





/*
 * Time Range selector page
 */
	//	{"/set_temp_off", cgiRedirect, "/set_temp_off/set_temp_off.tpl"},
	//	{"/set_temp_off/", cgiRedirect, "/set_temp_off/set_temp_off.tpl"},
		{"/working_time_selector/working_time_selector.tpl", cgiEspFsTemplate, tpl_working_time_selector},
		{"/working_time_selector/working_time_selector_1.cgi", cgi_working_time_selector_1, NULL},
//		{"/working_time_selector/working_time_selector_1.js", cgiEspFsTemplate, js_working_time_selector_1},


/*
 * set_mqtt page
 */
		//	{"/set_ntp", cgiRedirect, "/set_ntp/set_ntp.tpl"},
		//	{"/set_ntp/", cgiRedirect, "/set_ntp/set_ntp.tpl"},
			{"/set_mqtt/set_mqtt.tpl", cgiEspFsTemplate, tpl_set_mqtt},
			{"/set_mqtt/set_mqtt.cgi", cgi_set_mqtt, NULL},



/*
 * set_ip page
 */
		//	{"/set_ntp", cgiRedirect, "/set_ntp/set_ntp.tpl"},
		//	{"/set_ntp/", cgiRedirect, "/set_ntp/set_ntp.tpl"},
			{"/set_ip/set_ip.tpl", cgiEspFsTemplate, tpl_set_ip},
			{"/set_ip/set_ip_1.cgi", cgi_set_ip_1, NULL},
			{"/set_ip/set_ip_2.cgi", cgi_set_ip_2, NULL},



	{"/flash/download", cgiReadFlash, NULL},
#ifdef INCLUDE_FLASH_FNS
	{"/flash/next", cgiGetFirmwareNext, &uploadParams},
	{"/flash/upload", cgiUploadFirmware, &uploadParams},
#endif
	{"/flash/reboot", cgiRebootFirmware, NULL},



//Routines to make the /wifi URL and everything beneath it work.
//Enable the line below to protect the WiFi configuration with an username/password combo.
//	{"/wifi/*", authBasic, myPassFn},
	{"/wifi", cgiRedirect, "/wifi/wifi.tpl"},
	{"/wifi/", cgiRedirect, "/wifi/wifi.tpl"},
	{"/wifi/wifiscan.cgi", cgiWiFiScan, NULL},
	{"/wifi/wifi.tpl", cgiEspFsTemplate, tplWlan},
	{"/wifi/connect.cgi", cgiWiFiConnect, NULL},
	{"/wifi/connstatus.cgi", cgiWiFiConnStatus, NULL},
	{"/wifi/setmode.cgi", cgiWiFiSetMode, NULL},

	{"*", cgiEspFsHook, NULL}, //Catch-all cgi function for the filesystem
	{NULL, NULL, NULL}
};


#ifdef SHOW_HEAP_USE
static ETSTimer prHeapTimer;

static void ICACHE_FLASH_ATTR prHeapTimerCb(void *arg) {
	os_printf("Heap: %ld\n", (unsigned long)system_get_free_heap_size());
}
#endif






// ==============================================================
// MQTT Part
//MQTT_Client mqttClient;





void ICACHE_FLASH_ATTR wifiConnectCb(uint8_t status)
{
	if(status == STATION_GOT_IP){
		MQTT_Connect(&mqttClient);
	} else {
		MQTT_Disconnect(&mqttClient);
	}
}
void ICACHE_FLASH_ATTR mqttConnectedCb(uint32_t *args)
{
	MQTT_Client* client = (MQTT_Client*)args;
	INFO("MQTT: Connected\r\n");
	MQTT_Subscribe(client, "/esp1/LED", 0);
	MQTT_Subscribe(client, "/esp1/NTP", 1);

	MQTT_Publish(client, "/esp1/Temp", "0", 1, 0, 0);
	MQTT_Publish(client, "/esp1/Hum",  "0", 1, 1, 0);
	MQTT_Publish(client, "/esp1/LED",  "0", 1, 1, 0);
	MQTT_Publish(client, "/esp1/NTP",  "0", 1, 1, 0);

}

void ICACHE_FLASH_ATTR mqttDisconnectedCb(uint32_t *args)
{
//	MQTT_Client* client = (MQTT_Client*)args;
	INFO("MQTT: Disconnected\r\n");
}

void ICACHE_FLASH_ATTR mqttPublishedCb(uint32_t *args)
{
//	MQTT_Client* client = (MQTT_Client*)args;
	INFO("MQTT: Published\r\n");
}

void ICACHE_FLASH_ATTR mqttDataCb(uint32_t *args, const char* topic, uint32_t topic_len, const char *data, uint32_t data_len)
{
	char *topicBuf = (char*)os_zalloc(topic_len+1),
			*dataBuf = (char*)os_zalloc(data_len+1);

//	MQTT_Client* client = (MQTT_Client*)args;

	os_memcpy(topicBuf, topic, topic_len);
	topicBuf[topic_len] = 0;

	os_memcpy(dataBuf, data, data_len);
	dataBuf[data_len] = 0;

	INFO("Receive topic: %s, data: %s \r\n", topicBuf, dataBuf);

	char if_op[80];
	os_sprintf(if_op, "Get topic:%s with data:%s \r\n", topicBuf, dataBuf);
	INFO(topicBuf);
	if (strcmp(topicBuf, "/esp1/LED") == 0)
	{

	}

	if (strcmp(topicBuf, "/esp1/NTP") == 0)
	{

		uint8 triger = atoi (dataBuf);

		if (triger == 1)
		{
			ntp_get_time();
		}

	}


	os_free(topicBuf);
	os_free(dataBuf);



}

// END MQTT Part
// ==============================================================



static void ICACHE_FLASH_ATTR networkServerFoundCb(const char *name, ip_addr_t *ip, void *arg) {
  static esp_tcp tcp;
  os_printf("\r\n\r\n\r\n");
  struct espconn *conn=(struct espconn *)arg;
  if (ip==NULL) {
    os_printf("Nslookup failed :/ Trying again...\n");
    os_printf("lfai");
  }
  else{
  os_printf("lokk");
  char page_buffer[20];
  os_sprintf(page_buffer,"DST: %d.%d.%d.%d",
  *((uint8 *)&ip->addr), *((uint8 *)&ip->addr + 1),
  *((uint8 *)&ip->addr + 2), *((uint8 *)&ip->addr + 3));
  os_printf(page_buffer);
  }
}






/******************************************************************************
 * FunctionName : user_rf_cal_sector_set
 * Description  : SDK just reversed 4 sectors, used for rf init data and paramters.
 *                We add this function to force users to set rf cal sector, since
 *                we don't know which sector is free in user's application.
 *                sector map for last several sectors : ABBBCDDD
 *                A : rf cal
 *                B : at parameters
 *                C : rf init data
 *                D : sdk parameters
 * Parameters   : none
 * Returns      : rf cal sector
*******************************************************************************/
uint32 ICACHE_FLASH_ATTR user_rf_cal_sector_set(void)
{
    enum flash_size_map size_map = system_get_flash_size_map();
    uint32 rf_cal_sec = 0;

    switch (size_map) {
        case FLASH_SIZE_4M_MAP_256_256:
            rf_cal_sec = 128 - 8;
            break;

        case FLASH_SIZE_8M_MAP_512_512:
            rf_cal_sec = 256 - 5;
            break;

        case FLASH_SIZE_16M_MAP_512_512:
        case FLASH_SIZE_16M_MAP_1024_1024:
            rf_cal_sec = 512 - 5;
            break;

        case FLASH_SIZE_32M_MAP_512_512:
        case FLASH_SIZE_32M_MAP_1024_1024:
            rf_cal_sec = 1024 - 5;
            break;

        default:
            rf_cal_sec = 0;
            break;
    }

    return rf_cal_sec;
}



void ICACHE_FLASH_ATTR user_rf_pre_init(void)
{
}

















os_timer_t ext_int_timer;

extern int ets_uart_printf(const char *fmt, ...);
int (*console_printf)(const char *fmt, ...) = ets_uart_printf;

extern uint8_t pin_num[GPIO_PIN_NUM];

// GPIO_PIN_INTR_NEGEDGE - down
// GPIO_PIN_INTR_POSEDGE - up
// GPIO_PIN_INTR_ANYEDGE - both
// GPIO_PIN_INTR_LOLEVEL - low level
// GPIO_PIN_INTR_HILEVEL - high level
// GPIO_PIN_INTR_DISABLE - disable interrupt
const char *gpio_type_desc[] =
{
	    "GPIO_PIN_INTR_DISABLE (DISABLE INTERRUPT)",
	    "GPIO_PIN_INTR_POSEDGE (UP)",
	    "GPIO_PIN_INTR_NEGEDGE (DOWN)",
	    "GPIO_PIN_INTR_ANYEDGE (BOTH)",
	    "GPIO_PIN_INTR_LOLEVEL (LOW LEVEL)",
	    "GPIO_PIN_INTR_HILEVEL (HIGH LEVEL)"
};

void intr_callback(unsigned pin, unsigned level);
void ICACHE_FLASH_ATTR intr_reattach(void)
{
	INFO("\r\nINTERRUPT: GPIO%d reattached \r\n", WIFI_Button_Pin);
	gpio_intr_init(WIFI_Button_Pin,GPIO_PIN_INTR_POSEDGE);
	gpio_intr_attach(intr_callback);

}


void ICACHE_FLASH_ATTR intr_callback_t(void)
{
	INFO("\r\nINTERRUPT: GPIO%d = %d\r\n", WIFI_Button_Pin, GPIO_INPUT_GET(WIFI_Button_Pin));
	gpio_intr_deattach(WIFI_Button_Pin);
	SetTimerTask (ext_int_timer, intr_reattach, 50, 0);


}


void ICACHE_FLASH_ATTR intr_callback(unsigned pin, unsigned level)
{
	SetTimerTask (ext_int_timer, intr_callback_t, 1, 0);
}
















//Main routine. Initialize stdout, the I/O, filesystem and the webserver and we're done.
void ICACHE_FLASH_ATTR user_init(void)
{

	temporary_light_off_timer = 0;
	temporary_light_on_timer = 0;
	uptime=0;


//	stdoutInit();
	uart_init(BIT_RATE_115200, BIT_RATE_115200);
	INFO("\r\nStarting... \r\n");

//	ioInit();



	AddCFG_Load();
	mFlag.try++;
	AddCFG_Save();




/*
 * Working state. WiFi config 0 - normal, 1 - setup
 * |||||||||||||||||||||||||
 * ↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓
 */
uint8_t work_mode = 0;
	PIN_FUNC_SELECT(WIFI_Button_Mux, WIFI_Button_Func);

	if (GPIO_INPUT_GET(WIFI_Button_Pin))
	{
		SysCFG_Load(0);
//		INFO ("\r\n Try Normal = %d \r\n", mFlag.try);

		INFO("\r\nNormal mode\r\n");
		WIFI_Connect(sysCfg.sta_ssid, sysCfg.sta_pwd, wifiConnectCb);

		builtInUrls[0].cgiArg = "/index.tpl";
	}
	else
	{
		INFO("\r\nSetup mode\r\n");
		struct ip_info ipinfo;
			wifi_softap_dhcps_stop();
			ipinfo.ip=sysCfg.apipinfo.ip;
			ipinfo.gw=sysCfg.apipinfo.gw;
			ipinfo.netmask=sysCfg.apipinfo.netmask;
			wifi_set_ip_info(SOFTAP_IF, &ipinfo);
			wifi_softap_dhcps_start();
			wifi_set_opmode(STATIONAP_MODE);

		builtInUrls[0].cgiArg = "/index_s.tpl";
		work_mode=1;



		mFlag.try++;

//		INFO ("\r\n Try Setup = %d \r\n", mFlag.try);

		if(mFlag.try == 2)
		/*
		 * Init of flash variables
		 * |||||||||||||||||||||||||
		 * ↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓
		 */

			Default_CFG();
			SysCFG_Load(1);

		/* ↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑
		 * |||||||||||||||||||||||||
		 * Init of flash variables
		 */
		mFlag.try=0;
		AddCFG_Save();


	}


/* ↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑
 * |||||||||||||||||||||||||
 * Working state
 */


captdnsInit();

	// 0x40200000 is the base address for spi flash memory mapping, ESPFS_POS is the position
	// where image is written in flash that is defined in Makefile.
#ifdef ESPFS_POS
	espFsInit((void*)(0x40200000 + ESPFS_POS));
#else
//	espFsInit((void*)(webpages_espfs_start));
#endif
	httpdInit(builtInUrls, 80);
#ifdef SHOW_HEAP_USE
	os_timer_disarm(&prHeapTimer);
	os_timer_setfn(&prHeapTimer, prHeapTimerCb, NULL);
	os_timer_arm(&prHeapTimer, 3000, 1);
#endif
	os_timer_disarm(&websockTimer);
	os_timer_setfn(&websockTimer, websockTimerCb, NULL);
	os_timer_arm(&websockTimer, 1000, 1);
	INFO("\r\nHTTPd initialization finished ..\r\n");










	// Apply some Time zone vars
	_daylight = mFlag.dst_flag;                 // Non-zero if daylight savings time is used
	_dstbias = 3600;                  			// Offset for Daylight Saving Time
	_timezone = 0 - (mFlag.timezone*3600);      // Difference in seconds between GMT and local time








/*
 * Working state. Services config 0 - normal, 1 - setup
 * |||||||||||||||||||||||||
 * ↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓
 */

if(work_mode == 0)
{

	MQTT_InitConnection(&mqttClient, sysCfg.mqtt_host, sysCfg.mqtt_port, sysCfg.security);
	//MQTT_InitConnection(&mqttClient, "192.168.11.122", 1880, 0);

	MQTT_InitClient(&mqttClient, sysCfg.device_id, sysCfg.mqtt_user, sysCfg.mqtt_pass, sysCfg.mqtt_keepalive, 1);
	//MQTT_InitClient(&mqttClient, "client_id", "user", "pass", 120, 1);
	char cj[6] = "/lwt";
	char ci[9] = "offline";
	MQTT_InitLWT(&mqttClient, (uint8_t*)cj, (uint8_t*)ci, 0, 0);
	MQTT_OnConnected(&mqttClient, mqttConnectedCb);
	MQTT_OnDisconnected(&mqttClient, mqttDisconnectedCb);
	MQTT_OnPublished(&mqttClient, mqttPublishedCb);
	MQTT_OnData(&mqttClient, mqttDataCb);
	INFO("\r\nMQTT initialization finished ...\r\n");



	i2c_init();

	DS3231_Init();
	SetTimerTask (circular_timer, init_circular_timer_proc, 5000, 0);
//	i2c_SHT3X_Test();
	i2c_SSD1306_Init();

	SetTimerTask(ds18b20_timer, ds18b20_search, 1500, 0);
//	SetTimerTask(PCA9685_timer, i2c_PCA9685_Init, 2, 0);

	i2c_PCF8574_enabled = 1;

//	PIN_FUNC_SELECT(RELAY_MUX, RELAY_FUNC);

	SetTimerTask(BME280_timer_read, BME280_Start_Init, 5000, 0);

//	======================================
//	External interrupts
	GPIO_INT_TYPE gpio_type;
	uint8_t gpio_pin;


	INFO ("\r\n External Int.\r\n");
	gpio_pin = 5;
	gpio_type = GPIO_PIN_INTR_POSEDGE;
	if (set_gpio_mode(gpio_pin,  GPIO_INT, GPIO_PULLUP)) {
		INFO("GPIO%d set interrupt mode\r\n", pin_num[gpio_pin]);
		if (gpio_intr_init(gpio_pin, gpio_type)) {
			INFO("GPIO%d enable %s mode\r\n", pin_num[gpio_pin], gpio_type_desc[gpio_type]);
			gpio_intr_attach(intr_callback);
		} else {
			INFO("Error: GPIO%d not enable %s mode\r\n", pin_num[gpio_pin], gpio_type_desc[gpio_type]);
		}
	} else {
		INFO("Error: GPIO%d not set interrupt mode\r\n", pin_num[gpio_pin]);
	}
//	External interrupts
//	======================================


}
else
{

}



	INFO("\r\nSystem started ...\r\n");





}










