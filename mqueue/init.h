#ifndef __SYSTEM_INIT_H__
#define __SYSTEM_INIT_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "object.h"
#include "thread.h"

void app_init(int argc, char** argv);
int mqueue_init(int argc, char** argv);
void mqueue_loop(void);

extern struct object_information object_container[object_class_type_unknown];
#ifdef __cplusplus
}
#endif

#endif
