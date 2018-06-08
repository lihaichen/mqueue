#ifndef __MQUEUE_TIMER_H__
#define __MQUEUE_TIMER_H__
#include "message.h"
#include "object.h"

#define TIMER_STOP 0
#define TIMER_START 1

#define TIMER_ASYNC (0)
#define TIMER_SYNC (1)

#define TIMER_ONESHOT (0 << 1)
#define TIMER_PERIODIC (1 << 1)

#define TICK_PER_SECOND 10
#define ONE_SECOND TICK_PER_SECOND

#define TIMERID_DEFAULT -1

#define EQUEUE_MAX_TICK 0xFFFFFFFFFFFFFFFF
// 单位为ms
typedef unsigned long long equeue_tick;

#define EQUEUE_IS_TIMEOUT(current, timeout)                         \
  ((equeue_tick)((equeue_tick)(current) - (equeue_tick)(timeout)) < \
   EQUEUE_MAX_TICK / 2)

/**
 * @brief 定时器回调函数类型
 *
 * @param 参数
 *
 * @return NULL
 */
typedef void (*timer_func_t)(void* parameter);

/**
 * @brief 定时器类
 */
typedef struct object_timer {
  struct object parent;  ///<基类

  int mode;  ///<定时器种类

  HMOD hmod;  ///<定时器归属
  ULONG id;   ///<定时器编号

  equeue_tick timeout_tick;  ///<当前定时计数
  equeue_tick init_tick;     ///<定时器超时时间

  int run;   ///< 0 pause, 1 run
  int type;  ///< 0 async, 1 sync
  void* user_data;
} * object_timer_t;

void timer_add(HMOD hmod, ULONG id, int init_tick, void* user_data, int type);
void timer_remove(HMOD hmod, ULONG id);
void timer_start(HMOD hmod, ULONG id);
void timer_stop(HMOD hmod, ULONG id);
void timer_control(HMOD hmod, ULONG id, int init_tick);

void* thread_timer_entry(void* parameter);

#endif
