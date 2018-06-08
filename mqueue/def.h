#ifndef __MQUEUE_DEF_H__
#define __MQUEUE_DEF_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
#include <assert.h>
#include <poll.h>
#include <pthread.h>
#include <semaphore.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

#define TRUE 1
#define FALSE 0

#define REMOVE -1
#define OFFLINE 0
#define ONLINE 1
#define CONNECTING 2

#define MQUEUE_MALLOC(size) malloc(size)
#define MQUEUE_CALLOC(n, size) calloc(n, size)
#define MQUEUE_FREE(p) free(p)

#define MQUEUE_MSLEEP(ms) usleep((ms)*1000)

#define NOW time(NULL)

#define EQUEUE_GET_TICK(ms)                                                  \
  do {                                                                       \
    struct timespec spec;                                                    \
    clock_gettime(CLOCK_REALTIME, &spec);                                    \
    *ms = (spec.tv_nsec / 1000000 + (unsigned long long)spec.tv_sec * 1000); \
  } while (0)

#define MQUEUE_PRINT(...) printf(__VA_ARGS__)

#define MQUEUE_LOCK_TYPE pthread_mutex_t
#define MQUEUE_INIT_LOCK(lock) pthread_mutex_init(lock, NULL)
#define MQUEUE_DEL_LOCK(lock) pthread_mutex_destroy(lock)
#define MQUEUE_ENTER_LOCK(lock) pthread_mutex_lock(lock)
#define MQUEUE_EXIT_LOCK(lock) pthread_mutex_unlock(lock)

#define MQUEUE_SEM_TYPE sem_t
#define MQUEUE_SEM_INIT(sem, p, v) sem_init(sem, p, v)
#define MQUEUE_SEM_WAIT(sem) sem_wait(sem)
#define MQUEUE_SEM_WAIT_TIME(sem, ts) sem_timedwait(sem, ts)
#define MQUEUE_SME_DESTROY(sem) sem_destroy(sem)
#define MQUEUE_SEM_POST(sem) sem_post(sem)
#define MQUEUE_SEM_GET_VALUE(sem, value) sem_getvalue(sem, value)

#define MQUEUE_ASSERT(EX)                                    \
  do {                                                       \
    if (!(EX)) {                                             \
      MQUEUE_PRINT("%s,%s,%d", #EX, __FUNCTION__, __LINE__); \
    }                                                        \
  } while (0)

#ifdef __cplusplus
}
#endif

#endif