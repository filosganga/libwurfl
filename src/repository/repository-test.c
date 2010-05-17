/*
 * repository-test.c
 *
 *  Created on: Nov 24, 2009
 *      Author: filosganga
 */

#include <CUnit/CUnit.h>
#include <stdio.h>

#include "repository-test.h"

#include "resource/resource.h"
#include "repository.h"

void repository_size_should_be_right() {

	resource_t* resource = resource_create("etc/wurfl.xml");
	repository_t* repository = repository_create(resource, NULL);
	resource_destroy(resource);

	u_int32_t devices_size = repository_size(repository);

	fprintf(stderr, "Repository size: %d", devices_size);

	CU_ASSERT_DOUBLE_EQUAL(devices_size, 12123, 0);
}

void repository_should_contains_generic() {

	resource_t* resource = resource_create("etc/wurfl.xml");
	repository_t* repository = repository_create(resource, NULL);
	resource_destroy(resource);

	devicedef_t* generic = repository_get_device(repository, "generic");

	CU_ASSERT_PTR_NOT_NULL(generic);
}

static int notnull_functor(void* item, void* data) {

	CU_ASSERT_PTR_NOT_NULL(item);

	return 0;
}

void repository_get_devices_shouldnt_contains_null() {

	resource_t* resource = resource_create("etc/wurfl.xml");
	repository_t* repository = repository_create(resource, NULL);
	resource_destroy(resource);

	hashset_t* devices = repository_get_devices(repository);

	coll_functor_t functor;
	functor.functor = &notnull_functor;
	functor.data=NULL;

	hashset_foreach(devices, &functor);
}
