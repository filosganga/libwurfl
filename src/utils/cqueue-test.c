/*
 * cqueue-test.c
 *
 *  Created on: Jan 25, 2010
 *      Author: filosganga
 */

#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>

#include <stdio.h>
#include <strings.h>
#include <memory.h>

#include "cqueue-test.h"

#include "utils/memory/allocator.h"

void cqueue_create_should_returnnotnull() {

	allocator_t* allocator = allocator_create();

	cqueue_t* cqueue = cqueue_create(allocator, sizeof(char) * 16);

	CU_ASSERT_PTR_NOT_NULL(cqueue);
}

void cqueue_read_should_read() {

	char* buffer = "01234567890123456789012345678901"; //32
	allocator_t* allocator = allocator_create();

	cqueue_t* cqueue = cqueue_create(allocator, sizeof(char) * 16);

	char *tmp = malloc(strlen(buffer));

	cqueue_write(cqueue, buffer, 8);
	cqueue_read(cqueue, tmp,8);

	CU_ASSERT_TRUE(memcmp(tmp, buffer, 8)==0);

	cqueue_write(cqueue, buffer, 10);
	cqueue_read(cqueue, tmp, 10);

	CU_ASSERT_TRUE(memcmp(tmp, buffer, 10)==0);

	cqueue_write(cqueue, buffer, 10);
	cqueue_write(cqueue, buffer + 10, 6);
	cqueue_read(cqueue, tmp, 16);

	CU_ASSERT_TRUE(memcmp(tmp, buffer, 16)==0);

	free(tmp);
}
