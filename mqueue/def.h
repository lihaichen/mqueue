#ifndef __MQUEUE_DEF_H__
#define __MQUEUE_DEF_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
#include <pthread.h>
#include <stdio.h>

#define TRUE 1
#define FALSE 0

#define REMOVE -1
#define OFFLINE 0
#define ONLINE 1
#define CONNECTING 2

#define MQUEUE_USLEEP(us) usleep(us)

#define NOW time(NULL)

#define MQUEUE_LOCK_TYPE pthread_mutex_t
#define MQUEUE_INIT_LOCK(lock) pthread_mutex_init(lock, NULL)
#define MQUEUE_DEL_LOCK(lock) pthread_mutex_destroy(lock)
#define MQUEUE_ENTER_LOCK(lock) pthread_mutex_lock(lock)
#define MQUEUE_EXIT_LOCK(lock) pthread_mutex_unlock(lock)

#define MQUEUE_SEM_TYPE sem_t
#define MQUEUE_SEM_INIT(sem, p, v) sem_init(sem, p, v)
#define MQUEUE_SEM_WAIT(sem) sem_wait(sem)
#define MQUEUE_SME_DESTROY(sem) sem_destroy(sem)
#define MQUEUE_SEM_POST(sem) sem_post(sem)
#define MQUEUE_SEM_GET_VALUE(sem, value) sem_getvalue(sem, value)

#ifdef __cplusplus
}
#endif

#endif