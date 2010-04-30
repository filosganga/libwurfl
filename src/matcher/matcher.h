/*
 * matcher.h
 *
 *  Created on: Apr 29, 2010
 *      Author: filosganga
 */

#ifndef MATCHER_H_
#define MATCHER_H_

#include "../repository/repository.h"

typedef struct matcher_t_ matcher_t;

matcher_t* matcher_create(repository_t* repository);

const char* matcher_match(matcher_t* matcher, const char* user_agent);

#endif /* MATCHER_H_ */
