#include "object.h"

static int list_resize(Object *self, size_t size) {
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
    } else if(ListObject_SIZE(self) > ListObject_ALLOCATED(self)) {
        perror("Error : ListObject instance'size > allocated !");
    }
    return -1;
}

static Object *list_method_append(Object *self, Object *ob) {
    if(!list_resize(self, 1)) {
        *(ListObject_VALUE(self) + ListObject_SIZE(self)) = ob;
        ListObject_SIZE(self)++;
    }
    return Object_NULL;
}

static Object *list_method_remove(Object *self, Object *ob) {
    int i = 0;
    for(; i < ListObject_SIZE(self); i++) {
        Object *item = ListObject_GetITEM(self, i);
        if(item == ob) {
            Object_DECREF(item);
            ListObject_GetITEM(self, i) = Object_NULL;
            ListObject_SIZE(self)--;
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
    if(ListObject_SIZE(self) <= index) {
        return Object_NULL;
    }
    Object *item = ListObject_GetITEM(self, index);
    Object_INCREF(item);
    return item;
}
static MethodDef list_methods[] = {
    {"Append", list_method_append},
    {"Remove", list_method_remove},
    {"Slice", list_method_slice},
    {"Get", list_method_get},
    {Object_NULL, Object_NULL}
};

static int list_init(Object *self, Object *args) {
    ListObject_VALUE(self) = Object_NULL;
    ListObject_SIZE(self) = 0L;
    ListObject_ALLOCATED(self) = 0L;
    return 0;
}

static int list_deinit(Object *self) {
    if(ListObject_VALUE(self)) {
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

Object *ListObject_New() {
    Object *_lst = Object_Malloc(&List_Type, sizeof(ListObject));
    ListObject_SIZE(_lst) = 0;
    Object_Init(_lst, Object_NULL);
    return _lst;
}
