/*
 * logger.c
 *
 *  Created on: 07/set/2010
 *      Author: filippodeluca
 */

#include "logger.h"

typedef enum {ERROR, WARN, INFO, DEBUG, TRACE} level_e;

struct _logger_t {
	FILE *stream;
};

logger_t* logger_init() {
	logger_t* logger = malloc(sizeof(logger_t));
	if(!logger) {
		exit(-1);
	}

	logger->stream = stderr;

	return logger;
}

void logger_destroy(logger_t* logger) {

	free(logger);
}

static void logger_print(const logger_t* logger, const char* file, const int line, const level_e level, const char* format, va_list args) {

	const char* leveldesc;
	if(level==ERROR) {
		leveldesc = "ERROR";
	}
	else if(level==WARN) {
		leveldesc = "WARN";
	}
	else if(level==INFO) {
		leveldesc = "INFO";
	}
	else if(level==DEBUG) {
		leveldesc = "DEBUG";
	}
	else {
		leveldesc = "TRACE";
	}


	fprintf(logger->stream, "[%s] %s:%d\t",leveldesc, file, line);
	vfprintf(logger->stream, format, args);
	fprintf(logger->stream, "\n");
	fflush(logger->stream);
}

void logger_error(const logger_t* logger, const char* file, const int line, const char* format, ...) {
	va_list args;

	fflush (stdout);

	va_start (args, format);
    logger_print(logger, file, line, ERROR, format, args);
    va_end (args);
}

void logger_warn(const logger_t* logger, const char* file, const int line, const char* format, ...) {
	va_list args;

	fflush (stdout);

	va_start (args, format);
    logger_print(logger, file, line, WARN, format, args);
    va_end (args);

}

void logger_info(const logger_t* logger, const char* file, const int line, const char* format, ...) {
	va_list args;

	fflush (stdout);

	va_start (args, format);
    logger_print(logger, file, line, INFO, format, args);
    va_end (args);

}

void logger_debug(const logger_t* logger, const char* file, const int line, const char* format, ...) {
	va_list args;

	fflush (stdout);

	va_start (args, format);
    logger_print(logger, file, line, DEBUG, format, args);
    va_end (args);

}

void logger_trace(const logger_t* logger, const char* file, const int line, const char* format, ...) {
	va_list args;

	fflush (stdout);

	va_start (args, format);
    logger_print(logger, file, line, TRACE, format, args);
    va_end (args);

}
