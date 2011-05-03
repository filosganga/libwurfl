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

#include <assert.h>
#include <stdio.h>

#ifdef _WIN32
#include <windows.h>

typedef struct {
	int (*thread_function)(void*);
	void* thread_function_args;
} win32_thread_function_p;

static DWORD win32_thread_function(LPVOID param)
{
	int exit_code;

	win32_thread_function_p* win32_thread_function_param =
	(win32_thread_function_p*)param;

	int (*function)(void*) = win32_thread_function_param->thread_function;
	void* args = win32_thread_function_param->thread_function_args;

	printf("%s - %d: args = %p\n",__FILE__,__LINE__, args);

	exit_code = function(args);

	printf("%s - %d: exit_code = %d\n",__FILE__,__LINE__, exit_code);

	HANDLE heap_handle = GetProcessHeap();
	HeapFree(heap_handle,0,param);

	return (DWORD)exit_code;
}

thread_t* thread_create(thread_function_f thread_function, void* thread_function_args)
{

	assert(thread_function != NULL);
	assert(thread_function_args != NULL);

	// allocate memory to thread
	HANDLE heap_handle = GetProcessHeap();
	thread_t* thread = HeapAlloc(heap_handle,0,sizeof(thread));

	if(thread!=NULL) {

		win32_thread_function_p* param = HeapAlloc(heap_handle,0,sizeof(win32_thread_function_p));

		// TODO verificare param

		param->thread_function_args = thread_function_args;
		param->thread_function = thread_function;

		thread->handle = CreateThread(NULL,0, win32_thread_function, param, 0, &thread->id);
		if(thread->handle==NULL) {

			// free thread and ignore errors
			HeapFree(heap_handle,0,thread);
		}

	} // thread!=NULL

	return thread;
}

int thread_join(thread_t* thread) {

	assert(thread!=NULL);

	DWORD wait_code;
	DWORD exit_code;

	wait_code = WaitForSingleObject(thread->handle,INFINITE);
	if(wait_code == WAIT_OBJECT_0) {
		GetExitCodeThread(thread->handle,&exit_code);
		CloseHandle(thread->handle);
	}

	return (int)exit_code;
}

thread_t* thread_whoami(void)
{

	HANDLE heap_handle = GetProcessHeap();
	thread_t* thread = HeapAlloc(heap_handle,0,sizeof(thread));

	if(thread!=NULL) {
		thread->handle = GetCurrentThread();
		thread->id = GetCurrentThreadId();
	}

	return thread;
}

// Win32 implementation ****************************************

int thread_id(thread_t* thread)
{
	assert(thread!=NULL);

	return (int)thread->id;
}

#endif
