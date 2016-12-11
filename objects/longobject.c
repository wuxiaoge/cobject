#include "object.h"

static int long_init(Object *self, Object *args) {
    long _l = *(long *)args;
    LongObject_VALUE(self) = _l;
    return 0;
}

static int long_deinit(Object *self) {
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
    Object_HEAD_INIT
    long_init,
    long_deinit,
    long_hash,
    long_str
};

Object *LongObject_FromLong(long l) {
    Object *_l = Object_Malloc(&Long_Type, sizeof(LongObject));
    Object_Init(_l, Object_CONVERT(&l));
    return _l;
}


