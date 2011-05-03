/* Copyright (C) 2011 Fantayeneh Asres Gizaw, Filippo De Luca
 *  
 * This file is part of libWURFL.
 *
 * libWURFL is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or 
 * any later version.
 *
 * libWURFL is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
 */
 
/* Written by Filippo De Luca <me@filippodeluca.com>.  */

#ifndef PATRICIA_H_
#define PATRICIA_H_

#include "utils.h"

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct _patricia_t patricia_t;

patricia_t* patricia_init(coll_duper_f* key_dupe, coll_unduper_f* key_undupe, void* dupe_xtra);

void patricia_free(patricia_t* trie, coll_unduper_f* undupe, void* undupe_data);

void patricia_put(patricia_t* trie, const void* key, const void* value);

void* patricia_get(patricia_t* trie, const void* key);

void* patricia_search(patricia_t* trie, const void* key);

bool patricia_foreach(patricia_t* trie, coll_functor_f* functor, void* functor_data);

bool patricia_search_foreach(patricia_t* trie, const void* key, coll_functor_f* functor, void* functor_data);

#endif /* PATRICIA_H_ */
