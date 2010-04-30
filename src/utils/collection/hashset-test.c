/*
 * hashset-test.c
 *
 *  Created on: Nov 24, 2009
 *      Author: filosganga
 */

#include <CUnit/CUnit.h>

#include <stdio.h>

#include "hashset-test.h"
#include "utils/collection/hashset.h"
#include "utils/hashlib/cokusmt.h"

void hashset_create_should_return_not_null() {

	hashset_t* set = hashset_create(&string_hash, &string_rehash, &string_eq);

	CU_ASSERT_PTR_NOT_NULL(set);

	hashset_destroy(set);
}

void hashset_size_should_return_right_size() {

	int expected = 100;

	hashset_t* set = hashset_create(&int_hash, &int_rehash, &int_eq);

	for (int i = 0; i < expected; i++) {
		hashset_add(set, &i);
	}

	CU_ASSERT_TRUE(hashset_size(set) == expected)

	hashset_destroy(set);
}
