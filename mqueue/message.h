
#ifndef __MQUEUE_MESSAGE_H__
#define __MQUEUE_MESSAGE_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "def.h"
#include "object.h"
#include "types.h"

#define MSG_TERM 0x0000

/*
 * brief Ready to initialize modules
 */
#define MSG_INIT 0x0001

/*
 * brief Indicates a timer has expired
 */
#define MSG_TIMER 0x0002

/*
 * brief The command message
 *
 * \cmd
 * MSG_COMMAND
 * int cmd = wparam;
 */
#define MSG_COMMAND 0x0003

/*
 * brief Indicates a message has post
 */
#define QS_POSTMSG 0x4000

/*
 * @brief 常用命令类型（IO收到消息）
 */
#define MSG_COMMAND_POLLIN 0x01

/*
 * @brief 用户命令开头
 */
#define MSG_COMMAND_USER 0x100

/**
 * @brief 线程间消息结构
 */
typedef struct msg {
  HMOD hmod;

  int message;

  WPARAM wparam;
  LPARAM lparam;
} * msg_t;

/**
 * @brief 消息队列（环形）
 */
typedef struct msgqueue {
  UINT dw_data;

  int read_pos;
  int write_pos;

  MQUEUE_SEM_TYPE wait;
  MQUEUE_LOCK_TYPE lock;
  struct msg msg[MSGQUEUE_MAX];
} * msgqueue_t;

////////////////////////////////////////////////////////////////////////
HMOD find_thread(const char* name);
int send_message(HMOD hmod, int message, WPARAM wparam, LPARAM lparam);
int post_message(HMOD hmod, int message, WPARAM wparam, LPARAM lparam);
int get_message(HMOD hmod, msg_t pmsg);
int dispatch_message(msg_t pmsg);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
