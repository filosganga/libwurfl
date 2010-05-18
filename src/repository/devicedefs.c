/*
 * devicedefs.c
 *
 *  Created on: May 18, 2010
 *      Author: filosganga
 */

#include "devicedef-impl.h"

#include "gnulib/error.h"

#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

devicedefs_t* devicedefs_create_frommap(hashmap_t* devices) {

	devicedefs_t* devicedefs = malloc(sizeof(devicedefs_t));
	if(devices==NULL) {
		// TODO error
	}

	devicedefs->devices = devices;

	return devicedefs;
}

devicedefs_t devicedefs_create_fromset(hashtable_t* devices) {
}

devicedefs_t devicedefs_create_fromarray(devicedef_t** devices) {

}
