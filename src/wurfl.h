/*
 * wurfl.h
 *
 *  Created on: 23-mar-2009
 *      Author: Filippo De Luca
 */
#include "device.h"

#include "utils/memory/allocator.h"

#ifndef WURFL_H_
#define WURFL_H_

device_t* get_device_from_request(void* request);

device_t* get_device_from_user_agent(char* user_agent);

#endif /* WURFL_H_ */
