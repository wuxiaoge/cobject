#ifndef HTTP_HEADER_OBJECT_H
#define HTTP_HEADER_OBJECT_H
#ifdef __cplusplus
extern "C" {
#endif

struct _http_header_item {
    struct _http_header_item *hhi_next;
    Object *key;
    Object *value;
};

typedef struct _httpheaderobject {
    Object_HEAD
    struct _http_header_item *ob_item;
} HttpHeaderObject;

extern TypeObject HttpHeader_Type;

#define HttpHeaderObject_CHECK(ob) Object_CHECK(ob, &HttpHeader_Type)
#define HttpHeaderObject_CONVERT(ob) ((HttpHeaderObject *)(ob))
#define HttpHeaderObject_ITEMS(ob) HttpHeaderObject_CONVERT(ob)->ob_item

Object *HttpHeaderObject_New(Object *);

#ifdef __cplusplus
}
#endif
#endif
