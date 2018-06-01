#ifndef __EQUEUE_DEBUG_H__
#define __EQUEUE_DEBUG_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
#include "def.h"
#define HAVE_FUNCTIONNAME

#define EQUEUE_INFO
#define EQUEUE_WARN
#define EQUEUE_ERROR

#ifdef HAVE_FUNCTIONNAME
#define EQUEUE_EXTRA_FMT "%s: "
#define EQUEUE_EXTRA_ARG , __FUNCTION__
#else
#define EQUEUE_EXTRA_FMT
#define EQUEUE_EXTRA_ARG
#endif


#ifdef EQUEUE_INFO
#define equeue_info(format, ...) MQUEUE_PRINT(EQUEUE_EXTRA_FMT format EQUEUE_EXTRA_ARG, ##__VA_ARGS__)
#else
#define equeue_info(...)
#endif

#ifdef EQUEUE_WARN
#define equeue_warn(format, ...) MQUEUE_PRINT(EQUEUE_EXTRA_FMT format EQUEUE_EXTRA_ARG, ##__VA_ARGS__)
#else
#define equeue_warn(...)
#endif

#ifdef EQUEUE_ERROR
#define equeue_error(format, ...) MQUEUE_PRINT(EQUEUE_EXTRA_FMT format EQUEUE_EXTRA_ARG, ##__VA_ARGS__)
#else
#define equeue_error(...)
#endif

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif