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
