#include "object.h"
#include "httprequestobject.h"
#include "httpresponseobject.h"
#include "httphandlerobject.h"

static Object *httphandler_method_run(Object *self, Object *args) {
    assert(ListObject_CHECK(args) && ListObject_SIZE(args)==3);
    Object *_i0 = IntObject_FromInt(0);
    Object *handler = Object_CallMethod(args, "Get", _i0);
    Object_DECREF(_i0);
    Object *_i1 = IntObject_FromInt(1);
    Object *request = Object_CallMethod(args, "Get", _i1);
    Object_DECREF(_i1);
    Object *_i2 = IntObject_FromInt(2);
    Object *response = Object_CallMethod(args, "Get", _i2);
    Object_DECREF(_i2);
    Object *_owner = Object_NULL;
    methodfunc http_callback = Object_GetMethod(handler, HttpRequestObject_METHOD(request), &_owner);
    Object *result = Object_NULL;
    if(http_callback) {
        result = http_callback(handler, args);
    } else {
        Object_DECREF(HttpResponseObject_STATUS_CODE(response));
        Object_DECREF(HttpResponseObject_STATUS_TEXT(response));
        HttpResponseObject_STATUS_CODE(response) = StrObject_FromStr("405");
        HttpResponseObject_STATUS_TEXT(response) = StrObject_FromStr("Method Not Allowed");
        result = StrObject_FromStr(METHOD_NOT_ALLOWED_405_CONTENT);
    }
    return result;
}

static MethodDef httphandler_methods[] = {
    {"Run", httphandler_method_run},
    {Object_NULL, Object_NULL}
};

static int httphandler_init(Object *self, Object *args) {
    Object_Extend(self, &Object_Type, sizeof(Object));
    Object_Init(Object_BASE(self), Object_NULL);
    return Object_OK;
}

static int httphandler_deinit(Object *self) {
    return Object_OK;
}

static long httphandler_hash(Object *self) {
    return -1L;
}

static BOOL httphandler_equal(Object *self, Object *ob) {
    return Object_Equal(Object_BASE(self), Object_BASE(ob));
}

static Object *httphandler_str(Object *self) {
    return Object_Str(Object_BASE(self));
}

TypeObject HttpHandler_Type = {
    .tp_name = "httphandler",
    .tp_init = httphandler_init,
    .tp_deinit = httphandler_deinit,
    .tp_hash = httphandler_hash,
    .tp_equal = httphandler_equal,
    .tp_str = httphandler_str,
    .tp_methods = httphandler_methods
};

Object *HttpHandlerObject_New() {
    Object *_handler = Object_Malloc(&HttpHandler_Type, sizeof(HttpHandlerObject));
    Object_Init(_handler, Object_NULL);
    return _handler;
}
