#include "object.h"

static MethodDef object_methods[] = {
    {Object_NULL, Object_NULL}
};

static int object_init(Object *self, Object *args) {
    return Object_OK;
}

static int object_deinit(Object *self) {
    return Object_OK;
}

static long object_hash(Object *self) {
    return -1L;
}

static BOOL object_equal(Object *self, Object *ob) {
    if(self == ob) {
        return TRUE;
    }
    return FALSE;
}

static Object *object_str(Object *self) {
    char s[32] = {0};
    sprintf(s, "<object %ld>", (long)self);
    return StrObject_FromStr((const char *)s);
}

TypeObject Object_Type = {
    .tp_name = "object",
    .tp_init = object_init,
    .tp_deinit = object_deinit,
    .tp_hash = object_hash,
    .tp_equal = object_equal,
    .tp_str = object_str,
    .tp_methods = object_methods
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

BOOL TypeObject_Equal(Object *self, TypeObject *type, Object *ob) {
    BOOL ret = FALSE;
    TypeObject *_typeobject = type;
    Object *_base = Object_BASE(self);
    if(self && _typeobject) {
        if(_typeobject->tp_equal) {
            ret = _typeobject->tp_equal(self, ob);
        } else {
            ret = _base ? TypeObject_Equal(_base, Object_TYPE(_base), Object_BASE(ob)) : FALSE;
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

BOOL Object_Equal(Object *self, Object *ob) {
    if( (!self) || (!ob) ) {
        return FALSE;
    }
    return TypeObject_Equal(self, Object_TYPE(self), ob);
}

Object *Object_Str(Object *self) {
    return TypeObject_Str(self, Object_TYPE(self));
}

BOOL Object_Check(Object *self, TypeObject *type) {
    if(Object_TYPE(self) == type) {
        return TRUE;
    }
    Object *_base = Object_BASE(self);
    if(!_base) {
        return FALSE;
    }
    return Object_Check(_base, type);
}

methodfunc Object_GetMethod(Object *self, Object *name, Object **owner) {
    MethodDef *md = Object_TYPE(self)->tp_methods;
    while(md && md->md_name){
        if(!strcmp(StrObject_AsSTR(name), md->md_name)) {
            *owner = self;
            return md->md_func;
        }
        md++;
    }
    return Object_BASE(self) ? Object_GetMethod(Object_BASE(self), name, owner) : Object_NULL;
}

Object *Object_CallMethod(Object *self, const char *name, Object *args) {
    Object *ret = Object_NULL;
    Object **_owner = (Object **)malloc(sizeof(Object *));
    Object *_s = StrObject_FromStr(name);
    methodfunc mf = Object_GetMethod(self, _s, _owner);
    Object_DECREF(_s);
    if(mf) {
        ret = mf(*_owner, args);
    }
    free(_owner);
    return ret;
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
    Object *_self = self ? *self : Object_NULL;
    if(_self) {
        if(Object_BASE(_self)) {
            Object_Free(&Object_BASE(_self));
        }
        free(_self);
        *self = Object_NULL;
    }
}


