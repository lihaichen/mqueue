
#include "timer.h"
#include "debug.h"
#include "def.h"

#define TIMER_SORT

extern struct object_information object_container[object_class_type_unknown];

/**
 * @brief 定时器插入
 *
 * @param parameter 定时器
 *
 * @return 不会返回
 */
#ifdef TIMER_SORT
static void timer_insert_bytime(object_timer_t timer);
#endif

#ifndef TIMER_SORT
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

  pt = (object_timer_t)MQUEUE_CALLOC(1, sizeof(struct object_timer));
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
#endif

#ifdef TIMER_SORT
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

  pt = (object_timer_t)MQUEUE_CALLOC(1, sizeof(struct object_timer));
  MQUEUE_ASSERT(pt);

  pt->hmod = hmod;
  pt->id = id;
  pt->init_tick = init_tick;
  pt->run = TIMER_STOP;
  pt->type = type;
  pt->user_data = user_data;

  memset(name, 0, OBJ_NAME_MAX);
  sprintf(name, "%016lX:%016lX", hmod, id);
  strncpy(pt->parent.name, name, OBJ_NAME_MAX);
  timer_insert_bytime(pt);
}
#endif

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

  MQUEUE_FREE(po);
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
static equeue_tick __timer_counter = 0;

/**
 * @brief 定时器线程入口
 *
 * @param parameter 线程参数（互斥量）
 *
 * @return 不会返回
 */
#ifndef TIMER_SORT
void* thread_timer_entry(void* parameter) {
  equeue_tick old_timer_counter = 0;
  object_timer_t pt;
  MQUEUE_SEM_TYPE* wait = (MQUEUE_SEM_TYPE*)parameter;
  MQUEUE_SEM_POST(wait);

  for (;;) {
    EQUEUE_GET_TICK(&__timer_counter);
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
    MQUEUE_MSLEEP(2);
  }

  return NULL;
}
#else
void* thread_timer_entry(void* parameter) {
  object_timer_t pt = NULL;
  equeue_tick current_time = 0;
  MQUEUE_SEM_TYPE* wait = (MQUEUE_SEM_TYPE*)parameter;
  struct timespec spec;
  MQUEUE_SEM_POST(wait);
  for (;;) {
    pt = NULL;
    // 查找最近的定时器
    EQUEUE_GET_TICK(&current_time);
    MQUEUE_ENTER_LOCK(&object_container[object_class_type_timer].lock);
    OBJECT_FOREACH(object_class_type_timer, object_timer_t, pt)
    if (pt->run != TIMER_STOP)
      break;
    OBJECT_FOREACH_END
    MQUEUE_EXIT_LOCK(&object_container[object_class_type_timer].lock);
    if (pt) {
      if (EQUEUE_IS_TIMEOUT(current_time, pt->timeout_tick)) {
        // 移除定时器
        MQUEUE_ENTER_LOCK(&object_container[object_class_type_timer].lock);
        list_remove(&pt->parent.list);
        MQUEUE_EXIT_LOCK(&object_container[object_class_type_timer].lock);
        // 重新添加定时器
        timer_insert_bytime(pt);
        if (pt->type == TIMER_ASYNC)
          post_message(pt->hmod, MSG_TIMER, (WPARAM)pt->id,
                       (LPARAM)pt->user_data);
        else if (pt->type == TIMER_SYNC)
          send_message(pt->hmod, MSG_TIMER, (WPARAM)pt->id,
                       (LPARAM)pt->user_data);
      } else {
        int diff = pt->timeout_tick - current_time - 2;
        if (diff < 1)
          diff = 1;
        MQUEUE_MSLEEP(diff);
      }
    } else {
      MQUEUE_MSLEEP(100);
    }
  }

  return NULL;
}
#endif

/**
 * @brief 定时器插入
 *
 * @param parameter 定时器
 *
 * @return 不会返回
 */
#ifdef TIMER_SORT
static void timer_insert_bytime(object_timer_t timer) {
  object_timer_t pt = NULL;
  struct timespec spec;
  MQUEUE_ASSERT(timer);
  EQUEUE_GET_TICK(&timer->timeout_tick);
  timer->timeout_tick += timer->init_tick;
  MQUEUE_ENTER_LOCK(&object_container[object_class_type_timer].lock);
  OBJECT_FOREACH(object_class_type_timer, object_timer_t, pt)
  if (pt->run == TIMER_STOP)
    continue;
  if (pt->timeout_tick - timer->timeout_tick == 0)
    continue;
  else if (pt->timeout_tick - timer->timeout_tick < (0xFFFFFFFF / 2))
    break;
  OBJECT_FOREACH_END
  if (pt == NULL) {
    // 第一次添加定时器
    list_insert_after(&object_container[object_class_type_timer].list,
                      &timer->parent.list);
  } else {
    list_insert_after(&pt->parent.list, &timer->parent.list);
  }
  MQUEUE_EXIT_LOCK(&object_container[object_class_type_timer].lock);
  return;
}
#endif
