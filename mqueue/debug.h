#ifndef __EQUEUE_DEBUG_H__
#define __EQUEUE_DEBUG_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
#include "def.h"
#define HAVE_FUNCTIONNAME

#define MQUEUE_INFO
#define MQUEUE_WARN
#define MQUEUE_ERROR

#ifdef HAVE_FUNCTIONNAME
#define MQUEUE_EXTRA_FMT "%s: "
#define MQUEUE_EXTRA_ARG , __FUNCTION__
#else
#define MQUEUE_EXTRA_FMT
#define MQUEUE_EXTRA_ARG
#endif


#ifdef MQUEUE_INFO
#define mqueue_info(format, ...) MQUEUE_PRINT(MQUEUE_EXTRA_FMT format MQUEUE_EXTRA_ARG, ##__VA_ARGS__)
#else
#define mqueue_info(...)
#endif

#ifdef MQUEUE_WARN
#define mqueue_warn(format, ...) MQUEUE_PRINT(MQUEUE_EXTRA_FMT format MQUEUE_EXTRA_ARG, ##__VA_ARGS__)
#else
#define mqueue_warn(...)
#endif

#ifdef MQUEUE_ERROR
#define mqueue_error(format, ...) MQUEUE_PRINT(MQUEUE_EXTRA_FMT format MQUEUE_EXTRA_ARG, ##__VA_ARGS__)
#else
#define mqueue_error(...)
#endif

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif