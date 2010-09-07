/*
 * logger.h
 *
 *  Created on: 07/set/2010
 *      Author: filippodeluca
 */

#ifndef LOGGER_H_
#define LOGGER_H_

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

typedef struct _logger_t logger_t;

logger_t* logger_init();

void logger_destroy(logger_t*);

void logger_error(const logger_t*, const char* file, const int line, const char* format, ...);

void logger_warn(const logger_t*, const char* file, const int line, const char* format, ...);

void logger_info(const logger_t*, const char* file, const int line, const char* format, ...);

void logger_debug(const logger_t*, const char* file, const int line, const char* format, ...);

void logger_trace(const logger_t*, const char* file, const int line, const char* format, ...);

#endif /* LOGGER_H_ */
