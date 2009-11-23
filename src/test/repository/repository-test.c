/*
 * repository-test.c
 *
 *  Created on: Nov 24, 2009
 *      Author: filosganga
 */

#include <CUnit/CUnit.h>

#include "repository-test.h";

#include "../../main/resource/resource.h"
#include "../../main/repository/repository.h"

void repository_size_should_be_right() {

	resource_t* resource = resource_create("etc/wurfl.xml");
	repository_t* repository = repository_create(resource, NULL);

	u_int32_t devices_size = repository_size(repository);

	CU_ASSERT_DOUBLE_EQUAL(devices_size, 12123, 0);
}

void repository_should_contains_generic() {

	resource_t* resource = resource_create("etc/wurfl.xml");
	repository_t* repository = repository_create(resource, NULL);

	devicedef_t* generic = repository_get_device(repository, "generic");

	CU_ASSERT_PTR_NOT_NULL(generic);
}
