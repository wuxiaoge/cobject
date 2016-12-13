#ifndef LONG_OBJECT_H
#define LONG_OBJECT_H
#ifdef __cplusplus
extern "C" {
#endif

typedef struct _longobject {
    Object_HEAD
    long ob_lval;
} LongObject;

extern TypeObject Long_Type;

#define LongObject_CHECK(ob) Object_CHECK(ob, &Long_Type)
#define LongObject_CONVERT(ob) ((LongObject *)(ob))
#define LongObject_VALUE(ob) LongObject_CONVERT(ob)->ob_lval
#define LongObject_AsLONG(ob) LongObject_VALUE(ob)

Object *LongObject_FromLong(long);

#ifdef __cplusplus
}
#endif
#endif
