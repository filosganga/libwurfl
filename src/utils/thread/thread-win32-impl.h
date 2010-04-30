#ifndef THREADWIN32IMPL_H_
#define THREADWIN32IMPL_H_

#include "thread.h"

#ifdef _WIN32

#include <windows.h>

struct _tread_t {
	DWORD id;
	HANDLE handle;
};

struct _thread_mutex_t {
	HANDLE handle;
};

struct _thread_cv_t
{
	// Number of waiting threads.
	int waiters_count;

	// Serialize access to <waiters_count_>.
	CRITICAL_SECTION waiters_count_lock;

	/* Semaphore used to queue up threads waiting for the condition to
	 * become signaled. */
	HANDLE sem;

	/* An auto-reset event used by the broadcast/signal thread to wait
	 * for all the waiting thread(s) to wake up and be released from
	 * the semaphore. */
	HANDLE waiters_done;

	/* Keeps track of whether we were broadcasting or signaling.  This
	 * allows us to optimize the code if we're just signaling. */
	size_t was_broadcast;
}

#endif

#endif /*THREADWIN32IMPL_H_*/
