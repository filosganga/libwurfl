/*
 * matcher-test.c
 *
 *  Created on: Apr 29, 2010
 *      Author: filosganga
 */

#include <CUnit/CUnit.h>
#include <stdio.h>

#include "matcher.h"

#include "resource/resource.h"
#include "repository/repository.h"

void matcher_create_should_return_not_null() {

	resource_t* resource = resource_create("etc/wurfl.xml");
	repository_t* repository = repository_create(resource, NULL);

	matcher_t* matcher = matcher_create(repository);

	CU_ASSERT_PTR_NOT_NULL(matcher);
}

void matcher_match_should_return_valid_value() {

	resource_t* resource = resource_create("etc/wurfl.xml");
	repository_t* repository = repository_create(resource, NULL);

	matcher_t* matcher = matcher_create(repository);
	char* matched = matcher_match(matcher, "AUDIOVOX-CDM9100");

	fprintf(stderr, "Matched: %s", matched);

	CU_ASSERT_PTR_NOT_NULL(matched);
}
