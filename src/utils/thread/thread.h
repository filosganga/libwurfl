#ifndef THREAD_H_
#define THREAD_H_

enum thread_error {
	TH_OK, TH_TOO_MANY_THREADS
};

enum mutex_errors {
	MX_OK = 0,
	MX_FAILED,
	MX_NOT_OWNED,
	MX_ABANDONED,
	MX_TIMEOUT,
	MX_MAXLOCK,
	MX_BUSY,
	MX_NOMEM,
	MX_MAX
};

typedef struct _tread_t thread_t;

typedef struct _thread_mutex_t thread_mutex_t;

typedef struct _thread_cv_t thread_cv_t;

typedef int (*thread_function_f)(void* thread_args);

// Thread *****************************************************************

thread_t* thread_create(thread_function_f thread_function,
		void* thread_function_args);

int thread_join(thread_t* thread);

void thread_exit(void* status);

void* thread_terminate(thread_t* thread);

thread_t* thread_whoami(void);

// ThreadMutex ************************************************************

thread_mutex_t* thread_mutex_create();

int thread_mutex_destroy(thread_mutex_t *mutex);

int thread_mutex_lock(thread_mutex_t *mutex);

int thread_mutex_unlock(thread_mutex_t *mutex);

int thread_mutex_timedlock(thread_mutex_t *mutex, unsigned long int timeout);

int thread_mutex_try_lock(thread_mutex_t *mutex);

int thread_mutex_locks(thread_mutex_t* mutexes[], unsigned int mutexes_lenght);

int thread_mutex_try_locks(thread_mutex_t* mutexes[], unsigned int mutexes_lenght);

// ThreadCv ***************************************************************

thread_cv_t* thread_cv_create();

int thread_cv_wait(thread_cv_t* cv, thread_mutex_t* mutex);
int thread_cv_timedwait(thread_cv_t* cv);

int thread_cv_notify(thread_cv_t* cv);
int thread_cv_notifyall(thread_cv_t* cv);

void thread_cv_destroy(thread_cv_t* cv);

#endif /*THREAD_H_*/
