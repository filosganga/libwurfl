/*
 * wurfl.c
 *
 *  Created on: Apr 29, 2010
 *      Author: filosganga
 */

#include "wurfl.h"

#include "repository.h"
#include "device-impl.h"
#include "devicedef.h"
#include "utils/utils.h"
#include "utils/functors.h"
#include "utils/error.h"

#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <errno.h>

extern int errno;

struct _wurfl_t {
	repository_t* repository;
	devicedef_t** devices;
	size_t size;
};

static int devicedef_user_agent_cmp(const void* left, const void* right) {

	devicedef_t** l = (devicedef_t**)left;
	devicedef_t** r = (devicedef_t**)right;

	return strcmp((*l)->user_agent, (*r)->user_agent);
}

wurfl_t* wurfl_init(const char* root, const char** patches) {

	wurfl_t* wurfl = malloc(sizeof(wurfl_t));
	if(wurfl==NULL) {
		error(1,errno,"error allocating memory to wurfl");
	}

	repository_t* repository = repository_create(root, patches);

	error(0,0,"repo created");

	wurfl->repository = repository;

	wurfl->devices = malloc(sizeof(devicedef_t*) * repository_size(repository));
	if(!wurfl->devices) {
		// FIXME memory error message
		exit(-1);
	}
	wurfl->size = repository_size(repository);

	functor_toarray_data_t toarray_data;
	toarray_data.array = wurfl->devices;
	toarray_data.index = 0;

	repository_foreach(wurfl->repository, &functor_toarray, &toarray_data);
	qsort(wurfl->devices, wurfl->size, sizeof(device_t*), &devicedef_user_agent_cmp);

	int i;
	for(i=0; i<wurfl->size; i++) {
		fprintf(stderr, "Device ua: %s\n", wurfl->devices[i]->user_agent);
	}

	return wurfl;
}

void wurfl_destroy(wurfl_t* wurfl) {

	repository_destroy(wurfl->repository);

	free(wurfl);
}

static int32_t calc_distance(const char* t1, const char* t2){

	int32_t t1_len = strlen(t1);
	int32_t t2_len = strlen(t2);

	int32_t min_len = t1_len < t2_len ? t1_len : t2_len;

    int32_t i = 0;
    while (i < min_len && t1[i] == t2[i]) i++;

    error(0,0,"distance of %s and %s is %d", t1, t2, i);

    return i;
}

typedef struct {
	int32_t distance;
	devicedef_t* device;
} best_match_t;

bool match(const char* needle, devicedef_t** candidates, size_t candidates_size, best_match_t* best_match) {

	int32_t needle_lenght = strlen(needle);

	bool matched = false;

	best_match->distance = -1;
	best_match->device = NULL;

	int32_t low = 0;
	int32_t high = candidates_size - 1;

	error(0,0,"needle: %s, nedle_len: %d, low: %d, high: %d", needle, needle_lenght, low, high);


	/*
	 * Perfect match: bestDistance == needle.lenght()
	 * Best match: bestDistance >= tolerance
	 * First match: bestDistance == tolerance
	 */
	while (low <= high && best_match->distance < needle_lenght) {
		int32_t mid = (low + high) / 2;
		devicedef_t* mid_candidate = candidates[mid];

		int32_t distance = calc_distance(needle, mid_candidate->user_agent);
		if (distance > best_match->distance){
			best_match->device = mid_candidate;
			best_match->distance = distance;
			matched = true;
		}

		// Calculate low and high
		int cmp = strcmp(mid_candidate->user_agent, needle);
		if (cmp < 0) {
			low = mid + 1;
		}
		else if (cmp > 0) {
			high = mid - 1;
		}
		else {
			break;
		}
	}

	return matched;
}

device_t* wurfl_match(const wurfl_t* wurfl, const char* user_agent) {


	best_match_t best_match;
	match(user_agent, wurfl->devices, wurfl->size, &best_match);

	return device_create(wurfl->repository, best_match.device);
}


