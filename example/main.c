#include "../mqueue/mqueue.h"

static int thread_proc(HMOD hmod, int message, WPARAM wparam, LPARAM lparam) {
  switch (message) {
    case MSG_INIT: {
      mqueue_info("MSG_INIT\n");
      timer_add(hmod, 1, 1000, NULL, TIMER_ASYNC | TIMER_PERIODIC);
      timer_start(hmod, 1);

      timer_add(hmod, 2, 2000, NULL, TIMER_ASYNC | TIMER_PERIODIC);
      timer_start(hmod, 2);
    }

    break;
    case MSG_TIMER: {
      equeue_tick recv_time;
      EQUEUE_GET_TICK(&recv_time);
      mqueue_info("MSG_TIMER %d %d %llu\n", wparam, lparam, recv_time);
      if(wparam == 2){
        // timer_add(hmod, 2, 2000, NULL, TIMER_SYNC);
        // timer_start(hmod, 2);
      }
    } break;
    case MSG_COMMAND: {
      mqueue_info("MSG_COMMAND\n");
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
  struct object_thread_config config;
  config.stack_size = 2048;
  config.priority = 20;
  ot = new_object_thread(thread_proc, NULL);
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
