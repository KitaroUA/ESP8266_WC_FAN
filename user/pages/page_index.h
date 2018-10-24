/*
 * page_index.h
 *
 *  Created on: 15 лют. 2018 р.
 *      Author: Kitaro
 */

#ifndef USER_PAGES_PAGE_INDEX_H_
#define USER_PAGES_PAGE_INDEX_H_

#include "cgi.h"


int tpl_index(HttpdConnData *connData, char *token, void **arg);
//int tpl_index_js(HttpdConnData *connData, char *token, void **arg);
int cgi_index(HttpdConnData *connData);
int cgi_index_get_data(HttpdConnData *connData);


#endif /* USER_PAGES_PAGE_INDEX_H_ */
