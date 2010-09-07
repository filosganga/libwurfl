#include <stdlib.h>
#include <assert.h>
#include <errno.h>
#include <time.h>

#ifdef __linux__
#include "thread-pthread-impl.h"

thread_mutex_t* thread_mutex_create()
{

	thread_mutex_t* mutex = malloc(sizeof(thread_mutex_t));

	if (mutex!=NULL)
	{

		pthread_mutexattr_t attr;
		pthread_mutexattr_init(&attr);

		pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE_NP);

		int res = pthread_mutex_init(&(mutex->handle), &attr);
		if (res!=0)
		{
			if(res==ENOMEM){
				error(0,MX_NOMEM,"Errore creazione mutex\n");
			}else if(res==EAGAIN){
				error(0,MX_MAX,"Errore creazione mutex\n");

			}else{
				error(0,MX_FAILED,"Errore creazione mutex\n");
			}

			free(mutex);
			mutex = NULL;
		}

	}

	return mutex;
}

int thread_mutex_destroy(thread_mutex_t *mutex)
{
	assert(mutex!=NULL);

	pthread_mutex_destroy(&(mutex->handle));
	free(mutex);
	mutex=NULL;

	return 0;
}

int thread_mutex_lock(thread_mutex_t *mutex)
{

	assert(mutex!=NULL);

	int res = pthread_mutex_lock(&mutex->handle);

	if (res==0)
	{
		res = MX_OK;
	}
	else
	{
		switch (res)
		{
		case EAGAIN:
			res = MX_MAXLOCK;
			break;
		default:
			res = MX_FAILED;
			break;
		}
	}

	return res;
}

int thread_mutex_unlock(thread_mutex_t *mutex)
{

	assert(mutex!=NULL);

	int res = pthread_mutex_unlock(&mutex->handle);
	if (res==0)
	{
		res = MX_OK;
	}
	else
	{
		switch (res)
		{
		case EPERM:
			res = MX_NOT_OWNED;
			break;
		default:
			res = MX_FAILED;
		}
	}

	return res;
}

int thread_mutex_timedlock(thread_mutex_t *mutex, unsigned long int timeout)
{
	assert(mutex!=NULL);

	enum mutex_errors exit_code;

	struct timespec time;
	time.tv_nsec = timeout;

	int res = pthread_mutex_timedlock(&mutex->handle, &time);

	if (res==0)
	{
		exit_code = MX_OK;
	}
	else
	{
		switch (res)
		{
		case ETIMEDOUT:
			exit_code = MX_TIMEOUT;
			break;
		default:
			exit_code = MX_FAILED;
		}
	}

	return exit_code;
}

int thread_mutex_try_lock(thread_mutex_t *mutex)
{
	assert(mutex!=NULL);

	enum mutex_errors exit_code;

	int res = pthread_mutex_trylock(&mutex->handle);

	if (res==0)
	{
		exit_code = MX_OK;
	}
	else
	{
		switch (res)
		{
		case EBUSY:
			exit_code = MX_BUSY;
			break;
		default:
			exit_code = MX_FAILED;
		}
	}

	return exit_code;
}

int thread_mutex_locks(thread_mutex_t* mutexes[], unsigned int mutexes_lenght)
{
	assert(mutexes!=NULL);
	int index=0;
	int exit_code=0;
	int res;
	int interrupted=0;

	do
	{
		res = pthread_mutex_lock(&mutexes[0]->handle);

		if (res==0)
		{
			for (index=1; index<mutexes_lenght && exit_code==0; index++)
			{
				res = pthread_mutex_trylock(&mutexes[index]->handle);

				if (res==EBUSY)
				{
					/* Un mutex non è disponibile rilascia tutti i mutex
					 * acquisiti compreso il primo */
					index--;

					do
					{
						assert(index>=0);
						pthread_mutex_unlock(&mutexes[index]->handle);
					} while (index>0);

					struct timespec sleep_time;
					sleep_time.tv_nsec = 100;

					errno=0;
					if (nanosleep(&sleep_time, NULL)==-1)
					{
						/* errno has value EINTR */
						interrupted = 1;
					}
				} //res == EBUSY
			} // end for
		} // res==0

	} while (res==EBUSY && !interrupted);

	if (!interrupted)
	{
		switch (res)
		{
		case EAGAIN:
			exit_code = MX_MAXLOCK;
			break;
		case EBUSY:
			/* interrotto il processo */
			break;
		default:
			exit_code = MX_FAILED;
		}
	}
	else
	{
		exit_code = MX_FAILED;
	}

	return exit_code;
}

#endif //def __linux__
