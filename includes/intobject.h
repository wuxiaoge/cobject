#ifndef INT_OBJECT_H
#define INT_OBJECT_H
#ifdef __cplusplus
extern "C" {
#endif

typedef struct _intobject {
    Object_HEAD
    int ob_ival;
} IntObject;

#define IntObject_CONVERT(ob) ((IntObject *)(ob))
#define IntObject_VALUE(ob) IntObject_CONVERT(ob)->ob_ival
#define IntObject_AsINT(ob) IntObject_VALUE(ob)

Object *IntObject_FromInt(int);

#ifdef __cplusplus
}
#endif
#endif
