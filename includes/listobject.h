#ifndef LIST_OBJECT_H
#define LIST_OBJECT_H
#ifdef __cplusplus
extern "C" {
#endif

typedef void (*list_item_callback)(Object *, Object *);

typedef struct _listobject {
    Object_VAR_HEAD
    Object **ob_item;
    size_t allocated;
} ListObject;

extern TypeObject List_Type;

#define ListObject_CHECK(ob) Object_CHECK(ob, &List_Type)
#define ListObject_CONVERT(ob) ((ListObject *)ob)
#define ListObject_VALUE(ob) ListObject_CONVERT(ob)->ob_item
#define ListObject_SIZE(ob) ListObject_CONVERT(ob)->ob_size
#define ListObject_ALLOCATED(ob) ListObject_CONVERT(ob)->allocated
#define ListObject_GetITEM(ob, idx) *(ListObject_VALUE(ob) + (idx))

Object *ListObject_New(size_t);

#ifdef __cplusplus
}
#endif
#endif
