
#include "hashtable-impl.h"

#include "gnulib/error.h"

#include <assert.h>
#include <math.h>
#include <limits.h>
#include <errno.h>

extern int errno;

static void resize(hashtable_t* hashtable, int new_capacity) {

    if (hashtable->table_size == MAXIMUM_CAPACITY) {
    	hashtable->threshold = UINT32_MAX;
        return;
    }

    hashtable->table = realloc(hashtable->table, new_capacity);
    if(hashtable->table==NULL) {
    	error(-1, errno, "Errore resize hashtable");
    }

    hashtable->table_size = new_capacity;

    hashtable->threshold = new_capacity * hashtable->lfactor;
}

/**
 * Applies a supplemental hash function to a given hashCode, which
 * defends against poor quality hash functions.  This is critical
 * because HashMap uses power-of-two length hash tables, that
 * otherwise encounter collisions for hashCodes that do not differ
 * in lower bits. Note: Null keys always map to hash 0, thus index 0.
 */
static int reinforce_hash(const int weak_hash) {

	uint32_t hash = weak_hash;

	// This function ensures that hashCodes that differ only by
	// constant multiples at each bit position have a bounded
	// number of collisions (approximately 8 at default load factor).
	hash ^= (hash >> 20) ^ (hash >> 12);
	return hash ^ (hash >> 7) ^ (hash >> 4);
}

uint32_t index(const int32_t hash, uint32_t length) {
    return hash & (length-1);
}

hashtable_entry_t* find(hashtable_t* hashtable, uint32_t table_index, const void* item) {

	hashtable_entry_t* entry = *(hashtable->table + table_index);
	while(entry!=NULL && !hashtable->eq_fn(entry->item, item)) {
		entry = entry->next;
	}

	return entry;
}

hashtable_entry_t* add(hashtable_t* hashtable, uint32_t table_index, const void* item) {

	hashtable_entry_t* entry = malloc(sizeof(hashtable_entry_t));
	if(entry==NULL) {
		// TODO Error
	}

	entry->hash = hashtable->hash_fn(item);
	entry->item = item;
	entry->next = *(hashtable->table+table_index);

	*(hashtable->table+table_index) = entry;

    if (hashtable->size++ >= hashtable->threshold) {
    	resize(hashtable, 2 * hashtable->table_size);
    }

    return entry;
}

hashtable_entry_t* remove(hashtable_t* hashtable, uint32_t table_index, const void* item) {

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

hashtable_t* hashtable_create(coll_hash_f hash_fn, coll_equals_f eq_fn, uint32_t initial_capacity, float load_factor) {

	assert(initial_capacity > 0);
	assert(load_factor > 0);
	assert(!isnanf(load_factor));

	if (initial_capacity > MAXIMUM_CAPACITY) {
		initial_capacity = MAXIMUM_CAPACITY;
	}

	// Find a power of 2 >= initialCapacity
	int capacity = 1;
	while (capacity < initial_capacity) {
		capacity <<= 1;
	}

	hashtable_t* hashtable = malloc(sizeof(hashtable_t));
	if(hashtable==NULL) {
		// TODO error
	}

	hashtable->table = malloc(sizeof(hashtable_entry_t*) * capacity);
	if(hashtable->table==NULL) {
		free(hashtable);
		// TODO error
	}
	hashtable->table_size = capacity;

	hashtable->hash_fn = hash_fn;
	hashtable->eq_fn = eq_fn;

	hashtable->lfactor = load_factor;
	hashtable->threshold = capacity * load_factor;
	hashtable->size = 0;

	return hashtable;
}

void hashtable_destroy(hashtable_t* hashtable) {

	uint32_t tindex = 0;
	for(tindex=0; tindex<hashtable->table_size; tindex++) {

		hashtable_entry_t* entry = *(hashtable->table+tindex);
		while(entry!=NULL) {
			hashtable_entry_t* next = entry->next;
			free(entry);
			entry = next;
			*(hashtable->table+tindex)=entry;
		}
	}

	free(hashtable->table);
	free(hashtable);
}

void* hashtable_get(hashtable_t* hashtable, const void* item) {

	int hashcode = reinforce_hash(hashtable->hash_fn(item));
	uint32_t table_index = index(hashcode, hashtable->table_size);


	hashtable_entry_t* entry = find(hashtable, table_index, item);

	return entry!=NULL?entry->item:NULL;
}

void* hashtable_add(hashtable_t* hashtable, const void* item) {

	void* old_item = NULL;

	int hashcode = reinforce_hash(hashtable->hash_fn(item));
	uint32_t table_index = index(hashcode, hashtable->table_size);


	hashtable_entry_t* entry = find(hashtable, table_index, item);

	if(entry!=NULL) {
		old_item = entry->item;
		entry->item = item;
	}
	else {
		add(hashtable, table_index, item);
	}

	return old_item;
}

void* hashtable_remove(hashtable_t* hashtable, const void* item) {

	void* removed = NULL;

	int hashcode = reinforce_hash(hashtable->hash_fn(item));
	uint32_t table_index = index(hashcode, hashtable->table_size);

	hashtable_entry_t* entry = remove(hashtable, table_index, item);

	if(entry!=NULL) {
		removed = entry->item;
		free(entry);
	}

    return removed;
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

bool hashtable_iterate(hashtable_t* hashtable, coll_functor_t* functor) {

	uint32_t tindex = 0;
	bool finish = false;

	for(tindex=0; tindex<hashtable->table_size && !finish; tindex++) {

		hashtable_entry_t* entry = *(hashtable->table+tindex);
		while(entry!=NULL && !finish) {
			finish = functor->functor(entry->item, functor->data);
			entry = entry->next;
		}
	}

	return finish;
}

