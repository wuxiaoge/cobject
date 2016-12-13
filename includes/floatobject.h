#ifndef FLOAT_OBJECT_H
#define FLOAT_OBJECT_H
#ifdef __cplusplus
extern "C" {
#endif

typedef struct _floatobject {
    Object_HEAD
    float ob_fval;
} FloatObject;

extern TypeObject Float_Type;

#define FloatObject_CHECK(ob) Object_CHECK(ob, &Float_Type)
#define FloatObject_CONVERT(ob) ((FloatObject *)(ob))
#define FloatObject_VALUE(ob) FloatObject_CONVERT(ob)->ob_fval
#define FloatObject_AsFLOAT(ob) FloatObject_VALUE(ob)

Object *FloatObject_FromFloat(float);

#ifdef __cplusplus
}
#endif
#endif
