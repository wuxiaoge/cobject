#include "object.h"

static int list_resize(Object *self, size_t size) {
    int ret = -1;
    if(ListObject_SIZE(self) == ListObject_ALLOCATED(self)) {
        if(ListObject_VALUE(self)) {
            ListObject_VALUE(self) = (Object **)realloc(ListObject_VALUE(self),
                (size + ListObject_ALLOCATED(self)) * sizeof(Object *));
            ListObject_ALLOCATED(self) += size;
        } else {
            ListObject_VALUE(self) = (Object **)malloc(
                size * sizeof(Object *));
            ListObject_ALLOCATED(self) = size;
        }
        ret = 0;
    } else if(ListObject_SIZE(self) < ListObject_ALLOCATED(self)) {
        ret = 0;
    } else {
        perror("Error : ListObject instance'size > allocated !");
    }
    return ret;
}

static Object *list_method_append(Object *self, Object *ob) {
    if(!list_resize(self, 1)) {
        Object_INCREF(ob);
        *(ListObject_VALUE(self) + ListObject_SIZE(self)) = ob;
        ListObject_SIZE(self)++;
    }
    return Object_NULL;
}

static void list_move(Object *self, int i) {
    size_t _i = i;
    for(; _i < ListObject_SIZE(self) - 1; _i++) {
        ListObject_GetITEM(self, _i) = ListObject_GetITEM(self, _i + 1);
    }
    ListObject_GetITEM(self, _i) = Object_NULL;
}

static Object *list_method_remove(Object *self, Object *ob) {
    int i = 0;
    for(; i < ListObject_SIZE(self); i++) {
        Object *item = ListObject_GetITEM(self, i);
        if(item == ob) {
            Object_DECREF(item);
            list_move(self, i);
            ListObject_SIZE(self)--;
            break;
        }
    }
    return Object_NULL;
}

static Object *list_method_slice(Object *self, Object *args) {
    return Object_NULL;
}

static Object *list_method_get(Object *self, Object *ob) {
    if(!IntObject_CHECK(ob)) {
        return Object_NULL;
    }
    int index = IntObject_AsINT(ob);
    if(ListObject_SIZE(self) <= (size_t)index) {
        return Object_NULL;
    }
    return ListObject_GetITEM(self, index);
}
static MethodDef list_methods[] = {
    {"Append", list_method_append},
    {"Remove", list_method_remove},
    {"Slice", list_method_slice},
    {"Get", list_method_get},
    {Object_NULL, Object_NULL}
};

static int list_init(Object *self, Object *args) {
    int size = IntObject_AsINT(args);
    ListObject_VALUE(self) = Object_NULL;
    ListObject_SIZE(self) = 0L;
    ListObject_ALLOCATED(self) = 0L;
    list_resize(self, size);
    return 0;
}

static int list_deinit(Object *self) {
    if(ListObject_VALUE(self)) {
        for(int i = 0; i < ListObject_SIZE(self); i++) {
            Object_DECREF(ListObject_GetITEM(self, i));
        }
        free(ListObject_VALUE(self));
    }
    return 0;
}

static long list_hash(Object *self) {
    return -1L;
}

static Object *list_str(Object *self) {
    return Object_NULL;
}

TypeObject List_Type = {
    .tp_init = list_init,
    .tp_deinit = list_deinit,
    .tp_hash = list_hash,
    .tp_str = list_str,
    .tp_methods = list_methods
};

Object *ListObject_New(size_t size) {
    Object *_lst = Object_Malloc(&List_Type, sizeof(ListObject));
    Object *i = IntObject_FromInt((int)size);
    Object_Init(_lst, i);
    Object_DECREF(i);
    return _lst;
}
