#ifndef KEY_VALUE_OBJECT_H
#define KEY_VALUE_OBJECT_H
#ifdef __cplusplus
extern "C" {
#endif

struct _key_value_item {
    struct _key_value_item *hhi_next;
    Object *key;
    Object *value;
};

typedef struct _keyvalueobject {
    Object_HEAD
    struct _key_value_item *ob_item;
} KeyValueObject;

extern TypeObject KeyValue_Type;

#define KeyValueObject_CHECK(ob) Object_CHECK(ob, &KeyValue_Type)
#define KeyValueObject_CONVERT(ob) ((KeyValueObject *)(ob))
#define KeyValueObject_ITEMS(ob) KeyValueObject_CONVERT(ob)->ob_item

Object *KeyValueObject_New();

#ifdef __cplusplus
}
#endif
#endif
