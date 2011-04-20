
#include "hashtable-impl.h"
#include "functors.h"

#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <limits.h>
#include <errno.h>
#include <string.h>

extern int errno;

static void hashtable_resize(hashtable_t* hashtable, int new_capacity) {

	uint32_t index;

    if (hashtable->table_size == MAXIMUM_CAPACITY) {
    	hashtable->threshold = UINT32_MAX;
        return;
    }

    hashtable_entry_t** old_table = hashtable->table;
	uint32_t old_capacity = hashtable->table_size;

    hashtable_entry_t** new_table =  malloc(sizeof(hashtable_entry_t*) * new_capacity);
    if(new_table!=NULL) {

    	// reset table
		// Reset table
    	// FIXME use memset
		for(index=0; index<new_capacity; index++) {
			*(new_table+index)=NULL;
		}

        hashtable->table = new_table;
        hashtable->table_size = new_capacity;
        hashtable->table_used = 0;
        hashtable->size = 0;
        hashtable->threshold = new_capacity * hashtable->lfactor;

    	for(index=0; index<old_capacity; index++) {

    		hashtable_entry_t* entry = *(old_table + index);
    		while(entry!=NULL) {
    			void* item = entry->item;
    			hashtable_add(hashtable, item);
    			hashtable_entry_t* this_entry = entry;
    			entry=entry->next;
    			free(this_entry);
    		}
    	}

    	free(old_table);
    }
    else {
    	// TODO errore
    	//error(-1, errno, "Errore resize hashtable");
    }

}

/**
 * Applies a supplemental hash function to a given hashCode, which
 * defends against poor quality hash functions.  This is critical
 * because HashMap uses power-of-two length hash tables, that
 * otherwise encounter collisions for hashCodes that do not differ
 * in lower bits. Note: Null keys always map to hash 0, thus index 0.
 */
static int hashtable_reinforce_hash(const int weak_hash) {

	uint32_t hash = weak_hash;

	// This function ensures that hashCodes that differ only by
	// constant multiples at each bit position have a bounded
	// number of collisions (approximately 8 at default load factor).
	hash ^= (hash >> 20) ^ (hash >> 12);
	return hash ^ (hash >> 7) ^ (hash >> 4);
}

hashtable_t* hashtable_create(coll_equals_f eq_fn, coll_hash_f hash_fn, hashtable_options_t* options) {

	if(options==NULL) {
		hashtable_options_t default_opts = {16, 0.75f};
		options = &default_opts;
	}

	assert(options->initial_capacity > 0);
	assert(options->load_factor > 0);
	// gnulib assert(!isnanf(options->load_factor));

	uint32_t initial_capacity = options->initial_capacity;
	if (initial_capacity > MAXIMUM_CAPACITY) {
		initial_capacity = MAXIMUM_CAPACITY;
	}

	// Find a power of 2 >= initialCapacity
	uint32_t capacity = 1;
	while (capacity < initial_capacity) {
		capacity <<= 1;
	}

	hashtable_t* hashtable = malloc(sizeof(hashtable_t));
	if(hashtable==NULL) {
		// TODO error
		//error(-1, errno, "Error allocating hashtable");
	}

	hashtable_entry_t** table = malloc(sizeof(hashtable_entry_t*) * capacity);
	if(table==NULL) {
		free(hashtable);
		// TODO error
		//error(-1, errno, "Error allocating hashtable table");
	}
	else {
		// Reset table
		uint32_t i;
		for(i=0; i<capacity; i++) {
			*(table+i)=NULL;
		}

		hashtable->table = table;
		hashtable->table_size = capacity;
		hashtable->table_used = 0;
		hashtable->size = 0;
	}

	hashtable->hash_fn = hash_fn;
	hashtable->eq_fn = eq_fn;

	hashtable->lfactor = options->load_factor;
	hashtable->threshold = capacity * options->load_factor;

	return hashtable;
}

void hashtable_destroy(hashtable_t* hashtable, coll_unduper_f unduper, void* unduper_data) {

	hashtable_clear(hashtable, unduper, unduper_data);

	free(hashtable->table);
	free(hashtable);
}

void* hashtable_get(hashtable_t* hashtable, const void* item) {

	int hashcode = hashtable_reinforce_hash(hashtable->hash_fn(item));
	uint32_t table_index = hashtable_index(hashcode, hashtable->table_size);

	hashtable_entry_t* entry = hashtable_find_entry(hashtable, table_index, item);

	return entry!=NULL?entry->item:NULL;
}

