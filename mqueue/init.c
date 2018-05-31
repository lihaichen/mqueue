
#include "init.h"

extern struct object_information object_container[object_class_type_unknown];

/**
 * @brief 启动工作线程
 *
 * @return 成功返回0，失败返回-1
 */
static int system_threads(void) {
  int ret;
  sem_t wait;
  pthread_t tid;
  object_thread_t ot;

  ///<启动定时器线程
  // vlog("==> start thread '%s'\n", "timer");
  sem_init(&wait, 0, 0);
  ret = pthread_create(&tid, NULL, thread_timer_entry, &wait);
  if (ret != 0) {
    // vlog("==> create thread 'timer' error[%d]!\n", ret);
    return -1;
  }
  sem_wait(&wait);  ///<待定时器线程启动完毕后再启动其它线程
  sem_destroy(&wait);

  ///<启动应用线程
  OBJECT_FOREACH(object_class_type_thread, object_thread_t, ot)
  // vlog("==> start thread '%s'\n", ((object_t)ot)->name);
  ret = start_object_thread(ot);
  if (ret != 0) {
    // vlog("==> create thread '%s' error[%d]!\n", ((object_t)ot)->name, ret);
    return -1;
  }
  OBJECT_FOREACH_END

  return 0;
}

/**
 * @brief 初始化
 *
 * @return 成功返回0，失败返回失败的步骤
 */
int init(int argc, char** argv) {
  global_container_init();  ///<对象容器初始化
  app_init(argc, argv);

  if (system_threads() < 0)
    return -1;

  return 0;
}
