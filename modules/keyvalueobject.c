#include "object.h"
#include "keyvalueobject.h"

static Object *keyvalue_method_add(Object *self, Object *args) {
    assert(ListObject_CHECK(args) && ListObject_SIZE(args)==2);
    Object *_i0 = IntObject_FromInt(0);
    Object *key = Object_CallMethod(args, "Get", _i0);
    Object_DECREF(_i0);
    Object *_i1 = IntObject_FromInt(1);
    Object *value = Object_CallMethod(args, "Get", _i1);
    Object_DECREF(_i1);
    struct _key_value_item *item = (struct _key_value_item *)malloc(sizeof(struct _key_value_item));
    item->key = key;
    Object_INCREF(key);
    item->value = value;
    Object_INCREF(value);
    item->hhi_next = KeyValueObject_ITEMS(self);
    KeyValueObject_ITEMS(self) = item;
    return Object_NULL;
}

static Object *keyvalue_method_remove(Object *self, Object *key) {
    struct _key_value_item *item = KeyValueObject_ITEMS(self);
    struct _key_value_item *prev = NULL;
    while(item) {
        if(Object_Equal(key, item->key)) {
            if(!prev) {
                KeyValueObject_ITEMS(self) = item->hhi_next;
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

static Object *keyvalue_method_get(Object *self, Object *key) {
    struct _key_value_item *item = KeyValueObject_ITEMS(self);
    while(item) {
        if(Object_Equal(key, item->key)) {
            return item->value;
        }
        item = item->hhi_next;
    }
    return Object_NULL;
}

static MethodDef keyvalue_methods[] = {
    {"Add", keyvalue_method_add},
    {"Remove", keyvalue_method_remove},
    {"Get", keyvalue_method_get},
    {Object_NULL, Object_NULL}
};

static int keyvalue_init(Object *self, Object *args) {
    Object_Extend(self, &Object_Type, sizeof(Object));
    Object_Init(Object_BASE(self), Object_NULL);
    KeyValueObject_ITEMS(self) = NULL;
    return Object_OK;
}

static int keyvalue_deinit(Object *self) {
    struct _key_value_item*item = KeyValueObject_ITEMS(self);
    struct _key_value_item*tmp = NULL;
    while(item) {
        tmp = item->hhi_next;
        Object_DECREF(item->key);
        Object_DECREF(item->value);
        free(item);
        item = tmp;
    }
    return Object_Deinit(Object_BASE(self));
}

static long keyvalue_hash(Object *self) {
    return -1L;
}

static BOOL keyvalue_equal(Object *self, Object *ob) {
    return Object_Equal(Object_BASE(self), Object_BASE(ob));
}

static Object *keyvalue_str(Object *self) {
    Object *colon = StrObject_FromStr(": ");
    Object *comma = StrObject_FromStr(", ");
    Object *brace_begin = StrObject_FromStr("{");
    Object *key = Object_NULL;
    Object *value = Object_NULL;
    Object *result = StrObject_FromStr("}");
    Object *tmp = Object_NULL;
    struct _key_value_item *item = KeyValueObject_ITEMS(self);
    int idx = 0;
    while(item) {
        key = item->key;
        value = item->value;
        if(idx) {
            tmp = Object_CallMethod(comma, "Concat", result);
            Object_DECREF(result);
        } else {
            tmp = result;
        }
        idx++;
        result = Object_CallMethod(value, "Concat", tmp);
        Object_DECREF(tmp);
        tmp = Object_CallMethod(colon, "Concat", result);
        Object_DECREF(result);
        result = Object_CallMethod(key, "Concat", tmp);
        Object_DECREF(tmp);
        item = item->hhi_next;
    }
    tmp = result;
    result = Object_CallMethod(brace_begin, "Concat", tmp);
    Object_DECREF(tmp);
    Object_DECREF(colon);
    Object_DECREF(comma);
    Object_DECREF(brace_begin);
    return result;
}

TypeObject KeyValue_Type = {
    .tp_name = "keyvalue",
    .tp_init = keyvalue_init,
    .tp_deinit = keyvalue_deinit,
    .tp_hash = keyvalue_hash,
    .tp_equal = keyvalue_equal,
    .tp_str = keyvalue_str,
    .tp_methods = keyvalue_methods
};

Object *KeyValueObject_New() {
    Object *_keyvalue= Object_Malloc(&KeyValue_Type, sizeof(KeyValueObject));
    Object_Init(_keyvalue, Object_NULL);
    return _keyvalue;
}

