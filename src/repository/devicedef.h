/*
 * devicedef.h
 *
 *  Created on: 31-mar-2009
 *      Author: filosganga
 */

#ifndef DEVICEDEF_H_
#define DEVICEDEF_H_

#include "utils/collection/hashmap.h"
#include "utils/collection/collections.h"

#include <stdbool.h>

typedef struct _devicedef_t devicedef_t;

void devicedef_destroy(devicedef_t* device);

char* devicedef_get_id(const devicedef_t* devicedef);

char* devicedef_get_user_agent(const devicedef_t* devicedef);

char* devicedef_get_fall_back(const devicedef_t* devicedef);

int devicedef_is_root(const devicedef_t* devicedef);

char* devicedef_get_capability(const devicedef_t* devicedef, const char* name);

hashmap_t* devicedef_get_capabilities(const devicedef_t* devicedef);

int devicedef_cmp(const void* ldevicedef, const void* rdevicedef);

unsigned long devicedef_hash(const void* item);

unsigned long devicedef_rehash(const void* item);

bool devicedef_eq(const void* litem, const void* ritem);

#endif /* DEVICEDEF_H_ */
