#include "object.h"

static int str_init(Object *self, Object *args) {
    const char *_s = (const char *)args;
    memset(StrObject_VALUE(self), 0, StrObject_SIZE(self) + 1);
    memcpy(StrObject_VALUE(self), _s, StrObject_SIZE(self));
    return 0;
} 
static int str_deinit(Object *self) {
    return 0;
}

static long str_hash(Object *self) {
    return -1L;
}

static Object *str_str(Object *self) {
    Object_INCREF(self);
    return self;
}

TypeObject Str_Type = {
    Object_HEAD_INIT
    str_init,
    str_deinit,
    str_hash,
    str_str
};

Object *StrObject_FromStrAndSize(const char *s, size_t size) {
    Object *_s = Object_Malloc(&Str_Type, sizeof(StrObject) + size);
    StrObject_SIZE(_s) = size;
    Object_Init(_s, (Object *)s);
    return _s;
}

Object *StrObject_FromStr(const char *s) {
    size_t size = strlen(s);
    return StrObject_FromStrAndSize(s, size);
}


