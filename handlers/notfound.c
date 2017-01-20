#include "object.h"
#include "httprequestobject.h"
#include "httpresponseobject.h"
#include "httphandlerobject.h"
#include "notfound.h"

static Object *notfound_method_call(Object *self, Object *args) {
    return StrObject_FromStr("404 Not Found");
}

static MethodDef notfound_methods[] = {
    {"GET", notfound_method_call},
    {"POST", notfound_method_call},
    {"PUT", notfound_method_call},
    {"DELETE", notfound_method_call},
    {"HEAD", notfound_method_call},
    {"OPTIONS", notfound_method_call},
    {"TARCE", notfound_method_call},
    {"CONNECT", notfound_method_call},
    {Object_NULL, Object_NULL}
};

static int notfound_init(Object *self, Object *args) {
    Object_Extend(self, &HttpHandler_Type, sizeof(HttpHandlerObject));
    Object_Init(Object_BASE(self), Object_NULL);
    return Object_OK;
}

static int notfound_deinit(Object *self) {
    return Object_Deinit(Object_BASE(self));
}

static long notfound_hash(Object *self) {
    return -1L;
}

static BOOL notfound_equal(Object *self, Object *ob) {
    return Object_Equal(Object_BASE(self), Object_BASE(ob));
}

static Object *notfound_str(Object *self) {
    return Object_Str(Object_BASE(self));
}

TypeObject NotFoundHandler_Type = {
    .tp_name = "notfound",
    .tp_init = notfound_init,
    .tp_deinit = notfound_deinit,
    .tp_hash = notfound_hash,
    .tp_equal = notfound_equal,
    .tp_str = notfound_str,
    .tp_methods = notfound_methods
};

Object *NotFoundHandler_New() {
    Object *_handler = Object_Malloc(&NotFoundHandler_Type, sizeof(NotFoundHandler));
    Object_Init(_handler, Object_NULL);
    return _handler;
}
