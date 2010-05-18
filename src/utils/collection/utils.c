/*
 * utils.c
 *
 *  Created on: 28-mar-2009
 *      Author: filosganga
 */

#include "collections-impl.h"

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

static uint32_t hash_int_impl(uint32_t a);

static void nop_undupe(void* item, const void* xtra) {
	// Empty
}

static void allocator_undupe(void* item, const void* xtra) {
	const allocator_t* allocator = xtra;
	allocator_free(allocator, item);
}

void coll_init_nop_unduper(coll_unduper_t* unduper) {
	unduper->undupe = &nop_undupe;
	unduper->xtra = NULL;
}

void coll_init_allocator_unduper(coll_unduper_t* unduper, allocator_t* allocator) {
	unduper->undupe = &allocator_undupe;
	unduper->xtra = allocator;
}


// Functors ***************************************************************

int coll_finder(const void* item, void* xtra) {

	int found = 0;

	coll_finder_data_t* data = xtra;
	if(data->predicate->evaluate(item, data->predicate->data)) {
		if(data->nth==0) {
			data->found = item;
			found = 1;
		}
		else {
			data->nth--;
		}
	}

	return found;
}

// Primitive utility functions ********************************************

uint32_t string_hash(const void* item) {
   unsigned long h;
   char* string = (char*)item;

   h = 0;
   while (*string) {
      h = h * 37UL + (unsigned char) *string++;
   }
   return h;
}

int string_cmp(const void* litem, const void *ritem) {
	char* lstring = (char*)litem;
	char* rstring = (char*)ritem;

	return strcmp(lstring, rstring);
}

bool string_eq(const void* litem, const void *ritem) {
	char* lstring = (char*)litem;
	char* rstring = (char*)ritem;

	return strcmp(lstring, rstring)==0;
}


uint32_t ref_hash(const void* ref) {
	uint32_t* integer = (uint32_t*)ref;

	return hash_int_impl(integer);
}

int ref_cmp(const void* litem, const void *ritem) {
	return litem - ritem;
}

bool ref_eq(const void* litem, const void *ritem) {
	return litem == ritem;
}

uint32_t int_hash(const void* item) {
	uint32_t* integer = (uint32_t*)item;

	return hash_int_impl(*integer);
}

int int_cmp(const void* litem, const void *ritem) {
	uint32_t* lint = (uint32_t*)litem;
	uint32_t* rint = (uint32_t*)ritem;

	return *lint - *rint;
}

bool int_eq(const void* litem, const void *ritem) {
	uint32_t* lint = (uint32_t*)litem;
	uint32_t* rint = (uint32_t*)ritem;

	return *lint==*rint;
}

/**
 * Robert Jenkins' 32 bit integer hash function
 */
static uint32_t hash_int_impl(uint32_t a) {
   a = (a+0x7ed55d16) + (a<<12);
   a = (a^0xc761c23c) ^ (a>>19);
   a = (a+0x165667b1) + (a<<5);
   a = (a+0xd3a2646c) ^ (a<<9);
   a = (a+0xfd7046c5) + (a<<3);
   a = (a^0xb55a4f09) ^ (a>>16);
   return a;
}
