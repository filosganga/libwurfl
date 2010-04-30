/*
 * logger.h
 *
 *  Created on: Mar 18, 2010
 *      Author: filosganga
 */

#ifndef LOGGER_H_
#define LOGGER_H_

typedef struct _logger_t logger_t;

typedef void (*logger_log_f)(logger_t* log, const char* format, ...);

struct _logger_t {
	void* context;
	logger_log_f error;
	logger_log_f warn;
	logger_log_f info;
	logger_log_f debug;
	logger_log_f trace;
};


#endif /* LOGGER_H_ */