void* hashtable_add(hashtable_t* hashtable, const void* item) {

	void* old_item = NULL;

	int hashcode = hashtable_reinforce_hash(hashtable->hash_fn(item));
	uint32_t table_index = hashtable_index(hashcode, hashtable->table_size);

	hashtable_entry_t* entry = hashtable_find_entry(hashtable, table_index, item);

	if(entry!=NULL) {
		old_item = entry->item;
		entry->item = item;
	}
	else {
		hashtable_add_entry(hashtable, table_index, item);
	}

	return old_item;
}

void* hashtable_remove(hashtable_t* hashtable, const void* item) {

	void* removed = NULL;

	int hashcode = hashtable_reinforce_hash(hashtable->hash_fn(item));
	uint32_t table_index = hashtable_index(hashcode, hashtable->table_size);

	hashtable_entry_t* entry = hashtable_remove_entry(hashtable, table_index, item);

	if(entry!=NULL) {
		removed = entry->item;
		free(entry);
	}

    return removed;
}

void hashtable_clear(hashtable_t* hashtable, coll_unduper_f unduper, void* unduper_data) {

	uint32_t tindex = 0;
	for(tindex=0; tindex<hashtable->table_size; tindex++) {

		hashtable_entry_t* entry = *(hashtable->table+tindex);
		while(entry!=NULL) {
			hashtable_entry_t* next = entry->next;
			*(hashtable->table+tindex)=next;

			if(unduper!=NULL) {
				unduper(entry->item, unduper_data);
			}

			free(entry);
			entry = next;
		}
	}

}

bool hashtable_contains(hashtable_t* hashtable, void* item) {
	return hashtable_get(hashtable, item)!=NULL;
}

uint32_t hashtable_size(hashtable_t* hashtable) {
	return hashtable->size;
}

bool hashtable_empty(hashtable_t* hashtable) {
	return hashtable_size(hashtable)==0;
}

bool hashtable_foreach(hashtable_t* hashtable, coll_functor_f functor, void* functor_data) {

	uint32_t tindex = 0;
	bool finish = false;

	for(tindex=0; tindex<hashtable->table_size && !finish; tindex++) {

		hashtable_entry_t* entry = *(hashtable->table+tindex);
		while(entry!=NULL && !finish) {
			finish = functor(entry->item, functor_data);
			entry = entry->next;
		}
	}

	return finish;
}

//void* hashtable_find(hashtable_t* hashtable, coll_predicate_t* predicate, void* predicate_data) {
//
//	functor_find_data_t find_data;
//	find_data.predicate = predicate;
//
//	hashtable_foreach(hashtable, &functor_find, find_data);
//
//	void* found = find_data.found;
//
//	return found;
//}

void hashtable_toarray(hashtable_t* hashtable, void** array) {

	assert(hashtable!=NULL);
	assert(array!=NULL);

	functor_toarray_data_t functor_data;
	functor_data.array = array;
	functor_data.index = 0;

	hashtable_foreach(hashtable, &functor_toarray, &functor_data);
}

uint32_t hashtable_index(const int32_t hash, uint32_t length) {
    return hash & (length-1);
}

hashtable_entry_t* hashtable_find_entry(hashtable_t* hashtable, uint32_t table_index, const void* item) {

	hashtable_entry_t* entry = *(hashtable->table + table_index);
	while(entry!=NULL && !hashtable->eq_fn(entry->item, item)) {
		entry = entry->next;
	}

	return entry;
}

hashtable_entry_t* hashtable_add_entry(hashtable_t* hashtable, uint32_t table_index, const void* item) {

	hashtable_entry_t* bucket_head = *(hashtable->table+table_index);

	// Create entry
	hashtable_entry_t* entry = malloc(sizeof(hashtable_entry_t));
	entry->hash = hashtable->hash_fn(item);
	entry->item = item;
	entry->next = bucket_head;

	*(hashtable->table+table_index) = entry;
	hashtable->size++;

	// Bucket added
	if(bucket_head==NULL) {
		hashtable->table_used++;
		if (hashtable->table_used >= hashtable->threshold) {
			hashtable_resize(hashtable, 2 * hashtable->table_size);
		}
	}

    return entry;
}

hashtable_entry_t* hashtable_remove_entry(hashtable_t* hashtable, uint32_t table_index, const void* item) {

    hashtable_entry_t *entry = *(hashtable->table + table_index);
    hashtable_entry_t *prev = NULL;

    while(entry != NULL && !hashtable->eq_fn(entry->item, item)) {
    	prev = entry;
    	entry = entry->next;
    }

    if(entry!=NULL) {
    	if(prev!=NULL) {
    		prev->next = entry->next;
    	}
    	else {
    		*(hashtable->table + table_index) = entry->next;
    	}

    	hashtable->size--;
    }

    return entry;
}
