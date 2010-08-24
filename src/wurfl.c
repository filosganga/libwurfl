/*
 * wurfl.c
 *
 *  Created on: Apr 29, 2010
 *      Author: filosganga
 */

#include "wurfl.h"
#include "commons.h"

#include "repository/repository.h"

#include <stdio.h>

struct _wurfl_t {
	repository_t* repository;
};

void request_from_user_agent(request_t* request, const char* user_agent) {

	request->user_agent = user_agent;
}

wurfl_t* wurfl_init(const char* root, const char** patches) {

	wurfl_t* wurfl = malloc(sizeof(wurfl_t));
	if(wurfl==NULL) {
		// Error
	}

	repository_t* repository = repository_create(root, patches);

	wurfl->repository = repository;

	return wurfl;
}

void wurfl_destroy(wurfl_t* wurfl) {

	repository_destroy(wurfl->repository);

	free(wurfl);
}

device_t* wurfl_match(const wurfl_t* wurfl, const request_t request) {

	// TODO match
	return NULL;
}

int main(int argc, char **argv) {

	const char* test_ua="SAMSUNG-SGH-E708";
	const char* wurfl_path = "etc/wurfl.xml";

	wurfl_t* wurfl = wurfl_init(wurfl_path, NULL);

	request_t request = {test_ua};
	device_t* device = wurfl_match(wurfl, request);

	fprintf(stderr, "End\n");
	printf("Device: %s\n", device_id(device));

	wurfl_destroy(wurfl);

	return 0;
}

