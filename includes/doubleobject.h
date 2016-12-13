#ifndef DOUBLE_OBJECT_H
#define DOUBLE_OBJECT_H
#ifdef __cplusplus
extern "C" {
#endif

typedef struct _doubleobject {
    Object_HEAD
    double ob_dval;
} DoubleObject;

extern TypeObject Double_Type;

#define DoubleObject_CONVERT(ob) ((DoubleObject *)(ob))
#define DoubleObject_VALUE(ob) DoubleObject_CONVERT(ob)->ob_dval
#define DoubleObject_AsDOUBLE(ob) DoubleObject_VALUE(ob)

Object *DoubleObject_FromDouble(double);

#ifdef __cplusplus
}
#endif
#endif
