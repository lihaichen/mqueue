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

#define MQUEUE_SLEEP(us) usleep(us)

#define NOW time(NULL)

#define INIT_LOCK(lock) pthread_mutex_init(lock, NULL)
#define DEL_LOCK(lock) pthread_mutex_destroy(lock)
#define ENTER_LOCK(lock) pthread_mutex_lock(lock)
#define EXIT_LOCK(lock) pthread_mutex_unlock(lock)

#ifdef __cplusplus
}
#endif

#endif