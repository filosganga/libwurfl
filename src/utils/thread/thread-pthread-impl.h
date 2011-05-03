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
