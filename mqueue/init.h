#ifndef __SYSTEM_INIT_H__
#define __SYSTEM_INIT_H__

#include "object.h"
#include "thread.h"

#include <semaphore.h>
#include <signal.h>

void app_init(int argc, char** argv);
int init(int argc, char** argv);
void loop(void);

struct object_information object_container[object_class_type_unknown];

#endif
