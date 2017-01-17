#ifndef HTTP_SERVER_OBJECT_H
#define HTTP_SERVER_OBJECT_H
#ifdef __cplusplus
extern "C" {
#endif

typedef struct _httpserverobject {
    Object_HEAD
    Object *ob_sock;
    Object *ob_handers;
} HttpServerObject;

extern TypeObject HttpServer_Type;

#define HttpServerObject_CHECK(ob) Object_CHECK(ob, &HttpServer_Type)
#define HttpServerObject_CONVERT(ob) ((HttpServerObject *)(ob))
#define HttpServerObject_SOCK(ob) HttpServerObject_CONVERT(ob)->ob_sock
#define HttpServerObject_HANDERS(ob) HttpServerObject_CONVERT(ob)->ob_handers

Object *HttpServerObject_New(Object *, Object *);

#ifdef __cplusplus
}
#endif
#endif
