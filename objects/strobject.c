#include "object.h"

static Object *str_method_concat(Object *self, Object *ob) {
    if(!StrObject_CHECK(ob)) {
        return Object_NULL;
    }
    size_t ssize = Object_VAR_SIZE(self);
    size_t osize = Object_VAR_SIZE(ob);
    char *buf = (char *)malloc(ssize + osize + 1);
    memcpy(buf, StrObject_AsSTR(self), ssize);
    memcpy(buf + ssize, StrObject_AsSTR(ob), osize);
    *(char *)(buf + ssize + osize) = 0;
    Object *_s = StrObject_FromStr(buf);
    free(buf);
    return _s;
}

static Object *str_method_substr(Object *self, Object *args) {
    IntObject *a = IntObject_CONVERT(args);
    int start = IntObject_AsINT(a);
    int end = IntObject_AsINT(a+1);
    size_t ssize = Object_VAR_SIZE(self);
    if(!ssize) {
        return Object_NULL;
    }
    if(start < 0) {
        start = ssize + start;
    }
    if(start >= ssize) {
        return Object_NULL;
    }
    if(end < 0) {
        end = ssize + end;
    }
    if(end > ssize) {
        end = ssize;
    }
    if(start < 0 || end < 0 || start > end) {
        return Object_NULL;
    }
    char *buf = (char *)malloc(end - start + 1);
    memcpy(buf, StrObject_AsSTR(self) + start, end - start);
    *(char *)(buf + end - start) = 0;
    Object *_s = StrObject_FromStr(buf);
    free(buf);
    return _s;
}

static Object *str_method_index(Object *self, Object *ob) {
    if(!StrObject_CHECK(ob)) {
        return Object_NULL;
    }
    int index = 0;
    const char *tmp = strstr(StrObject_AsSTR(self), StrObject_AsSTR(ob));
    if(tmp) {
        index = (int)(tmp - StrObject_AsSTR(self));
    }
    return IntObject_FromInt(index);
}

static Object *str_method_format(Object *self, Object *args) {
    return Object_NULL;
}

static MethodDef str_methods[] = {
    {"Concat", str_method_concat},
    {"Substr", str_method_substr},
    {"Index", str_method_index},
    {"Format", str_method_format},
    {Object_NULL, Object_NULL}
};

static int str_init(Object *self, Object *args) {
    Object_Extend(self, &Object_Type, sizeof(Object));
    Object_Init(Object_BASE(self), Object_NULL);
    const char *_s = (const char *)args;
    *(char *)(StrObject_VALUE(self) + StrObject_SIZE(self)) = 0;
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


