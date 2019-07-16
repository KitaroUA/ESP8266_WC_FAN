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

#define GPIO_PIN_NUM 13
#define ESP_GPIO_PIN_NUM 17

uint8_t pin2esp_pin[GPIO_PIN_NUM];
uint8_t pin2esp_pin[GPIO_PIN_NUM] = {16,  5,  4,  0,		// D1 mini pins (d0-d8) to GPIO numbers
								     2, 14, 12, 13,
								    15,  3,  1,  9,
								    10};

uint8_t esp_pin2pin[ESP_GPIO_PIN_NUM];
uint8_t esp_pin2pin[ESP_GPIO_PIN_NUM] = {  3,
										  10,   4,   9,  2,  1,
										 255, 255, 255, 11, 12,
										 255,   6,   7,  5,  8,
										   0};

// D0 - not used, d1-d8
uint8  DXpin_array[] = { 255, 5,  4,  0,  2, 14, 12, 13, 15};
uint8 DXgpio_array[] = { 255, FUNC_GPIO5, FUNC_GPIO4, FUNC_GPIO0, FUNC_GPIO2, FUNC_GPIO14, FUNC_GPIO12, FUNC_GPIO13, FUNC_GPIO15};
uint32 DXmux_array[] = { 255, PERIPHS_IO_MUX_GPIO5_U, PERIPHS_IO_MUX_GPIO4_U, PERIPHS_IO_MUX_GPIO0_U, PERIPHS_IO_MUX_GPIO2_U,
							  PERIPHS_IO_MUX_MTMS_U, PERIPHS_IO_MUX_MTDI_U, PERIPHS_IO_MUX_MTCK_U, PERIPHS_IO_MUX_MTDO_U};
const char *gpio_type_desc[] =
{
	    "GPIO_PIN_INTR_DISABLE (DISABLE INTERRUPT)",
	    "GPIO_PIN_INTR_POSEDGE (UP)",
	    "GPIO_PIN_INTR_NEGEDGE (DOWN)",
	    "GPIO_PIN_INTR_ANYEDGE (BOTH)",
	    "GPIO_PIN_INTR_LOLEVEL (LOW LEVEL)",
	    "GPIO_PIN_INTR_HILEVEL (HIGH LEVEL)"
};




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
#if OTA_FLASH_SIZE_K < 2049
        .fw2Pos=((OTA_FLASH_SIZE_K*1024)/2)+0x1000,
        .fwSize=((OTA_FLASH_SIZE_K*1024)/2)-0x1000,
#else
        .fw2Pos=0x101000,
        .fwSize=0xFF000,
#endif//	.tagName=OTA_TAGNAME
	.tagName="tag"
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
	MQTT_Subscribe(client, "/IoT_02001/Fan", 0);
	MQTT_Subscribe(client, "/IoT_02001/Setup", 0);

	MQTT_Publish(client, "/IoT_02001/Temp", "0", 1, 0, 0);
	MQTT_Publish(client, "/IoT_02001/Hum",  "0", 1, 1, 0);
	MQTT_Publish(client, "/IoT_02001/Mov",  "0", 1, 1, 0);
	MQTT_Publish(client, "/IoT_02001/Fan",  "0", 1, 1, 0);
	MQTT_Publish(client, "/IoT_02001/Setup",  "0", 1, 1, 0);


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















LOCAL void ICACHE_FLASH_ATTR dht22_cb(void)
{
	static uint8_t i;
	DHT_Sensor_Data data;
//	uint8_t pin;
//	os_timer_disarm(&dht22_timer);

	// One DHT22 sensor
//	pin = sensor.pin;
	if (DHTRead(&sensor, &data))
	{
	    char buff[20];
//	    INFO("GPIO%d\r\n", pin);
	    INFO("Temperature: %s *C\r\n", DHTFloat2String(buff, data.temperature));
	    INFO("Humidity: %s %%\r\n", DHTFloat2String(buff, data.humidity));
	} else {
//	    INFO("Failed to read temperature and humidity sensor on GPIO%d\n", pin);
	}

//	os_timer_arm(&dht22_timer, DELAY, 1);
}




os_timer_t intr_protect_timer;


void intr_rearm();

void ICACHE_FLASH_ATTR intr_callback(unsigned pin, unsigned level)
{
#ifdef int_debug
	INFO("INTERRUPT: GPIO%d = %d\r\n", pin2esp_pin[pin], level);
#endif


	if(pin == esp_pin2pin[PIR_PIN]) // Button
	{
#ifdef int_debug
		INFO("\r\n in: %d\r\n",GPIO_INPUT_GET(PIR_PIN));
#endif
//		GPIO_OUTPUT_SET(RELAY_PIN, GPIO_INPUT_GET(PIR_PIN));
		PIR_movement_set_falg();
		return;
	}



}






