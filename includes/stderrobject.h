#ifndef STDERR_OBJECT_H
#define STDERR_OBJECT_H
#ifdef __cplusplus
extern "C" {
#endif

typedef struct _stderrobject {
    Object_HEAD
} StderrObject;

extern TypeObject Stderr_Type;

#define StderrObject_CHECK(ob) Object_CHECK(ob, &Stderr_Type)
#define StderrObject_CONVERT(ob) ((StderrObject *)(ob))

Object *StderrObject_New();

#ifdef __cplusplus
}
#endif
#endif
