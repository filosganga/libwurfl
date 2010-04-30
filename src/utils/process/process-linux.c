/*
 * process-linux.c
 *
 *  Created on: 7-mag-2009
 *      Author: filosganga
 */
#ifdef __linux__

#include "process.h"

#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/wait.h>

#include <gnulib/error.h>

struct _process_t {
	pid_t pid;
};

process_t* process_create(process_function_f process_function, void* process_function_args) {

	process_t* process = NULL;

	pid_t child_pid = fork();
	if(child_pid == -1) {
		// Error
		error(-1, errno, "error creating process");
	}
	else if(child_pid == 0){
		// Child
		process_function(process_function_args);
	}
	else {
		// Parent
		process = malloc(sizeof(process_t));
		memset(process, 0, sizeof(process_t));

		process->pid = child_pid;
	}

	return process;
}

int process_wait(process_t* process) {

	int status = 0;
	pid_t pid = waitpid(process->pid, &status, 0);
	if(pid==-1) {
		 int error_no = errno;
		 error(-1, errno, "error executing waitpid(%d)", process->pid);
	}

	//TODO: is it always defined?
	int exit_status = WEXITSTATUS(status);

	return exit_status;
}

int process_kill(process_t* process, int sig) {

	kill(process->pid, sig);
}

#endif
