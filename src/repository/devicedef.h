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

bool devicedef_eq(const void* litem, const void* ritem);


typedef struct _devicedefs_t devicedefs_t;

devicedef_t* devicedefs_get(const char* id);

bool devicedefs_contains(const char* id);

int devicedefs_iterate(coll_functor_t* functor);

#endif /* DEVICEDEF_H_ */
