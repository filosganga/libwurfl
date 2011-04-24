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

#ifndef PATRICIA_H_
#define PATRICIA_H_

#include "utils.h"

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct _trie_t trie_t;

trie_t* trie_init(coll_duper_f* key_dupe, coll_unduper_f* key_undupe, void* dupe_xtra);

void trie_destroy(trie_t* trie, coll_unduper_f* undupe, void* undupe_data);

void trie_put(trie_t* trie, const void* key, const void* value);

void* trie_get(trie_t* trie, const void* key);

void* trie_search(trie_t* trie, const void* key);

bool trie_foreach(trie_t* trie, coll_functor_f* functor, void* functor_data);

bool trie_search_foreach(trie_t* trie, const void* key, coll_functor_f* functor, void* functor_data);

#endif /* PATRICIA_H_ */
