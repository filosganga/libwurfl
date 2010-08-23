/*
 * device-impl.h
 *
 *  Created on: 23/ago/2010
 *      Author: filippodeluca
 */

#ifndef DEVICE_IMPL_H_
#define DEVICE_IMPL_H_

#include "device.h"
#include "utils/collection/hashmap.h"

struct _device_t {
	const char* id;
	const char* user_agent;
	hashmap_t* capabilities;
	device_t* parent;
};

device_t* device_create(const char* id, const char* user_agent, hashmap_t* capabilities, device_t* parent);

void device_destroy(device_t* device);

#endif /* DEVICE_IMPL_H_ */
