#include "object.h"

static int object_init(Object *self, Object *args) {
    printf("object init ...\n");
    return 0;
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
    Object_HEAD_INIT
    object_init,
    object_deinit,
    object_hash,
    object_str
};

int TypeObject_Init(Object *self, TypeObject *type, Object *args) {
    TypeObject *_typeobject = type;
    if(_typeobject) {
        if(_typeobject->tp_init) {
            return _typeobject->tp_init(self, args);
        } else {
            return TypeObject_Init(self, Object_TYPE(_typeobject), args);
        }
    }
    return -1;
}

int TypeObject_Deinit(Object *self, TypeObject *type) {
    TypeObject *_typeobject = type;
    if(_typeobject) {
        if(_typeobject->tp_deinit) {
            return _typeobject->tp_deinit(self);
        } else {
            return TypeObject_Deinit(self, Object_TYPE(_typeobject));
        }
    }
    return -1;
}

long TypeObject_Hash(Object *self, TypeObject *type) {
    TypeObject *_typeobject = type;
    if(_typeobject) {
        if(_typeobject->tp_hash) {
            return _typeobject->tp_hash(self);
        } else {
            return TypeObject_Hash(self, Object_TYPE(_typeobject));
        }
    }
    return -1L;
}

Object *TypeObject_Str(Object *self, TypeObject *type) {
    TypeObject *_typeobject = type;
    if(_typeobject) {
        if(_typeobject->tp_str) {
            return _typeobject->tp_str(self);
        } else {
            return TypeObject_Str(self, Object_TYPE(_typeobject));
        }
    }
    return Object_NULL;
}

Object *Object_Malloc(TypeObject *type, size_t size) {
    Object *_ob = Object_MALLOC(size);
    Object_INIT(_ob, type);
    return _ob;
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


