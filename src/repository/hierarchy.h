/*
 * hierarchy.h
 *
 *  Created on: 31-mar-2009
 *      Author: filosganga
 */

#ifndef HIERARCHY_H_
#define HIERARCHY_H_

#include "devicedef-impl.h"

typedef struct _device_hierarchy_t device_hierarchy_t;

void destroy_hierarchy(device_hierarchy_t* hierarchy);

int hierarchy_get_size(device_hierarchy_t* hierarchy);

char* hierarchy_get_capability(device_hierarchy_t* hierarchy, char* name);

devicedef_t* hierarchy_get_target(device_hierarchy_t* hierarchy);

devicedef_t* hierarchy_get_device(device_hierarchy_t* hierarchy, u_int32_t index);


#endif /* HIERARCHY_H_ */
