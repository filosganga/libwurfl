/*
 * device.h
 *
 *  Created on: 23-mar-2009
 *      Author: Filippo De Luca
 */
#ifndef DEVICE_H_
#define DEVICE_H_

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct _device_t device_t;

void device_destroy(device_t* device);

unsigned char* device_id(const device_t* device);

unsigned char* device_user_agent(const device_t* device);

unsigned char* device_capability(const device_t* device, const unsigned char* name);

int device_cmp(const void* ldevice, const void* rdevice);

uint32_t device_hash(const void* item);

bool device_eq(const void* litem, const void* ritem);

#endif /* DEVICE_H_ */
