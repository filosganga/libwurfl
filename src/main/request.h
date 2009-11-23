/*
 * request.h
 *
 *  Created on: 23-mar-2009
 *      Author: filosganga
 */

#ifndef REQUEST_H_
#define REQUEST_H_

typedef struct _request request;

char* request_get_header(void* request);

void request_destroy()

#endif /* REQUEST_H_ */
