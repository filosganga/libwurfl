/*
 * device.h
 *
 *  Created on: 23-mar-2009
 *      Author: Filippo De Luca
 */
#ifndef DEVICE_H_
#define DEVICE_H_

#include "repository/repository.h"

typedef struct _device_t device_t;

device_t* device_create(device_hierarchy_t* hierarchy);

void device_destroy(device_t* device);

char* device_get_capability(device_t* device, char* name);

char* device_get_id(device_t* device);

char* device_get_user_agent(device_t* device);

#endif /* DEVICE_H_ */
