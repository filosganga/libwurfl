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

#include <libxml/xmlstring.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <math.h>
#include <iconv.h>
#include <errno.h>

extern int errno;

struct _wurfl_t {
	repository_t* repository;
	devicedef_t** devices;
	size_t size;
};

static xmlChar* devicedef_user_agent(devicedef_t* device) {
	return device->user_agent;
}

static int devicedef_user_agent_cmp(const void* left, const void* right) {

	devicedef_t** l = left;
	devicedef_t** r = right;

	return xmlStrcmp((*l)->user_agent, (*r)->user_agent);
}

wurfl_t* wurfl_init(const char* root, const char** patches) {

	wurfl_t* wurfl = malloc(sizeof(wurfl_t));
	if(wurfl==NULL) {
		// Error
	}

	repository_t* repository = repository_create(root, patches);

	wurfl->repository = repository;

	hashmap_options_t devices_opts = {10000, 1.5f};
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

	return wurfl;
}

void wurfl_destroy(wurfl_t* wurfl) {

	repository_destroy(wurfl->repository);

	free(wurfl);
}

static int32_t calc_distance(const xmlChar* t1, const xmlChar* t2){

	uint32_t t1_len = xmlStrlen(t1);
	uint32_t t2_len = xmlStrlen(t2);

	uint32_t t;
	if(t1_len < t2_len) {
		t = t1_len;
	}
	else {
		t = t2_len;
	}

    uint32_t i = 0;
    while (i < t && t1[i] == t2[i]) i++;

    return i;
}

typedef struct {
	int32_t distance;
	devicedef_t* device;
} best_match_t;

bool match(const xmlChar* needle, devicedef_t** candidates, size_t candidates_size, best_match_t* best_match) {

	int32_t needle_lenght = xmlStrlen(needle);

	bool matched = false;

	best_match->distance = -1;
	best_match->device = NULL;

	int32_t low = 0;
	int32_t high = candidates_size - 1;


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
		int cmp = xmlStrcmp(mid_candidate->user_agent, needle);
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

device_t* wurfl_match(const wurfl_t* wurfl, const wchar_t *user_agent, const char *encoding) {

	device_t* device = NULL;

	size_t user_agent_len = wcslen(user_agent);
	size_t inbytesleft = user_agent_len * sizeof(wchar_t);

	size_t outbytesleft = 255 * sizeof(xmlChar);
	xmlChar* needle = malloc(outbytesleft);
	if(needle==NULL) {
		error(-1, errno, "error allocating user_agent UTF-8 string");
	}

	iconv_t cd = iconv_open("UTF-8", encoding);
	if(cd == -1) {
		error(0, errno, "iconv does not support %s encoding", encoding);
	}
	else {

		size_t result = iconv(cd, (const char**)&user_agent, &inbytesleft, (char**)&needle, &outbytesleft);
		iconv_close(cd);

		best_match_t best_match;
		match(needle, wurfl->devices, wurfl->size, &best_match);

		device = device_create(wurfl->repository, best_match.device);
	}

	return device;
}


