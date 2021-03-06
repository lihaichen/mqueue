
#include "message.h"
#include "thread.h"

/**
 * @brief 查找线程对象
 *
 * @param name 线程对象名称
 *
 * @return 查找到返回线程对象句柄，未查找到返回0
 */
HMOD find_thread(const char* name) {
  object_t p;

  p = object_find(name, object_class_type_thread);

  return (HMOD)p;
}

/**
 * @brief 发送消息-同步方式
 *
 * @param hmod 接受端线程句柄
 * @param message 消息
 * @param wparam 参数一
 * @param lparam 参数二
 *
 * @return 发送成功返回0，失败返回-1
 */
int send_message(HMOD hmod, int message, WPARAM wparam, LPARAM lparam) {
  int ret = -1;
  object_thread_t ot = hmod;
  if (ot == 0)
    return ret;
  thread_proc_t thread_proc = ((object_thread_t)hmod)->thread_proc;
  if (thread_proc == NULL)
    return ret;
  MQUEUE_SEM_WAIT(&ot->lock);
  ret = thread_proc(hmod, message, wparam, lparam);
  MQUEUE_SEM_POST(&ot->lock);
  return ret;
}

/**
 * @brief 投递消息-异步方式
 *
 * @param hmod 接收端线程句柄
 * @param message 消息
 * @param wparam 参数一
 * @param lparam 参数二
 *
 * @return 发送成功返回0，失败返回-1
 */
int post_message(HMOD hmod, int message, WPARAM wparam, LPARAM lparam) {
  object_thread_t p = NULL;
  msg_t pmsg = NULL;
  int rtn = 0, sem_value;

  p = (object_thread_t)hmod;
  if (p == NULL)
    return -1;

  MQUEUE_ENTER_LOCK(&p->msgqueue.lock);

  if ((p->msgqueue.write_pos + 1) % 16 == p->msgqueue.read_pos)  //已经写满了
  {
    rtn = -1;
    goto err;
  }

  pmsg = p->msgqueue.msg + p->msgqueue.write_pos;
  pmsg->hmod = hmod;
  pmsg->message = message;
  pmsg->wparam = wparam;
  pmsg->lparam = lparam;

  p->msgqueue.write_pos++;
  if (p->msgqueue.write_pos >= MSGQUEUE_MAX)
    p->msgqueue.write_pos = 0;

  p->msgqueue.dw_data |= QS_POSTMSG;

err:
  MQUEUE_EXIT_LOCK(&p->msgqueue.lock);

  MQUEUE_SEM_GET_VALUE(&p->msgqueue.wait, &sem_value);
  if (sem_value == -1)  ///<如果有线程在等待
    MQUEUE_SEM_POST(&p->msgqueue.wait);

  return rtn;
}

/**
 * @brief 获取投递消息
 *
 * @param hmod 线程句柄
 * @param pmsg 消息缓存
 *
 * @return 成功返回0，失败返回-1
 */
int get_message(HMOD hmod, msg_t pmsg) {
  object_thread_t p = NULL;

  p = (object_thread_t)hmod;
  if (p == NULL)
    return -1;

  memset(pmsg, 0, sizeof(struct msg));

  MQUEUE_ENTER_LOCK(&p->msgqueue.lock);

  if (p->msgqueue.dw_data & QS_POSTMSG) {
    if (p->msgqueue.read_pos != p->msgqueue.write_pos) {
      *pmsg = p->msgqueue.msg[p->msgqueue.read_pos];

      p->msgqueue.read_pos++;
      if (p->msgqueue.read_pos >= MSGQUEUE_MAX)
        p->msgqueue.read_pos = 0;

      MQUEUE_EXIT_LOCK(&p->msgqueue.lock);

      return 0;
    } else  ///<已读完
      p->msgqueue.dw_data &= ~QS_POSTMSG;
  }

  MQUEUE_EXIT_LOCK(&p->msgqueue.lock);

  ///< no message to read
  MQUEUE_SEM_WAIT(&p->msgqueue.wait);
  MQUEUE_SEM_SET_VALUE(&p->msgqueue.wait, 0);
  return 0;
}

/**
 * @brief 处理投递消息
 *
 * @param pmsg 消息
 *
 * @return 成功返回0，失败返回-1
 */
int dispatch_message(msg_t pmsg) {
  object_thread_t ot = NULL;
  thread_proc_t thread_proc = NULL;
  int ret = -1;
  if (pmsg == NULL || pmsg->hmod == 0)
    return ret;
  ot = (object_thread_t)pmsg->hmod;
  thread_proc = ot->thread_proc;
  if (thread_proc == NULL)
    return ret;
  MQUEUE_SEM_WAIT(&ot->lock);
  ret = thread_proc(pmsg->hmod, pmsg->message, pmsg->wparam, pmsg->lparam);
  MQUEUE_SEM_POST(&ot->lock);
  return ret;
}
