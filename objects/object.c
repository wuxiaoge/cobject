#include "object.h"

static int object_init(Object *self, Object *args) {
    printf("object init ...\n"); return 0;
}

static int object_deinit(Object *self) {
    printf("object deinit ...\n");
    return 0;
}

static long object_hash(Object *self) {
    printf("object hash ...\n");
    return -1L;
}

static Object *object_str(Object *self) {
    char s[32] = {0};
    sprintf(s, "<object %ld>", (long)self);
    return StrObject_FromStr((const char *)s);
}

TypeObject Object_Type = {
    object_init,
    object_deinit,
    object_hash,
    object_str
};

int TypeObject_Init(Object *self, TypeObject *type, Object *args) {
    int ret = -1;
    TypeObject *_typeobject = type;
    Object *_base = Object_BASE(self);
    if(self && _typeobject) {
        if(_typeobject->tp_init) {
            ret = _typeobject->tp_init(self, args);
        } else {
            ret = _base ? TypeObject_Init(_base, Object_TYPE(_base), args) : -1;
        }
    }
    return ret;
}

int TypeObject_Deinit(Object *self, TypeObject *type) {
    int ret = -1;
    TypeObject *_typeobject = type;
    Object *_base = Object_BASE(self);
    if(self && _typeobject) {
        if(_typeobject->tp_deinit) {
            ret = _typeobject->tp_deinit(self);
        } else {
            ret = _base ? TypeObject_Deinit(_base, Object_TYPE(_base)) : -1;
        }
    }
    return ret;
}

long TypeObject_Hash(Object *self, TypeObject *type) {
    long ret = -1L;
    TypeObject *_typeobject = type;
    Object *_base = Object_BASE(self);
    if(self && _typeobject) {
        if(_typeobject->tp_hash) {
            ret = _typeobject->tp_hash(self);
        } else {
            ret = _base ? TypeObject_Hash(_base, Object_TYPE(_base)) : -1L;
        }
    }
    return ret;
}

Object *TypeObject_Str(Object *self, TypeObject *type) {
    Object *ret = Object_NULL;
    TypeObject *_typeobject = type;
    Object *_base = Object_BASE(self);
    if(self && _typeobject) {
        if(_typeobject->tp_str) {
            ret = _typeobject->tp_str(self);
        } else {
            ret = _base ? TypeObject_Str(_base, Object_TYPE(_base)) : Object_NULL;
        }
    }
    return ret;
}

Object *Object_Malloc(TypeObject *type, size_t size) {
    Object *_ob = Object_MALLOC(size);
    Object_INIT(_ob, type);
    return _ob;
}

int Object_Extend(Object *self, TypeObject *type, size_t size) {
    Object *ob = Object_Malloc(type, size);
    Object_EXTEND(self, ob);
    return 0;
}

int Object_Init(Object *self, Object *args) {
    return TypeObject_Init(self, Object_TYPE(self), args);
}

int Object_Deinit(Object *self) {
    return TypeObject_Deinit(self, Object_TYPE(self));
}

long Object_Hash(Object *self) {
    return TypeObject_Hash(self, Object_TYPE(self));
}

Object *Object_Str(Object *self) {
    return TypeObject_Str(self, Object_TYPE(self));
}

int Object_Check(Object *self, TypeObject *type) {
    return (Object_TYPE(self) == type) ? 1 : 0;
}

size_t Object_IncRef(Object *self) {
    int ret = 0;
    if(self) {
        Object_REFCNT(self)++;
        ret = Object_REFCNT(self);
    }
    return ret;
}

size_t Object_DecRef(Object *self) {
    int ret = 0;
    if(self){
        Object_REFCNT(self)--;
        ret = Object_REFCNT(self);
        if(!ret) {
            Object_Deinit(self);
            Object_Free(&self);
        }
    }
    return ret;
}

void Object_Free(Object **self) {
    Object *_base = Object_NULL;
    Object *_self = self ? *self : Object_NULL;
    if(_self) {
        _base = Object_BASE(_self);
        if(_base) {
            Object_Free(&_base);
        }
        free(_self);
        *self = Object_NULL;
    }
}


