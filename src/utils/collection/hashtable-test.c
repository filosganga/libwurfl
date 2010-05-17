/*
 * hashtable-test.c
 *
 *  Created on: May 17, 2010
 *      Author: filosganga
 */


#include "hashtable-test.h"

#include "hashtable.h"
#include "utils/hashlib/cokusmt.h"

#include <CUnit/CUnit.h>
#include <stdio.h>
#include <string.h>


void hashtable_create_should_return_not_null() {

	hashtable_t* hashtable = hashtable_create(&string_hash, &string_eq, 16, 0.75f);

	CU_ASSERT_PTR_NOT_NULL(hashtable);
	CU_ASSERT_EQUAL(hashtable_size(hashtable), 0);
	CU_ASSERT_EQUAL(hashtable_empty(hashtable), true);

	hashtable_destroy(hashtable);
}

void hashtable_add_should_increase_size() {

	const char* expected = "TEST";

	hashtable_t* hashtable = hashtable_create(&string_hash, &string_eq, 16, 0.75f);

	void* added = hashtable_add(hashtable, expected);

	CU_ASSERT_EQUAL(hashtable_size(hashtable), 1);
	CU_ASSERT_EQUAL(hashtable_empty(hashtable), false);
	CU_ASSERT_PTR_NULL(added);

	hashtable_destroy(hashtable);
}

void hashtable_remove_should_decrease_size() {

	const char* expected = "TEST";

	hashtable_t* hashtable = hashtable_create(&string_hash, &string_eq, 16, 0.75f);

	void* replaced = hashtable_add(hashtable, expected);
	void* removed = hashtable_remove(hashtable, expected);

	CU_ASSERT_EQUAL(hashtable_size(hashtable), 0);
	CU_ASSERT_EQUAL(hashtable_empty(hashtable), true);
	CU_ASSERT_PTR_EQUAL(removed, expected);

	hashtable_destroy(hashtable);
}

void hashtable_add_should_return_relaced() {

	const char* expected = "TEST";

	hashtable_t* hashtable = hashtable_create(&string_hash, &string_eq, 16, 0.75f);

	hashtable_add(hashtable, expected);
	void* replaced = hashtable_add(hashtable, expected);


	CU_ASSERT_EQUAL(hashtable_size(hashtable), 1);
	CU_ASSERT_EQUAL(hashtable_empty(hashtable), false);
	CU_ASSERT_PTR_EQUAL(replaced, expected);

	hashtable_destroy(hashtable);
}

static uint32_t dummy_hash(const void* item) {
	return 5;
}

void hashtable_add_with_same_hash_should_increase_size() {

	const uint32_t expected_size = 100;

	const char* expected = "TEST";

	hashtable_t* hashtable = hashtable_create(&dummy_hash, &string_eq, 16, 0.75f);

	for(int x=0; x<expected_size; x++) {

		char word[9];
		memset(word, '\0', 9);
		for(int c=0; c<8; c++) {
			word[c] = (char)randomMT()%25 + 65;
		}

		const char* adding = strdup(word);
		hashtable_add(hashtable, adding);
	}

	CU_ASSERT_EQUAL(hashtable_size(hashtable), expected_size);
	CU_ASSERT_EQUAL(hashtable_empty(hashtable), false);

	hashtable_destroy(hashtable);
}
