#ifndef THREADPTHREADIMPL_H_
#define THREADPTHREADIMPL_H_

#include "thread.h"

#ifdef __linux__
#include <pthread.h>

#define SLEEP_TIME struct timespec{0;100};

struct _tread_t {
	pthread_t handle;
};

struct _thread_mutex_t
{
	pthread_mutex_t handle;
};

struct _thread_cv_t {
	pthread_cond_t cond;
};

#endif // __linux__

#endif /*THREADPTHREADIMPL_H_*/
