#include "object.h"

static Object *str_method_concat(Object *self, Object *ob) {
    assert(StrObject_CHECK(ob));
    size_t ssize = Object_VAR_SIZE(self);
    size_t osize = Object_VAR_SIZE(ob);
    char *buf = (char *)malloc(ssize + osize + 1);
    memcpy(buf, StrObject_AsSTR(self), ssize);
    memcpy(buf + ssize, StrObject_AsSTR(ob), osize);
    Object *_s = StrObject_FromStrAndSize(buf, ssize + osize);
    free(buf);
    return _s;
}

static Object *str_method_substr(Object *self, Object *args) {
    int start, end;
    size_t ssize = StrObject_SIZE(self);
    if(!ssize) {
        return Object_NULL;
    }
    if(ListObject_CHECK(args)){
        start = IntObject_AsINT(ListObject_GetITEM(args, 0));
        end = IntObject_AsINT(ListObject_GetITEM(args, 1));
    } else if(IntObject_CHECK(args)) {
        start = IntObject_AsINT(args);
        end = ssize;
    } else {
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
    return StrObject_FromStrAndSize(StrObject_AsSTR(self) + start, end - start);
}

static Object *str_method_index(Object *self, Object *ob) {
    assert(StrObject_CHECK(ob));
    int index = -1;
    const char *tmp = strstr(StrObject_AsSTR(self), StrObject_AsSTR(ob));
    if(tmp) {
        index = (int)(tmp - StrObject_AsSTR(self));
    }
    return IntObject_FromInt(index);
}

static Object *str_method_split(Object *self, Object *ob) {
    assert(StrObject_CHECK(ob));
    const char *start = StrObject_AsSTR(self);
    const char *end = NULL;
    Object *size = IntObject_FromInt(1);
    Object *_lst = ListObject_New(size);
    Object_DECREF(size);
    Object *sub = Object_NULL;
    do {
        end = strstr(start, StrObject_AsSTR(ob));
        if(end) {
            sub = StrObject_FromStrAndSize(start, end - start);
            start = (const char *)(end + StrObject_SIZE(ob));
        } else {
            sub = StrObject_FromStrAndSize(start, StrObject_AsSTR(self) + StrObject_SIZE(self) - start);
        }
        Object_CallMethod(_lst, "Append", sub);
        Object_DECREF(sub);
    } while(end);
    return _lst;
}

static MethodDef str_methods[] = {
    {"Concat", str_method_concat},
    {"Substr", str_method_substr},
    {"Index", str_method_index},
    {"Split", str_method_split},
    {Object_NULL, Object_NULL}
};

static int str_init(Object *self, Object *args) {
    Object_Extend(self, &Object_Type, sizeof(Object));
    Object_Init(Object_BASE(self), Object_NULL);
    const char *_s = (const char *)args;
    *(char *)(StrObject_VALUE(self) + StrObject_SIZE(self)) = 0;
    memcpy(StrObject_VALUE(self), _s, StrObject_SIZE(self));
    return Object_OK;
} 
static int str_deinit(Object *self) {
    return Object_OK;
}

static long str_hash(Object *self) {
    return -1L;
}

static BOOL str_equal(Object *self, Object *ob) {
    return 0 == strcmp(StrObject_AsSTR(self), StrObject_AsSTR(ob)) ? TRUE : FALSE;
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
    .tp_equal = str_equal,
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