//Main routine. Initialize stdout, the I/O, filesystem and the webserver and we're done.
void ICACHE_FLASH_ATTR user_init(void)
{

	temporary_fan_on_timer = 0;
	temporary_fan_off_timer = 0;
	uptime=0;


//	stdoutInit();



	uart_init(BIT_RATE_115200, BIT_RATE_115200);
	INFO("\r\nStarting... \r\n");

//	ioInit();



	SysCFG_Load();
	sysCfg.try++;
	SysCFG_Save();




/*
 * Working state. WiFi config 0 - normal, 1 - setup
 * |||||||||||||||||||||||||
 * ↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓
 */
uint8_t work_mode = 0;
	PIN_FUNC_SELECT(WIFI_Button_Mux, WIFI_Button_Func);

	if (GPIO_INPUT_GET(WIFI_Button_Pin))
	{

		INFO("\r\nNormal mode\r\n");
		WIFI_Connect(sysCfg.sta_ssid, sysCfg.sta_pwd, wifiConnectCb);

		builtInUrls[0].cgiArg = "/index.tpl";
	}
	else
	{
		INFO("\r\nSetup mode\r\n");


		sysCfg.try++;

//		INFO ("\r\n Try Setup = %d \r\n", sysCfg.try);

		if(sysCfg.try == 2)
		/*
		 * Init of flash variables
		 * |||||||||||||||||||||||||
		 * ↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓
		 */

			Default_CFG();
//			SysCFG_Load(1);

		/* ↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑
		 * |||||||||||||||||||||||||
		 * Init of flash variables
		 */
		sysCfg.try=0;
		SysCFG_Save();


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
	_daylight = sysCfg.dst_flag;                 // Non-zero if daylight savings time is used
	_dstbias = 3600;                  			// Offset for Daylight Saving Time
	_timezone = 0 - (sysCfg.timezone*3600);      // Difference in seconds between GMT and local time








/*
 * Working state. Services config 0 - normal, 1 - setup
 * |||||||||||||||||||||||||
 * ↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓
 */

if(work_mode == 0)
{

	//	MQTT_InitConnection(&mqttClient, sysCfg.mqtt_host, sysCfg.mqtt_port, sysCfg.security);
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

//	DS3231_Init();
	DS1307_Init();
	char ct[21];
//	os_sprintf(ci, "%02x:%02x:%02x %02x.%02x.20%02x",  DS3231_Time[2], DS3231_Time[1], DS3231_Time[0], DS3231_Date[0], DS3231_Date[1], DS3231_Date[2]);
	os_sprintf(ct, "%02x:%02x:%02x",  DS1307_Time[2], DS1307_Time[1], DS1307_Time[0]);
	INFO("%s",ct);
	SetTimerTask (circular_timer, init_circular_timer_proc, 5000, 0);


		PIN_FUNC_SELECT(RELAY_MUX, RELAY_FUNC);

	// One DHT22 sensor
		sensor.pin =  esp_pin2pin[DHT_PIN];
		sensor.type = DHT22;
		INFO("DHT22 init on GPIO%d\r\n", sensor.pin);
		DHTInit(&sensor);



		//	======================================
		//	External interrupts
			GPIO_INT_TYPE gpio_type;
			uint8_t gpio_pin;


			INFO ("\r\n External Int.\r\n");

			gpio_pin=esp_pin2pin[PIR_PIN];
			gpio_type = GPIO_PIN_INTR_POSEDGE;
			if (set_gpio_mode(gpio_pin,  GPIO_INT, GPIO_PULLUP)) {
		#ifdef int_debug
				INFO("GPIO%d set interrupt mode\r\n", gpio_pin);
		#endif
				if (gpio_intr_init(gpio_pin, gpio_type)) {
		#ifdef int_debug
					INFO("GPIO%d enable %s mode\r\n", gpio_pin, gpio_type_desc[gpio_type]);
		#endif
					gpio_intr_attach(intr_callback);
				} else {
		#ifdef int_debug
					INFO("Error: GPIO%d not enable %s mode\r\n", gpio_pin, gpio_type_desc[gpio_type]);
		#endif
				}
			} else {
		#ifdef int_debug
				INFO("Error: GPIO%d not set interrupt mode\r\n", gpio_pin);
		#endif
			}

		//	External interrupts
		//	======================================

}
else
{

}



	INFO("\r\nSystem started ...\r\n");

	GPIO_OUTPUT_SET(RELAY_PIN, 1);





}










