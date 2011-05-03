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
#include <assert.h>

#ifdef __linux__

thread_cv_t* thread_cv_create() {
	int err_code;

	thread_cv_t* cv = malloc(sizeof(thread_cv_t));

	if (cv!=NULL) {

		err_code = pthread_cond_init(&cv->cond,NULL);
		if(err_code!=0){
			free(cv);
			cv=NULL;
		}
	}

	return cv;
} // thread_cv_create

void thread_cv_destroy(thread_cv_t* cv) {
	assert(cv!=NULL);

	pthread_cond_destroy(&cv->cond);
	free(cv);
} // thread_cv_destroy

int thread_cv_wait(thread_cv_t* cv, thread_mutex_t* guard) {
	int err_code;
	int exit_code;

	assert(cv!=NULL);
	assert(guard!=NULL);

	err_code = pthread_cond_wait(&cv->cond, &guard->handle);
	if (err_code!=0) {

		// TODO errore
		exit_code = -1;
	}

	return exit_code;

} // thread_cv_wait

int thread_cv_notify(thread_cv_t* cv) {

	int err_code=0;
	int exit_code=0;

	assert(cv!=NULL);

	err_code = pthread_cond_signal(&cv->cond);
	if (err_code!=0) {

		exit_code = -1;
	}

	return exit_code;
} // thread_cv_notify

int thread_cv_notifyall(thread_cv_t* cv) {

	int err_code=0;
	int exit_code=0;

	assert(cv!=NULL);

	err_code = pthread_cond_broadcast(&cv->cond);

	if (err_code!=0) {
		// TODO Errore
		exit(-1);
	}

	return exit_code;

} // thread_cv_notifyall

#endif
