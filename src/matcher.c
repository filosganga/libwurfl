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

#include "devicedef.h"
#include "utils/patricia.h"
#include "utils/error.h"
#include "utils/functors.h"

#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <errno.h>
#include <assert.h>

#define LD_TOLL 5 // LD tolerance

extern int errno;

struct _matcher_t {
	trie_t* prefix;
};

typedef struct {
	const char* needle;
	hashmap_t* map;
} find_data_t;

static bool find(void* item, const void* xtra) {
	void** kv = (void**)item;
	find_data_t* data = (find_data_t*)xtra;

	const char* key = (char*)kv[0];

	hashmap_put(data->map, key, kv[1]);

	return strcmp(key, data->needle)==0;
}

matcher_t* matcher_init(repository_t* repo) {

	matcher_t* matcher = malloc(sizeof(matcher_t));
	if(!matcher) {
		error(1, errno, "error allocating matcher");
	}

	matcher->prefix = trie_init(NULL, NULL, NULL);

	functor_totrie_data_t totrie_data;
	totrie_data.trie = matcher->prefix;
	totrie_data.key_get = &devicedef_user_agent;
	repository_foreach(repo, &functor_totrie, &totrie_data);

	return matcher;
}

void matcher_destroy(matcher_t* matcher) {

	trie_destroy(matcher->prefix, NULL, NULL);
	free(matcher);
}

/**
 * <p>
 * Find the Levenshtein distance between two Strings.
 * </p>
 *
 * <p>
 * This is the number of changes needed to change one String into another,
 * where each change is a single character modification (deletion, insertion
 * or substitution).
 * </p>
 *
 * <p>
 * The previous implementation of the Levenshtein distance algorithm was
 * from <a
 * href="http://www.merriampark.com/ld.htm">http://www.merriampark.com
 * /ld.htm</a>
 * </p>
 *
 * <p>
 * Chas Emerick has written an implementation in Java, which avoids an
 * OutOfMemoryError which can occur when my Java implementation is used with
 * very large strings.<br>
 * This implementation of the Levenshtein distance algorithm is from <a
 * href="http://www.merriampark.com/ldjava.htm">http://www.merriampark.com/
 * ldjava.htm</a>
 * </p>
 *
 * <pre>
 * StringUtils.getLevenshteinDistance(null, *)             = IllegalArgumentException
 * StringUtils.getLevenshteinDistance(*, null)             = IllegalArgumentException
 * StringUtils.getLevenshteinDistance(&quot;&quot;,&quot;&quot;)               = 0
 * StringUtils.getLevenshteinDistance(&quot;&quot;,&quot;a&quot;)              = 1
 * StringUtils.getLevenshteinDistance(&quot;aaapppp&quot;, &quot;&quot;)       = 7
 * StringUtils.getLevenshteinDistance(&quot;frog&quot;, &quot;fog&quot;)       = 1
 * StringUtils.getLevenshteinDistance(&quot;fly&quot;, &quot;ant&quot;)        = 3
 * StringUtils.getLevenshteinDistance(&quot;elephant&quot;, &quot;hippo&quot;) = 7
 * StringUtils.getLevenshteinDistance(&quot;hippo&quot;, &quot;elephant&quot;) = 7
 * StringUtils.getLevenshteinDistance(&quot;hippo&quot;, &quot;zzzzzzzz&quot;) = 8
 * StringUtils.getLevenshteinDistance(&quot;hello&quot;, &quot;hallo&quot;)    = 1
 * </pre>
 *
 * @param s
 *            the first String, must not be null
 * @param t
 *            the second String, must not be null
 * @param tolerance
 *           the minimum distance
 * @return result distance
 * @throws IllegalArgumentException
 *             if either String input <code>null</code>
 */
