#include "object.h"
#include "keyvalueobject.h"
#include "httpheaderobject.h"
#include "httprequestobject.h"

static MethodDef httprequest_methods[] = {
    {Object_NULL, Object_NULL}
};

static int httprequest_init(Object *self, Object *args) {
    assert(StrObject_CHECK(args));
    Object_Extend(self, &Object_Type, sizeof(Object));
    Object_Init(Object_BASE(self), Object_NULL);
    HttpRequestObject_ARGUMENTS(self) = KeyValueObject_New();
    Object *size = IntObject_FromInt(2);
    Object *lst = ListObject_New(size);
    Object *start = IntObject_FromInt(0);
    Object *newline = StrObject_FromStr("\r\n");
    Object *idx = Object_CallMethod(args, "Index", newline);
    Object_DECREF(newline);
    Object_CallMethod(lst, "Append", start);
    Object_CallMethod(lst, "Append", idx);
    Object *req_method = Object_CallMethod(args, "Substr", lst);
    Object *space = StrObject_FromStr(" ");
    Object *params = Object_CallMethod(req_method, "Split", space);
    Object_DECREF(space);
    Object_DECREF(req_method);
    Object *_i0 = IntObject_FromInt(0);
    Object *_i1 = IntObject_FromInt(1);
    Object *_i2 = IntObject_FromInt(2);
    HttpRequestObject_METHOD(self) = Object_CallMethod(params, "Get", _i0);
    Object_INCREF(HttpRequestObject_METHOD(self));
    Object *url_args = Object_CallMethod(params, "Get", _i1);
    Object *question = StrObject_FromStr("?");
    url_args = Object_CallMethod(url_args, "Split", question);
    Object_DECREF(question);
    HttpRequestObject_URL(self) = Object_CallMethod(url_args, "Get", _i0);
    Object_INCREF(HttpRequestObject_URL(self));
    Object *and = StrObject_FromStr("&");
    Object *eq = StrObject_FromStr("=");
    Object *arg_lst = Object_CallMethod(url_args, "Get", _i1);
    arg_lst = Object_CallMethod(arg_lst, "Split", and);
    Object *arg_item = Object_NULL;
    Object *index = Object_NULL;
    Object *empty = StrObject_FromStr("");
    int i = 0;
    if(ListObject_SIZE(arg_lst)) {
        for(; i < ListObject_SIZE(arg_lst); i++) {
            index = IntObject_FromInt(i);
            arg_item = Object_CallMethod(arg_lst, "Get", index);
            arg_item = Object_CallMethod(arg_item, "Split", eq);
            if(ListObject_SIZE(arg_item) == 1) {
                Object_CallMethod(arg_item, "Append", empty);
                Object_CallMethod(HttpRequestObject_ARGUMENTS(self), "Add", arg_item);
            } else if(ListObject_SIZE(arg_item) == 2) {
                Object_CallMethod(HttpRequestObject_ARGUMENTS(self), "Add", arg_item);
            }
            Object_DECREF(arg_item);
            Object_DECREF(index);
        }
    }
    Object_DECREF(empty);
    Object_DECREF(arg_lst);
    Object_DECREF(eq);
    Object_DECREF(and);
    Object_DECREF(url_args);
    Object *protocol_version = Object_CallMethod(params, "Get", _i2);
    Object *line = StrObject_FromStr("/");
    Object *pv_lst = Object_CallMethod(protocol_version, "Split", line);
    Object_DECREF(line);
    HttpRequestObject_PROTOCOL(self) = Object_CallMethod(pv_lst, "Get", _i0);
    Object_INCREF(HttpRequestObject_PROTOCOL(self));
    HttpRequestObject_VERSION(self) = Object_CallMethod(pv_lst, "Get", _i1);
    Object_INCREF(HttpRequestObject_VERSION(self));
    Object_DECREF(pv_lst);
    Object_DECREF(params);
    Object_DECREF(start);
    Object_DECREF(idx);
    start = Object_CallMethod(idx, "Add", size);
    Object *new2line = StrObject_FromStr("\r\n\r\n");
    idx = Object_CallMethod(args, "Index", new2line);
    Object_DECREF(new2line);
    Object_CallMethod(lst, "Clear", Object_NULL);
    Object_CallMethod(lst, "Append", start);
    Object_CallMethod(lst, "Append", idx);
    Object *substr = Object_CallMethod(args, "Substr", lst);
    HttpRequestObject_HEADERS(self) = HttpHeaderObject_New(substr);
    Object_DECREF(substr);
    Object_DECREF(start);
    Object *_i4 = IntObject_FromInt(4);
    start = Object_CallMethod(idx, "Add", _i4);
    Object_DECREF(idx);
    Object_DECREF(_i4);
    HttpRequestObject_BODY(self) = Object_CallMethod(args, "Substr", start);
    Object_DECREF(start);
    Object_DECREF(size);
    Object_DECREF(lst);
    Object_DECREF(_i0);
    Object_DECREF(_i1);
    Object_DECREF(_i2);

    Object *out = StdoutObject_New();
    Object_CallMethod(out, "Writeline", HttpRequestObject_ARGUMENTS(self));
    Object_DECREF(out);
    return Object_OK;
}

static int httprequest_deinit(Object *self) {
    Object_DECREF(HttpRequestObject_METHOD(self));
    Object_DECREF(HttpRequestObject_URL(self));
    Object_DECREF(HttpRequestObject_PROTOCOL(self));
    Object_DECREF(HttpRequestObject_VERSION(self));
    Object_DECREF(HttpRequestObject_HEADERS(self));
    Object_DECREF(HttpRequestObject_ARGUMENTS(self));
    Object_DECREF(HttpRequestObject_BODY(self));
    return Object_Deinit(Object_BASE(self));
}

static long httprequest_hash(Object *self) {
    return -1L;
}

static BOOL httprequest_equal(Object *self, Object *ob) {
    return Object_Equal(Object_BASE(self), Object_BASE(ob));
}

static Object *httprequest_str(Object *self) {
    return Object_Str(Object_BASE(self));
}

TypeObject HttpRequest_Type = {
    .tp_name = "httprequest",
    .tp_init = httprequest_init,
    .tp_deinit = httprequest_deinit,
    .tp_hash = httprequest_hash,
    .tp_equal = httprequest_equal,
    .tp_str = httprequest_str,
    .tp_methods = httprequest_methods
};

Object *HttpRequestObject_New(Object *sock) {
    Object *_request = Object_Malloc(&HttpRequest_Type, sizeof(HttpRequestObject));
    Object_Init(_request, sock);
    return _request;
}

