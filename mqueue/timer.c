
#include "timer.h"

extern struct object_information object_container[object_class_type_unknown];

/**
 * @brief 定时器-添加定时器
 *
 * @param hmod 安装定时器的线程句柄
 * @param id 定时器ID
 * @param init_tick 定时器初始计数
 */
void timer_add(HMOD hmod, ULONG id, int init_tick, void* user_data, int type) {
  char name[OBJ_NAME_MAX];
  object_timer_t pt = NULL;

  pt = (object_timer_t)calloc(1, sizeof(struct object_timer));
  assert(pt);

  pt->hmod = hmod;
  pt->id = id;
  pt->init_tick = init_tick;
  pt->run = TIMER_STOP;
  pt->type = type;
  pt->user_data = user_data;

  memset(name, 0, OBJ_NAME_MAX);
  sprintf(name, "%016lX:%016lX", hmod, id);

  object_addend(&pt->parent, name, object_class_type_timer);
}

/**
 * @brief 定时器-移除定时器
 *
 * @param hmod 安装定时器的线程句柄
 * @param id 定时器ID
 */
void timer_remove(HMOD hmod, ULONG id) {
  char name[OBJ_NAME_MAX];
  object_t po;

  memset(name, 0, OBJ_NAME_MAX);
  sprintf(name, "%016lX:%016lX", hmod, id);

  po = object_find(name, object_class_type_timer);
  if (po == NULL)
    return;

  object_delete(po);

  free(po);
}

/**
 * @brief 定时器-启动定时器
 *
 * @param hmod 安装定时器的线程句柄
 * @param id 定时器ID
 */
void timer_start(HMOD hmod, ULONG id) {
  char name[OBJ_NAME_MAX];
  object_t p;
  object_timer_t pt;

  memset(name, 0, OBJ_NAME_MAX);
  sprintf(name, "%016lX:%016lX", hmod, id);

  p = object_find(name, object_class_type_timer);
  if (p == NULL) {
    return;
  }

  pt = (object_timer_t)p;
  pt->run = TIMER_START;
}

/**
 * @brief 定时器-停止定时器
 *
 * @param hmod 安装定时器的线程句柄
 * @param id 定时器ID
 */
void timer_stop(HMOD hmod, ULONG id) {
  char name[OBJ_NAME_MAX];
  object_t p;
  object_timer_t pt;

  memset(name, 0, OBJ_NAME_MAX);
  sprintf(name, "%016lX:%016lX", hmod, id);

  p = object_find(name, object_class_type_timer);
  if (p == NULL) {
    return;
  }

  pt = (object_timer_t)p;
  pt->run = TIMER_STOP;
}

/**
 * @brief 定时器-控制定时器
 *
 * @param hmod 安装定时器的线程句柄
 * @param id 定时器ID
 * @param init_tick 定时器初始计数
 */
void timer_control(HMOD hmod, ULONG id, int init_tick) {
  char name[OBJ_NAME_MAX];
  object_t p;
  object_timer_t pt;

  memset(name, 0, OBJ_NAME_MAX);
  sprintf(name, "%016lX:%016lX", hmod, id);  ///<定时器命名方式：地址+ID

  p = object_find(name, object_class_type_timer);
  if (p == NULL)
    return;

  pt = (object_timer_t)p;

  pt->run = TIMER_STOP;

  pt->timeout_tick = 0;
  pt->init_tick = init_tick;

  pt->run = TIMER_START;
}

/**
 * @brief 计数器
 */
static unsigned long long __timer_counter = 0;

/**
 * @brief 定时器线程入口
 *
 * @param parameter 线程参数（互斥量）
 *
 * @return 不会返回
 */
void* thread_timer_entry(void* parameter) {
  unsigned long long old_timer_counter = 0;
  object_timer_t pt;
  MQUEUE_SEM_TYPE* wait = (MQUEUE_SEM_TYPE*)parameter;
  struct timespec spec;
  MQUEUE_SEM_POST(wait);

  for (;;) {
    clock_gettime(CLOCK_REALTIME, &spec);
    __timer_counter = (long)spec.tv_nsec / 1000000 + spec.tv_sec * 1000;
    MQUEUE_ENTER_LOCK(&object_container[object_class_type_timer].lock);

    OBJECT_FOREACH(object_class_type_timer, object_timer_t, pt)
    if (pt->run == TIMER_STOP)
      continue;

    pt->timeout_tick += (__timer_counter - old_timer_counter);
    if (pt->timeout_tick < 0)
      pt->timeout_tick = 0;

    if (pt->timeout_tick < pt->init_tick)
      continue;

    if (pt->type == TIMER_ASYNC)
      post_message(pt->hmod, MSG_TIMER, (WPARAM)pt->id, (LPARAM)pt->user_data);
    else if (pt->type == TIMER_SYNC)
      send_message(pt->hmod, MSG_TIMER, (WPARAM)pt->id, (LPARAM)pt->user_data);

    pt->timeout_tick = 0;
    OBJECT_FOREACH_END

    MQUEUE_EXIT_LOCK(&object_container[object_class_type_timer].lock);

    old_timer_counter = __timer_counter;
    MQUEUE_USLEEP(2000);
  }

  return NULL;
}
