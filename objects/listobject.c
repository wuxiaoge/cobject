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

static Object *list_method_remove(Object *self, Object *ob) {
    Object *item = Object_NULL;
    Object *index = Object_NULL;
    int i = 0;
    for(; i < ListObject_SIZE(self); i++) {
        index = IntObject_FromInt(i);
        item = list_method_get(self, index);
        Object_DECREF(index);
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
    if(!ListObject_CHECK(args)) {
        return Object_NULL;
    }
    Object *ostart = ListObject_GetITEM(args, 0);
    Object *oend = ListObject_GetITEM(args, 1);
    int start = IntObject_AsINT(ostart);
    int end = IntObject_AsINT(oend);
    size_t lsize = ListObject_SIZE(self);
    if(!lsize) {
        return Object_NULL;
    }
    if(start < 0) {
        start = lsize + start;
    }
    if(start >= lsize) {
        return Object_NULL;
    }
    if(end < 0) {
        end = lsize + end;
    }
    if(end > lsize) {
        end = lsize;
    }
    if(start < 0 || end < 0 || start > end) {
        return Object_NULL;
    }
    Object *_lst = ListObject_New((int)(end - start));
    Object *item = Object_NULL;
    Object *index = Object_NULL;
    int i = start;
    for(; i < end; i++) {
        index = IntObject_FromInt(i);
        item = list_method_get(self, index);
        list_method_append(_lst, item);
        Object_DECREF(index);
    }
    return _lst;
}

static Object *list_method_foreach(Object *self, Object *func) {
    int i = 0, ret = 0;
    Object *index = Object_NULL;
    Object *item = Object_NULL;
    list_item_foreach_cb callback = (list_item_foreach_cb)func;
    for(; i < ListObject_SIZE(self); i++) {
        index = IntObject_FromInt(i);
        item = list_method_get(self, index);
        ret = callback(index, item);
        Object_DECREF(index);
        if(ret) {
            break;
        }
    }
    return Object_NULL;
}

static Object *list_method_map(Object *self, Object *func) {
    Object *index = Object_NULL;
    Object *item = Object_NULL;
    Object *_item = Object_NULL;
    list_item_map_cb callback = (list_item_map_cb)func;
    Object *_lst = ListObject_New(ListObject_SIZE(self));
    int i = 0;
    for(;i < ListObject_SIZE(self); i++) {
        index = IntObject_FromInt(i);
        _item = list_method_get(self, index);
        item = callback(index, _item);
        list_method_append(_lst, item);
        Object_DECREF(index);
        Object_DECREF(item);
    }
    return _lst;
}

static Object *list_method_filter(Object *self, Object *func) {
    Object *index = Object_NULL;
    Object *item = Object_NULL;
    list_item_filter_cb callback = (list_item_filter_cb)func;
    Object *_lst = ListObject_New(ListObject_SIZE(self));
    int i = 0, ret = FALSE;
    for(;i < ListObject_SIZE(self); i++) {
        index = IntObject_FromInt(i);
        item = list_method_get(self, index);
        ret = callback(index, item);
        if(ret) {
            list_method_append(_lst, item);
        }
        Object_DECREF(index);
    }
    return _lst;
}

static MethodDef list_methods[] = {
    {"Append", list_method_append},
    {"Remove", list_method_remove},
    {"Slice", list_method_slice},
    {"Get", list_method_get},
    {"Foreach", list_method_foreach},
    {"Map", list_method_map},
    {"Filter", list_method_filter},
    {Object_NULL, Object_NULL}
};

static int list_init(Object *self, Object *args) {
    int size = IntObject_AsINT(args);
    ListObject_VALUE(self) = Object_NULL;
    ListObject_SIZE(self) = 0L;
    ListObject_ALLOCATED(self) = 0L;
    list_resize(self, size);
    return Object_OK;
}

static int list_deinit(Object *self) {
    Object *index = Object_NULL;
    Object *item = Object_NULL;
    if(ListObject_VALUE(self)) {
        int i = 0;
        for(; i < ListObject_SIZE(self); i++) {
            index = IntObject_FromInt(i);
            item = list_method_get(self, index);
            Object_DECREF(index);
            Object_DECREF(item);
        }
        free(ListObject_VALUE(self));
    }
    return Object_OK;
}

static long list_hash(Object *self) {
    return -1L;
}

static Object *list_str(Object *self) {
    int i = 0;
    Object *ret = StrObject_FromStr("[");
    Object *_s = Object_NULL;
    Object *item = Object_NULL;
    Object *index = Object_NULL;
    Object *tmp = Object_NULL;
    Object *_dh = StrObject_FromStr(", ");
    for(; i < ListObject_SIZE(self); i++) {
        index = IntObject_FromInt(i);
        item = list_method_get(self, index);
        Object_DECREF(index);
        _s = Object_Str(item);
        tmp = Object_CallMethod(ret, "Concat", _s);
        Object_DECREF(ret);
        Object_DECREF(_s);
        ret = tmp;
        tmp = Object_NULL;
        if(i < (ListObject_SIZE(self) - 1)) {
            tmp = Object_CallMethod(ret, "Concat", _dh);
            Object_DECREF(ret);
            ret = tmp;
            tmp = Object_NULL;
        }
    }
    _s = StrObject_FromStr("]");
    tmp = Object_CallMethod(ret, "Concat", _s);
    Object_DECREF(ret);
    Object_DECREF(_s);
    Object_DECREF(_dh);
    ret = tmp;
    tmp = Object_NULL;
    return ret;
}

TypeObject List_Type = {
    .tp_init = list_init,
    .tp_deinit = list_deinit,
    .tp_hash = list_hash,
    .tp_equal = Object_NULL,
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
