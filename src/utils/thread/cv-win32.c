#include "thread-win32-impl.h"

#ifdef _WIN32


thread_cv_t * thread_cv_create() {
	// TODO implement
	HANDLE* heap_handle = NULL;
}

int thread_cv_wait(thread_cv_t* cv, thread_mutex_t* mutex);

int thread_cv_timedwait(thread_cv_t* cv);

int thread_cv_notify(thread_cv_t* cv);
int thread_cv_notifyall(thread_cv_t* cv);

void thread_cv_destroy(thread_cv_t* cv);


#endif
