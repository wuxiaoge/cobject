#ifndef STDOUT_OBJECT_H
#define STDOUT_OBJECT_H
#ifdef __cplusplus
extern "C" {
#endif

typedef struct _stdoutobject {
    Object_HEAD
} StdoutObject;

extern TypeObject Stdout_Type;

#define StdoutObject_CHECK(ob) Object_CHECK(ob, &Stdout_Type)
#define StdoutObject_CONVERT(ob) ((StdoutObject *)(ob))

Object *StdoutObject_New();

#ifdef __cplusplus
}
#endif
#endif
