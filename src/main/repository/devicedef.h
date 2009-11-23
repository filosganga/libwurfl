/*
 * devicedef.h
 *
 *  Created on: 31-mar-2009
 *      Author: filosganga
 */

#ifndef DEVICEDEF_H_
#define DEVICEDEF_H_

typedef struct _devicedef_t devicedef_t;

void devicedef_destroy(devicedef_t* device);

char* devicedef_get_id(const devicedef_t* devicedef);

char* devicedef_get_user_agent(const devicedef_t* devicedef);

char* devicedef_get_fall_back(const devicedef_t* devicedef);

int devicedef_is_root(const devicedef_t* devicedef);

int devicedef_cmp(const void* ldevicedef, const void* rdevicedef);

int devicedef_equals(const void* ldevicedef, const void* rdevicedef);

unsigned long devicedef_hash(const void* devicedef);

unsigned long devicedef_rehash(const void* devicedef);

#endif /* DEVICEDEF_H_ */
