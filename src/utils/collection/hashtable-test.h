/*
 * hashtable-test.h
 *
 *  Created on: May 17, 2010
 *      Author: filosganga
 */

#ifndef HASHTABLETEST_H_
#define HASHTABLETEST_H_

void hashtable_create_should_return_not_null();

void hashtable_add_should_increase_size();

void hashtable_remove_should_decrease_size();

void hashtable_add_should_return_relaced();

void hashtable_add_with_same_hash_should_increase_size();


#endif /* HASHTABLETEST_H_ */
