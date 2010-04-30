/*
 * process.h
 *
 *  Created on: 7-mag-2009
 *      Author: filosganga
 */

#ifndef PROCESS_H_
#define PROCESS_H_

typedef struct _process_t process_t;

typedef int (*process_function_f)(void* process_args);

process_t* process_create(process_function_f process_function, void* process_function_args);

int process_wait(process_t* process);

int process_kill(process_t* process, int sig);

#endif /* PROCESS_H_ */
