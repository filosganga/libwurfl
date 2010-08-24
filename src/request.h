/*
 * request.h
 *
 *  Created on: 23-mar-2009
 *      Author: filosganga
 */

#ifndef REQUEST_H_
#define REQUEST_H_

char* request_header_value(const request_t* request, const char* name);

char** request_header_values(const request_t* request, const char* name);

#endif /* REQUEST_H_ */
