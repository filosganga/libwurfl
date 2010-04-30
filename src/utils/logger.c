/*
 * logger.c
 *
 *  Created on: Mar 18, 2010
 *      Author: filosganga
 */

#include "logger.h"

#include <stdlib.h>
#include <stdio.h>

typedef struct {
	int fd;
} console_context_t;


void logger_stream_log(logger_t* logger, const char* format, ...) {

	int fd = ((console_context_t*)logger->context)->fd;

	// TODO ...
}


logger_t* logger_create_console_logger(int fd) {

	console_context_t* context = NULL;
	context->fd = fd;

	logger_t* logger = NULL;
	logger->context = context;

	logger->error = &logger_stream_log;
	logger->warn = &logger_stream_log;
	logger->info = &logger_stream_log;
	logger->debug = &logger_stream_log;
	logger->trace = &logger_stream_log;

	return logger;
}
