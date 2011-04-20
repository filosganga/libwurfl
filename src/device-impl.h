/*
 * device-impl.h
 *
 *  Created on: 19/apr/2011
 *      Author: filippodeluca
 */

#ifndef DEVICE_IMPL_H_
#define DEVICE_IMPL_H_

#include "device.h"

#include "repository.h"
#include "devicedef.h"

struct _device_t {
	devicedef_t* root;
	repository_t* repository;
};

device_t* device_create(repository_t* repository, const devicedef_t* devicedef);

#endif /* DEVICE_IMPL_H_ */
