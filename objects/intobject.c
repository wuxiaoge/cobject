#include "object.h"

static Object *int_method_add(Object *self, Object *ob) {
    return IntObject_FromInt(IntObject_AsINT(self) + IntObject_AsINT(ob));
}

MethodDef int_methods[] = {
    {"Add_Int", int_method_add},
    {Object_NULL, Object_NULL}
};

static int int_init(Object *self, Object *args) {
    Object_Extend(self, &Object_Type, sizeof(Object));
    Object_Init(Object_BASE(self), Object_NULL); 
    int _i = *(int *)args;
    IntObject_VALUE(self) = _i;
    return 0;
}

static int int_deinit(Object *self) {
    return 0;
}

static long int_hash(Object *self) {
    return IntObject_VALUE(self);
}

static Object *int_str(Object *self) {
    char s[32] = {0};
    sprintf(s, "%d", IntObject_AsINT(self));
    return StrObject_FromStr((const char *)s);
}

TypeObject Int_Type = {
    .tp_name = "int",
    .tp_init = int_init,
    .tp_deinit = int_deinit,
    .tp_hash = int_hash,
    .tp_str = int_str,
    .tp_methods = int_methods
};

Object *IntObject_FromInt(int i) {
    Object *_i = Object_Malloc(&Int_Type, sizeof(IntObject));
    Object_Init(_i, Object_CONVERT(&i));
    return _i;
}


