/*
 * devicedefinition.c
 *
 *  Created on: 28-mar-2009
 *      Author: Filippo De Luca
 */

#include "resource.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>

int devicedef_cmp(const void* litem, const void* ritem) {

	devicedef_t* ldevicedef = (devicedef_t*)litem;
	devicedef_t* rdevicedef = (devicedef_t*)ritem;

	return strcmp(ldevicedef->id, rdevicedef->id);
}

uint32_t devicedef_hash(const void* item) {

	devicedef_t* devicedef = (devicedef_t*)item;

	return string_hash(devicedef->id);
}

bool devicedef_eq(const void* litem, const void* ritem) {

	devicedef_t* ldevicedef = (devicedef_t*)litem;
	devicedef_t* rdevicedef = (devicedef_t*)ritem;

	return strcmp(ldevicedef->id, rdevicedef->id)==0;
}
