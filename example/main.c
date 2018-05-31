#include "../mqueue/mqueue.h"

static int thread_proc(HMOD hmod, int message, WPARAM wparam, LPARAM lparam) {
  switch (message) {
    case MSG_INIT: {
      equeue_info("MSG_INIT\n");
      timer_add(hmod, 1, 1000, NULL, TIMER_ASYNC);
      timer_start(hmod, 1);
    }

    break;
    case MSG_TIMER: {
      equeue_info("MSG_TIMER %lu %lu\n", wparam, lparam);
    } break;
    case MSG_COMMAND: {
      equeue_info("MSG_COMMAND\n");
    } break;
  }

  return thread_default_process(hmod, message, wparam, lparam);
}

/**
 * @brief register thread module
 *
 * @return always 0
 */
int register_task_test1(void) {
  object_thread_t ot;

  ot = new_object_thread(thread_proc);
  assert(ot);

  object_addend(&ot->parent, "test1", object_class_type_thread);

  return 0;
}

void app_init(int argc, char** argv) {
  register_task_test1();
  return;
}

int main(int argc, char** argv) {
  mqueue_init(argc, argv);
  mqueue_loop();
  return 0;
}
