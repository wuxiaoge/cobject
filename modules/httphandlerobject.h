#ifndef HTTP_HANDLER_OBJECT_H
#define HTTP_HANDLER_OBJECT_H
#ifdef __cplusplus
extern "C" {
#endif

typedef struct _httphandlerobject {
    Object_HEAD
} HttpHandlerObject;

extern TypeObject HttpHandler_Type;

#define HttpHandlerObject_CHECK(ob) Object_CHECK(ob, &HttpHandler_Type)
#define HttpHandlerObject_CONVERT(ob) ((HttpHandlerObject *)(ob))

Object *HttpHandlerObject_New();

#ifdef __cplusplus
}
#endif
#endif
