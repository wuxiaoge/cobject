#ifndef HTTP_REQUEST_OBJECT_H
#define HTTP_REQUEST_OBJECT_H
#ifdef __cplusplus
extern "C" {
#endif

typedef struct _httprequestobject {
    Object_HEAD
    Object *ob_querystr;
    Object *ob_method;
    Object *ob_url;
    Object *ob_protocol;
    Object *ob_version;
    Object *ob_headers;
    Object *ob_arguments;
    Object *ob_body;
} HttpRequestObject;

extern TypeObject HttpRequest_Type;

#define HttpRequestObject_CHECK(ob) Object_CHECK(ob, &HttpRequest_Type)
#define HttpRequestObject_CONVERT(ob) ((HttpRequestObject *)(ob))
#define HttpRequestObject_METHOD(ob) HttpRequestObject_CONVERT(ob)->ob_method
#define HttpRequestObject_URL(ob) HttpRequestObject_CONVERT(ob)->ob_url
#define HttpRequestObject_PROTOCOL(ob) HttpRequestObject_CONVERT(ob)->ob_protocol
#define HttpRequestObject_VERSION(ob) HttpRequestObject_CONVERT(ob)->ob_version
#define HttpRequestObject_HEADERS(ob) HttpRequestObject_CONVERT(ob)->ob_headers
#define HttpRequestObject_ARGUMENTS(ob) HttpRequestObject_CONVERT(ob)->ob_arguments
#define HttpRequestObject_BODY(ob) HttpRequestObject_CONVERT(ob)->ob_body
#define HttpRequestObject_QUERYSTR(ob) HttpRequestObject_CONVERT(ob)->ob_querystr

Object *HttpRequestObject_New(Object *);

#ifdef __cplusplus
}
#endif
#endif
