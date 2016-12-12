#include "object.h"

MethodDef str_methods[] = {
    {Object_NULL, Object_NULL}
};

static int str_init(Object *self, Object *args) {
    Object_Extend(self, &Object_Type, sizeof(Object));
    Object_Init(Object_BASE(self), Object_NULL);
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
    .tp_name = "str",
    .tp_init = str_init,
    .tp_deinit = str_deinit,
    .tp_hash = str_hash,
    .tp_str = str_str,
    .tp_methods = str_methods 
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


