
#include "thread.h"

int thread_default_process(HMOD hmod,
                           int message,
                           WPARAM wparam,
                           LPARAM lparam) {
  return 0;
}

void* thread_entry(void* parameter) {
  struct msg msg;
  HMOD hmod;

  hmod = (HMOD)parameter;

  while (!get_message(hmod, &msg))
    dispatch_message(&msg);

  return NULL;
}

object_thread_t new_object_thread(thread_proc_t thread_proc,
                                  struct object_thread_config* config) {
  object_thread_t ot;

  ot = (object_thread_t)MQUEUE_CALLOC(1, sizeof(struct object_thread));
  if (ot == NULL)
    return NULL;
  memset(&ot->thread_config, 0, sizeof(struct object_thread_config));
  if (config != NULL) {
    memcpy(&ot->thread_config, config, sizeof(struct object_thread_config));
  }
  ot->thread_proc = thread_proc;
  ot->entry = thread_entry;
  MQUEUE_INIT_LOCK(&ot->msgqueue.lock);
  MQUEUE_SEM_INIT(&ot->msgqueue.wait, 0, 1);
  MQUEUE_SEM_INIT(&ot->lock,0,1);
  ////////////
  object_container_init(&ot->io_container);

  return ot;
}

int start_object_thread(object_thread_t ot) {
  send_message((HMOD)ot, MSG_INIT, 0, 0);
  if (ot->thread_config.stack_size < 1) {
    return pthread_create(&ot->tid, NULL, ot->entry, (void*)ot);
  } else {
    pthread_attr_t attr = {0};
    struct sched_param sparam;
    pthread_attr_init(&attr);
    sparam.sched_priority = ot->thread_config.priority;
    pthread_attr_setschedparam(&attr, &sparam);
    pthread_attr_setstacksize(&attr, ot->thread_config.stack_size);
    return pthread_create(&ot->tid, &attr, ot->entry, (void*)ot);
  }
}

void set_object_thread_add_data(object_thread_t ot, ULONG add_data) {
  ot->add_data = add_data;
}

ULONG get_object_thread_add_data(object_thread_t ot) {
  return ot->add_data;
}
