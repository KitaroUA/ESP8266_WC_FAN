/*
 * debug.h
 *
 *  Created on: Dec 4, 2014
 *      Author: Minh
 */

#ifndef USER_DEBUG_H_
#define USER_DEBUG_H_



#ifndef INFO

	#ifdef DEBUG_INFO
		#define INFO os_printf
	#else
		#define INFO(fmt, ...) do {	\
									} while(0)

	#endif
#endif

#endif /* USER_DEBUG_H_ */
