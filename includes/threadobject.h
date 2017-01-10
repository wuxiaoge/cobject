#include <pthread.h>

#ifndef THREAD_OBJECT_H
#define THREAD_OBJECT_H
#ifdef __cplusplus
extern "C" {
#endif

typedef struct _threadobject {
    Object_HEAD
    pthread_t ob_tid;
} ThreadObject;

extern TypeObject Thread_Type;

#define ThreadObject_CHECK(ob) Object_CHECK(ob, &Thread_Type)
#define ThreadObject_CONVERT(ob) ((ThreadObject *)(ob))
#define ThreadObject_TID(ob) ThreadObject_CONVERT(ob)->ob_tid

Object *ThreadObject_New(Object *);

#ifdef __cplusplus
}
#endif
#endif
