#ifndef STDIN_OBJECT_H
#define STDIN_OBJECT_H
#ifdef __cplusplus
extern "C" {
#endif

typedef struct _stdinobject {
    Object_HEAD
} StdinObject;

extern TypeObject Stdin_Type;

#define StdinObject_CHECK(ob) Object_CHECK(ob, &Stdin_Type)
#define StdinObject_CONVERT(ob) ((StdinObject *)(ob))

Object *StdinObject_New();

#ifdef __cplusplus
}
#endif
#endif
