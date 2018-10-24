/*
 * mqtt_settings.h
 *
 *  Created on: 30 жовт. 2017 р.
 *      Author: Kitaro
 */

#ifndef USER_PAGES_MQTT_SETTINGS_H_
#define USER_PAGES_MQTT_SETTINGS_H_

#include "cgi.h"

int tpl_set_mqtt(HttpdConnData *connData, char *token, void **arg);
int cgi_set_mqtt(HttpdConnData *connData);

#endif /* USER_PAGES_MQTT_SETTINGS_H_ */


