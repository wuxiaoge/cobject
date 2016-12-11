#include "object.h"

static int int_init(Object *self, Object *args) {
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
    Object_HEAD_INIT
    int_init,
    int_deinit,
    int_hash,
    int_str
};

Object *IntObject_FromInt(int i) {
    Object *_i = Object_Malloc(&Int_Type, sizeof(IntObject));
    Object_Init(_i, Object_CONVERT(&i));
    return _i;
}


