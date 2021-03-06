#ifndef __MQUEUE_THREAD_H__
#define __MQUEUE_THREAD_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
#include "def.h"
#include "message.h"
#include "object.h"
#include "timer.h"

typedef int (*thread_proc_t)(HMOD, int, WPARAM, LPARAM);  ///<线程处理函数类型

/**
 * @brief 线程配置
 */
struct object_thread_config {
  int stack_size;
  int priority;
};

/**
 * @brief 线程模块类
 */
typedef struct object_thread {
  struct object parent;  ///<基类

  pthread_t tid;                              ///<线程ID
  void* (*entry)(void*);                      ///<线程入口
  thread_proc_t thread_proc;                  ///<线程处理函数
  struct object_thread_config thread_config;  ///<线程配置
  ULONG add_data;                             ///<附加参数
  struct object_information io_container;     ///<线程管理的IO容器

  struct msgqueue msgqueue;  ///<消息队列
  MQUEUE_SEM_TYPE lock;
} * object_thread_t;

void* thread_entry(void* parameter);
int thread_default_process(HMOD hmod,
                           int message,
                           WPARAM wparam,
                           LPARAM lparam);

object_thread_t new_object_thread(thread_proc_t thread_proc,
                                  struct object_thread_config* config);
int start_object_thread(object_thread_t ot);
void set_object_thread_add_data(object_thread_t ot, ULONG add_data);
ULONG get_object_thread_add_data(object_thread_t ot);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
