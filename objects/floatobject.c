#include "object.h"

static int float_init(Object *self, Object *args) {
    Object_Extend(self, &Object_Type, sizeof(Object));
    Object_Init(Object_BASE(self), Object_NULL);
    float _f = *(float *)args;
    FloatObject_VALUE(self) = _f;
    return 0;
}

static int float_deinit(Object *self) {
    return 0;
}

static long float_hash(Object *self) {
    return -1L;
}

static Object *float_str(Object *self) {
    char s[32] = {0};
    sprintf(s, "%f", FloatObject_AsFLOAT(self));
    return StrObject_FromStr(s);
}

TypeObject Float_Type = {
    float_init,
    float_deinit,
    float_hash,
    float_str
};

Object *FloatObject_FromFloat(float f) {
    Object *_f = Object_Malloc(&Float_Type, sizeof(FloatObject));
    Object_Init(_f, Object_CONVERT(&f));
    return _f;
}



