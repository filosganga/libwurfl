/*
 * Copyright 2011 ff-dev.org
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/* Written by Filippo De Luca <me@filippodeluca.com>.  */

#include "matcher.h"
#include "utils/patricia.h"
#include "utils/error.h"
#include "utils/functors.h"


#include <stdlib.h>
#include <errno.h>

extern int errno;

struct _matcher_t {
	trie_t* ptrie;
};

static void* devicedef_user_agent(const void* item) {
	devicedef_t* device = (devicedef_t*)item;

	return (void*)device->user_agent;
}

matcher_t* matcher_init(repository_t* repo) {

	matcher_t* matcher = malloc(sizeof(matcher_t));
	if(!matcher) {
		error(1, errno, "error allocating matcher");
	}

	matcher->ptrie = trie_init();

	functor_totrie_data_t totrie_data;
	totrie_data.trie = matcher->ptrie;
	totrie_data.key_get = &devicedef_user_agent;
	repository_foreach(repo, &functor_totrie, &totrie_data);

	return matcher;
}

void matcher_destroy(matcher_t* matcher) {

	trie_destroy(matcher->ptrie, NULL, NULL);
	free(matcher);
}

devicedef_t* matcher_match(matcher_t* matcher, const char* user_agent) {

	devicedef_t* matched = (devicedef_t*)trie_search(matcher->ptrie, user_agent);

	return matched;
}
