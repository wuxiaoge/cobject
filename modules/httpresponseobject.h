#ifndef HTTP_RESPONSE_OBJECT_H
#define HTTP_RESPONSE_OBJECT_H
#ifdef __cplusplus
extern "C" {
#endif

typedef struct _httpresponseobject {
    Object_HEAD
    Object *ob_protocol;
    Object *ob_version;
    Object *ob_status_code;
    Object *ob_status_text;
    Object *ob_headers;
    Object *ob_body;
} HttpResponseObject;

extern TypeObject HttpResponse_Type;

#define HttpResponseObject_CHECK(ob) Object_CHECK(ob, &HttpResponse_Type)
#define HttpResponseObject_CONVERT(ob) ((HttpResponseObject *)(ob))
#define HttpResponseObject_PROTOCOL(self) HttpResponseObject_CONVERT(self)->ob_protocol
#define HttpResponseObject_VERSION(self) HttpResponseObject_CONVERT(self)->ob_version
#define HttpResponseObject_STATUS_CODE(self) HttpResponseObject_CONVERT(self)->ob_status_code
#define HttpResponseObject_STATUS_TEXT(self) HttpResponseObject_CONVERT(self)->ob_status_text
#define HttpResponseObject_HEADERS(self) HttpResponseObject_CONVERT(self)->ob_headers
#define HttpResponseObject_BODY(self) HttpResponseObject_CONVERT(self)->ob_body

Object *HttpResponseObject_New(Object *, Object *);

#ifdef __cplusplus
}
#endif
#endif
