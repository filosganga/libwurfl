/*
 * wurfl.c
 *
 *  Created on: Apr 29, 2010
 *      Author: filosganga
 */

#include "wurfl.h"
#include "commons.h"

#include "repository/repository.h"
#include "matcher/matcher.h"

#include <stdio.h>
#include <datrie/trie.h>
#include <datrie/alpha-map.h>

int main(int argc, char **argv) {

	const char* test_ua="SAMSUNG-SGH-E708";
	const char* wurfl_path = "etc/wurfl.xml";

	repository_t* repository = repository_create(wurfl_path, NULL);
	matcher_t* matcher = matcher_create(repository);

	const char* matched = matcher_match(matcher, test_ua);

	fprintf(stderr, "End\n");
	printf("Device: %s\n", matched);

	// TODO free matcher
	// TODO free repository

	return 0;
}

