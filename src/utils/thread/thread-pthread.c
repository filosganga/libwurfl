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

#include "thread-pthread-impl.h"

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#ifdef __linux__

typedef struct {
	int (*thread_function)(void*);
	void* thread_function_args;
} pthread_thread_function_p;

static void* pthread_thread_function(void* param) {
	int* exit_code;

	exit_code = malloc(sizeof(int));

	pthread_thread_function_p* pthread_thread_function_param =
			(pthread_thread_function_p*)param;

	int (*function)(void*) = pthread_thread_function_param->thread_function;
	void* args = pthread_thread_function_param->thread_function_args;

	printf("%s - %d: args = %p\n",__FILE__ ,__LINE__, args);

	*exit_code = function(args);

	printf("%s - %d: exit_code = %d\n",__FILE__,__LINE__, *exit_code);

	free(param);

	return exit_code;
}

thread_t* thread_create(thread_function_f thread_function,
		void* thread_function_args) {

	assert(thread_function != NULL);
	assert(thread_function_args != NULL);

	int res;

	// allocate memory to thread
	thread_t* thread = malloc(sizeof(thread_t));

	if (thread!=NULL) {

		pthread_thread_function_p* param =
				malloc(sizeof(pthread_thread_function_p));

		if (param!=NULL) {
			param->thread_function_args = thread_function_args;
			param->thread_function = thread_function;

			res = pthread_create(&thread->handle, NULL,
					pthread_thread_function, param);
			if (res!=0) {

				// free param and thread and ignore errors
				free(param);
				free(thread);
				thread = NULL;
			}

		} // param!=NULL
		else {

			free(thread);
			thread = NULL;

		}

	} // thread!=NULL

	return thread;
}

int thread_join(thread_t* thread) {

	assert(thread!=NULL);

	void* return_code;
	int exit_code;

	pthread_join(thread->handle, &return_code);

	exit_code = *((int*)return_code);

	free(return_code);

	return (int)exit_code;
}

thread_t* thread_whoami(void) {
	thread_t* thread = malloc(sizeof(thread_t));

	if (thread!=NULL) {
		thread->handle = pthread_self();
	}

	return thread;
}


#endif
