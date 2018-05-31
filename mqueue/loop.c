#include "def.h"
#include "mqueue.h"
/**
 * @brief 轮询
 * 这里监听来自外界的事件
 */
void mqueue_loop(void) {
  for (;;)
    MQUEUE_SLEEP(1000 * 1000);
}
