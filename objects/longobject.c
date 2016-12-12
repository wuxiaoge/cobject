#include "object.h"

static int long_init(Object *self, Object *args) {
    Object_Extend(self, &Object_Type, sizeof(Object));
    Object_Init(Object_BASE(self), Object_NULL);
    long _l = *(long *)args;
    LongObject_VALUE(self) = _l;
    return 0;
}

static int long_deinit(Object *self) {
    printf("long deinit ...\n");
    return 0;
}

static long long_hash(Object *self) {
    return LongObject_VALUE(self);
}

static Object *long_str(Object *self) {
    char s[32] = {0};
    sprintf(s, "%ld", LongObject_AsLONG(self));
    return StrObject_FromStr((const char *)s);
}

TypeObject Long_Type = {
    .tp_name = "long",
    .tp_init = long_init,
    .tp_deinit = long_deinit,
    .tp_hash = long_hash,
    .tp_str = long_str
};

Object *LongObject_FromLong(long l) {
    Object *_l = Object_Malloc(&Long_Type, sizeof(LongObject));
    Object_Init(_l, Object_CONVERT(&l));
    return _l;
}


