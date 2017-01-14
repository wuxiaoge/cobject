#ifndef HTTP_HEADER_OBJECT_H
#define HTTP_HEADER_OBJECT_H
#ifdef __cplusplus
extern "C" {
#endif

typedef struct _httpheaderobject {
    Object_HEAD
} HttpHeaderObject;

extern TypeObject HttpHeader_Type;

#define HttpHeaderObject_CHECK(ob) Object_CHECK(ob, &HttpHeader_Type)
#define HttpHeaderObject_CONVERT(ob) ((HttpHeaderObject *)(ob))

Object *HttpHeaderObject_New(Object *);

#ifdef __cplusplus
}
#endif
#endif