static uint32_t levenshtein_distance(const char* s, const char* t, uint32_t tolerance) {
	assert(s != NULL);
	assert(t != NULL);

	if(tolerance == 0) {
		return strcmp(s, t) == 0?0:UINT32_MAX;
	}

	/*
	 * The difference between this impl. and the previous is that, rather
	 * than creating and retaining a matrix of size s.length()+1 by
	 * t.length()+1, we maintain two single-dimensional arrays of length
	 * s.length()+1. The first, d, is the 'current working' distance array
	 * that maintains the newest distance cost counts as we iterate through
	 * the characters of String s. Each time we increment the index of
	 * String t we are comparing, d is copied to p, the second int[]. Doing
	 * so allows us to retain the previous cost counts as required by the
	 * algorithm (taking the minimum of the cost count to the left, up one,
	 * and diagonally up and to the left of the current cost count being
	 * calculated). (Note that the arrays aren't really copied anymore, just
	 * switched...this is clearly much better than cloning an array or doing
	 * a System.arraycopy() each time through the outer loop.)
	 *
	 * Effectively, the difference between the two implementations is this
	 * one does not cause an out of memory condition when calculating the LD
	 * over two very large strings.
	 */

	size_t n = strlen(s); // length of s
	size_t m = strlen(t); // length of t

	if (n == 0) {
		return m;
	} else if (m == 0) {
		return n;
	}

	uint32_t previous[8 * 1024];  // 'previous' cost array, horizontally
	uint32_t costs[8 * 1024]; // cost array, horizontally

	uint32_t *p = previous;
	uint32_t *d = costs;
	uint32_t *tmp; // placeholder to assist in swapping p and d

	// indexes into strings s and t
	uint32_t i; // iterates through s
	uint32_t j; // iterates through t

	char t_j; // jth character of t

	uint32_t cost; // cost

	for (i = 0; i <= n; i++) {
		p[i] = i;
	}

	for (j = 1; j <= m; j++) {
		t_j = t[j - 1];
		d[0] = j;

		for (i = 1; i <= n; i++) {
			cost = s[i - 1]== t_j ? 0 : 1;
			// minimum of cell to the left+1, to the top+1, diagonally left
			// and up +cost
			d[i] = min(min(d[i - 1] + 1, p[i] + 1), p[i - 1] + cost);

			// Performance check
			if (i == j && d[i] > (tolerance + 3)) {
				return UINT32_MAX;
			}

		}

		// copy current distance counts to 'previous row' distance counts
		tmp = p;
		p = d;
		d = tmp;
	}

	// our last action in the above loop was to switch d and p, so p now
	// actually has the most recent cost counts
	return p[n];
}

/**
 * Searches for the string which has the minor Levenshtein distance from
 * given needle. If there is not candidates within given tolerance, it
 * returns null.
 *
 * @param candidates
 *            The SortedSet of possible candidates.
 * @param needle
 *            The String to match.
 * @param tolerance
 *            the tolerance between needle and candidates.
 *
 * @return Matched candidate String.
 */
static devicedef_t* match(devicedef_t** candidates, size_t candidates_size, const char* needle, uint32_t tolerance) {

	devicedef_t* match = NULL;

	uint32_t needle_len = strlen(needle);
	uint32_t best = tolerance;
	uint32_t current = needle_len;

	uint32_t i;
	for(i=0; current>0 && i<candidates_size; i++) {
		devicedef_t* candidate = candidates[i];

		if(abs(strlen(candidate->user_agent) - needle_len) < tolerance) {
			current = levenshtein_distance(candidate->user_agent, needle, tolerance);
			if(current < best || current == 0) {
				best = current;
				match = candidate;
			}
		}

	}

	return match;
}

devicedef_t* matcher_match(matcher_t* matcher, const char* user_agent) {

	find_data_t pfx_data;
	pfx_data.needle = user_agent;
	pfx_data.map = hashmap_create(&string_eq, &string_hash, NULL);
	if(trie_search_foreach(matcher->prefix, user_agent, &find, &pfx_data)) {
		return hashmap_get(pfx_data.map, user_agent);
	}
	else {

		functor_toarray_data_t toarray_data;
		toarray_data.index = 0;
		toarray_data.size = hashmap_size(pfx_data.map);
		toarray_data.array = malloc(sizeof(devicedef_t*) * toarray_data.size);
		if(!toarray_data.array) {
			error(1, errno, "error allocating candidates array");
		}
		hashmap_foreach(pfx_data.map, &functor_toarray, &toarray_data);

		return match(toarray_data.array, toarray_data.size, user_agent, LD_TOLL);
	}
}
