/*
 * devidedef-impl.h
 *
 *  Created on: 31-mar-2009
 *      Author: filosganga
 */

#ifndef DEVIDEDEFIMPL_H_
#define DEVIDEDEFIMPL_H_

#include "devicedef.h"
#include "../collection/hashmap.h"
#include "../collection/collections.h"

devicedef_t* devicedef_create(char* id, char* user_agent, char* fall_back, int actual_device_root, hashmap_t* capabilities, hashmap_t* groups);

#endif /* DEVIDEDEFIMPL_H_ */
