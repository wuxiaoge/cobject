#include "object.h"
#include "httpheaderobject.h"

static Object *httpheader_method_add(Object *self, Object *args) {
    assert(ListObject_CHECK(args) && ListObject_SIZE(args)==2);
    Object *_i0 = IntObject_FromInt(0);
    Object *key = Object_CallMethod(args, "Get", _i0);
    Object_DECREF(_i0);
    Object *_i1 = IntObject_FromInt(1);
    Object *value = Object_CallMethod(args, "Get", _i1);
    Object_DECREF(_i1);
    struct _http_header_item *item = (struct _http_header_item *)malloc(sizeof(struct _http_header_item));
    item->key = key;
    Object_INCREF(key);
    item->value = value;
    Object_INCREF(value);
    item->hhi_next = HttpHeaderObject_ITEMS(self);
    HttpHeaderObject_ITEMS(self) = item;
    return Object_NULL;
}

static Object *httpheader_method_remove(Object *self, Object *key) {
    struct _http_header_item *item = HttpHeaderObject_ITEMS(self);
    struct _http_header_item *prev = NULL;
    while(item) {
        if(Object_Equal(key, item->key)) {
            if(!prev) {
                HttpHeaderObject_ITEMS(self) = item->hhi_next;
            }else {
                prev->hhi_next = item->hhi_next;
            }
            Object_DECREF(item->key);
            Object_DECREF(item->value);
            free(item);
        }
        prev = item;
        item = item->hhi_next;
    }
    return Object_NULL;
}

static Object *httpheader_method_get(Object *self, Object *key) {
    struct _http_header_item *item = HttpHeaderObject_ITEMS(self);
    while(item) {
        if(Object_Equal(key, item->key)) {
            return item->value;
        }
        item = item->hhi_next;
    }
    return Object_NULL;
}

static MethodDef httpheader_methods[] = {
    {"Add", httpheader_method_add},
    {"Remove", httpheader_method_remove},
    {"Get", httpheader_method_get},
    {Object_NULL, Object_NULL}
};

static int httpheader_init(Object *self, Object *args) {
    Object_Extend(self, &Object_Type, sizeof(Object));
    Object_Init(Object_BASE(self), Object_NULL);
    HttpHeaderObject_ITEMS(self) = NULL;
    if(!(args && StrObject_CHECK(args))) return Object_OK;
    Object *index = Object_NULL;
    Object *item = Object_NULL;
    Object *colon = StrObject_FromStr(": ");
    Object *newline = StrObject_FromStr("\r\n");
    Object *res = Object_CallMethod(args, "Split", newline);
    Object_DECREF(newline);
    Object *lst = Object_NULL;
    int i = 0;
    for(; i < ListObject_SIZE(res); i++) {
        index = IntObject_FromInt(i);
        item = Object_CallMethod(res, "Get", index);
        Object_DECREF(index);
        lst = Object_CallMethod(item, "Split", colon);
        if(ListObject_SIZE(lst) == 2) {
            Object_CallMethod(self, "Add", lst);
        }
        Object_DECREF(lst);
    }
    Object_DECREF(colon);
    Object_DECREF(res);
    return Object_OK;
}

static int httpheader_deinit(Object *self) {
    struct _http_header_item *item = HttpHeaderObject_ITEMS(self);
    struct _http_header_item *tmp = NULL;
    while(item) {
        tmp = item->hhi_next;
        Object_DECREF(item->key);
        Object_DECREF(item->value);
        free(item);
        item = tmp;
    }
    return Object_Deinit(Object_BASE(self));
}

static long httpheader_hash(Object *self) {
    return -1L;
}

static BOOL httpheader_equal(Object *self, Object *ob) {
    return Object_Equal(Object_BASE(self), Object_BASE(ob));
}

static Object *httpheader_str(Object *self) {
    Object *colon = StrObject_FromStr(": ");
    Object *newline = StrObject_FromStr("\r\n");
    Object *key = Object_NULL;
    Object *value = Object_NULL;
    Object *result = StrObject_FromStr("");
    Object *tmp = Object_NULL;
    struct _http_header_item *item = HttpHeaderObject_ITEMS(self);
    while(item) {
        key = item->key;
        value = item->value;
        tmp = Object_CallMethod(newline, "Concat", result);
        Object_DECREF(result);
        result = Object_CallMethod(value, "Concat", tmp);
        Object_DECREF(tmp);
        tmp = Object_CallMethod(colon, "Concat", result);
        Object_DECREF(result);
        result = Object_CallMethod(key, "Concat", tmp);
        Object_DECREF(tmp);
        tmp = result;
        item = item->hhi_next;
    }
    Object_DECREF(colon);
    Object_DECREF(newline);
    return result;
}

TypeObject HttpHeader_Type = {
    .tp_name = "httpheader",
    .tp_init = httpheader_init,
    .tp_deinit = httpheader_deinit,
    .tp_hash = httpheader_hash,
    .tp_equal = httpheader_equal,
    .tp_str = httpheader_str,
    .tp_methods = httpheader_methods
};

Object *HttpHeaderObject_New(Object *header_str) {
    Object *_header = Object_Malloc(&HttpHeader_Type, sizeof(HttpHeaderObject));
    Object_Init(_header, header_str);
    return _header;
}

