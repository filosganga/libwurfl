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
 * along with libWURFL.  If not, see <http://www.gnu.org/licenses/>.
 */
 
/* Written by Filippo De Luca <me@filippodeluca.com>.  */

#include "thread-win32-impl.h"

#include <stdlib.h>
#include <assert.h>

#ifdef _WIN32
#include <windows.h>

thread_mutex_t* thread_mutex_create() {
	thread_mutex_t* mutex = malloc(sizeof(mutex));

	if (mutex!=NULL) {

		/* No name, no Security_attribute */
		mutex->handle = CreateMutex(NULL,FALSE,NULL);

		if (mutex->handle==NULL) {
			free(mutex);
		}

	}

	return mutex;
}

int thread_mutex_destroy(thread_mutex_t *mutex) {
	assert(mutex!=NULL);

	/* Don't check for errors */
	CloseHandle(mutex->handle);

	free(mutex);

	return 0;
}

int thread_mutex_lock(thread_mutex_t *mutex) {

	assert(mutex!=NULL);

	DWORD res = WaitForSingleObject(mutex->handle, INFINITE);

	if (res == WAIT_OBJECT_0) {
		return MX_OK;
	} else {
		assert(res == WAIT_ABANDONED);

		return MX_ABANDONED;
	}
}

int thread_mutex_unlock(thread_mutex_t *mutex) {
	assert(mutex!=NULL);

	BOOL res = ReleaseMutex(mutex->handle);

	if (!res) {
		return MX_NOT_OWNED;
	}

	return MX_OK;
}

int thread_mutex_try_lock(thread_mutex_t *mutex) {
	DWORD res = WaitForSingleObject(mutex->handle, 0);

	if (res==WAIT_TIMEOUT) {
		return MX_TIMEOUT;
	} else if (res==WAIT_ABANDONED) {
		return MX_ABANDONED;
	} else {
		assert(res == WAIT_OBJECT_0);

		return MX_OK;
	}

}

int thread_mutex_timedlock(thread_mutex_t *mutex, unsigned long int timeout) {

	DWORD res = WaitForSingleObject(mutex->handle, timeout);

	if (res==WAIT_TIMEOUT) {
		return MX_TIMEOUT;
	} else if (res==WAIT_ABANDONED) {
		return MX_ABANDONED;
	} else {
		assert(res == WAIT_OBJECT_0);

		return MX_OK;
	}

}

int thread_mutex_locks(thread_mutex_t* mutexes[], unsigned int mutexes_lenght) {

	assert(mutexes!=NULL);

	// WaitForMultipleObject accept max 64 handles
	assert(mutexes_lenght<=64);

	int index=0;
	int exit_code=0;
	int res;

	HANDLE* handles;

	handles = malloc(sizeof(HANDLE)*mutexes_lenght);
	if (handles==NULL) {
		perror("handles nullo\n");
		exit(-1);
	}

	for (index=0; index<mutexes_lenght; index++) {
		handles[index] = &mutexes[index]->handle;
	}

	res = WaitForMultipleObjects(mutexes_lenght, handles, TRUE, INFINITE);

	if (res == WAIT_OBJECT_0) {
		exit_code = MX_OK;
	} else if (res == WAIT_ABANDONED) {
		exit_code = MX_ABANDONED;
	} else {
		assert(res==WAIT_FAILED);
		exit_code = MX_FAILED;
	}

	return exit_code;
}

#endif // def _WIN32
