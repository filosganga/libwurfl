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
