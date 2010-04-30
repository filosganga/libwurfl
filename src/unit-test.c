/*
 * unit-test.c
 *
 *  Created on: Nov 23, 2009
 *      Author: Filippo de Luca
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include <sys/time.h>

#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>

#include "wurfl.h"

#include "commons.h"

#include "utils/collection/collections.h"
#include "utils/collection/hashset.h"
#include "utils/hashlib/cokusmt.h"
#include "utils/memory/allocator.h"

#include "repository/repository-test.h"
#include "matcher/matcher-test.h"
#include "utils/collection/hashset-test.h"

int main(int argc, char **argv) {

	main_allocator = allocator_create();

	CU_Suite* suite;

	if (CUE_SUCCESS != CU_initialize_registry()) {
		return CU_get_error();
	}

	suite = CU_add_suite("main", NULL, NULL);
	if (suite == NULL) {
		CU_cleanup_registry();
		return CU_get_error();
	}

	CU_add_test(suite, "hashset_create_should_return_not_null", &hashset_create_should_return_not_null);
	CU_add_test(suite, "hashset_size_should_return_right_size", &hashset_size_should_return_right_size);
	CU_add_test(suite, "repository_size_should_be_right", &repository_size_should_be_right);
	CU_add_test(suite, "repository_should_contains_generic", &repository_should_contains_generic);
	CU_add_test(suite, "matcher_create_should_return_not_null", &matcher_create_should_return_not_null);
	CU_add_test(suite, "matcher_match_should_return_valid_value", &matcher_match_should_return_valid_value);

	CU_basic_set_mode(CU_BRM_VERBOSE);
	CU_basic_run_tests();

	CU_basic_show_failures(CU_get_failure_list());

	CU_cleanup_registry();



	return CU_get_error();
}
