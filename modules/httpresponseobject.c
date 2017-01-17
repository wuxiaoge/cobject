#include "object.h"
#include "httpheaderobject.h"
#include "httpresponseobject.h"

static MethodDef httpresponse_methods[] = {
    {Object_NULL, Object_NULL}
};

static int httpresponse_init(Object *self, Object *args) {
    assert(ListObject_CHECK(args) && ListObject_SIZE(args)==2);
    Object *_i0 = IntObject_FromInt(0);
    Object *_i1 = IntObject_FromInt(1);
    HttpResponseObject_STATUS_CODE(self) = Object_CallMethod(args, "Get", _i0);
    Object_INCREF(HttpResponseObject_STATUS_CODE(self));
    HttpResponseObject_STATUS_TEXT(self) = Object_CallMethod(args, "Get", _i1);
    Object_INCREF(HttpResponseObject_STATUS_TEXT(self));
    Object_DECREF(_i0);
    Object_DECREF(_i1);
    HttpResponseObject_PROTOCOL(self) = StrObject_FromStr("HTTP");
    HttpResponseObject_VERSION(self) = StrObject_FromStr("1.1");
    Object *header_text = StrObject_FromStr(
        "Server: LQS/1.0\r\n"
        "Connection: keep-alive\r\n"
        "Content-Length: 11\r\n"
    );
    HttpResponseObject_HEADERS(self) = HttpHeaderObject_New(header_text);
    Object_DECREF(header_text);
    HttpResponseObject_BODY(self) = StrObject_FromStr("Success !!!");
    return Object_OK;
}

static int httpresponse_deinit(Object *self) {
    Object_DECREF(HttpResponseObject_PROTOCOL(self));
    Object_DECREF(HttpResponseObject_VERSION(self));
    Object_DECREF(HttpResponseObject_STATUS_CODE(self));
    Object_DECREF(HttpResponseObject_STATUS_TEXT(self));
    Object_DECREF(HttpResponseObject_HEADERS(self));
    Object_DECREF(HttpResponseObject_BODY(self));
    return Object_OK;
}

static long httpresponse_hash(Object *self) {
    return -1L;
}

static BOOL httpresponse_equal(Object *self, Object *ob) {
    return Object_Equal(Object_BASE(self), Object_BASE(ob));
}

static Object *httpresponse_str(Object *self) {
    Object *result = Object_NULL;
    Object *tmp = Object_NULL;
    Object *line = StrObject_FromStr("/");
    Object *space = StrObject_FromStr(" ");
    Object *newline = StrObject_FromStr("\r\n");
    result = Object_CallMethod(HttpResponseObject_PROTOCOL(self), "Concat", line);
    tmp = Object_CallMethod(result, "Concat", HttpResponseObject_VERSION(self));
    Object_DECREF(result);
    result = Object_CallMethod(tmp, "Concat", space);
    Object_DECREF(tmp);
    tmp = Object_CallMethod(result, "Concat", HttpResponseObject_STATUS_CODE(self));
    Object_DECREF(result);
    result = Object_CallMethod(tmp, "Concat", space);
    Object_DECREF(tmp);
    tmp = Object_CallMethod(result, "Concat", HttpResponseObject_STATUS_TEXT(self));
    Object_DECREF(result);
    result = Object_CallMethod(tmp, "Concat", newline);
    Object_DECREF(tmp);
    Object *headers = Object_Str(HttpResponseObject_HEADERS(self));
    tmp = Object_CallMethod(result, "Concat", headers);
    Object_DECREF(headers);
    Object_DECREF(result);
    result = Object_CallMethod(tmp, "Concat", newline);
    Object_DECREF(tmp);
    tmp = Object_CallMethod(result, "Concat", HttpResponseObject_BODY(self));
    Object_DECREF(result);
    Object_DECREF(line);
    Object_DECREF(newline);
    Object_DECREF(space);
    return tmp;
}

TypeObject HttpResponse_Type = {
    .tp_name = "httprequest",
    .tp_init = httpresponse_init,
    .tp_deinit = httpresponse_deinit,
    .tp_hash = httpresponse_hash,
    .tp_equal = httpresponse_equal,
    .tp_str = httpresponse_str,
    .tp_methods = httpresponse_methods
};

Object *HttpResponseObject_New(Object *status_code, Object *status_text) {
    Object *_response = Object_Malloc(&HttpResponse_Type, sizeof(HttpResponseObject));
    Object *size = IntObject_FromInt(2);
    Object *lst = ListObject_New(size);
    Object_DECREF(size);
    Object_CallMethod(lst, "Append", status_code);
    Object_CallMethod(lst, "Append", status_text);
    Object_Init(_response, lst);
    Object_DECREF(lst);
    return _response;
}
