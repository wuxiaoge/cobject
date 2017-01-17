#include "object.h"
#include "keyvalueobject.h"
#include "httpheaderobject.h"
#include "httprequestobject.h"

static MethodDef httprequest_methods[] = {
    {Object_NULL, Object_NULL}
};

static Object *httprequest_build_request_argument(Object *self, Object *arguments) {
    assert(arguments && StrObject_CHECK(arguments));
    Object *and = StrObject_FromStr("&");
    Object *eq = StrObject_FromStr("=");
    Object *empty = StrObject_FromStr("");
    Object *arg_lst = Object_CallMethod(arguments, "Split", and);
    Object *arg_item = Object_NULL;
    Object *index = Object_NULL;
    int i = 0;
    for(; i < ListObject_SIZE(arg_lst); i++) {
        index = IntObject_FromInt(i);
        arg_item = Object_CallMethod(Object_CallMethod(arg_lst, "Get", index), "Split", eq);
        if(ListObject_SIZE(arg_item)==1) {
            Object_CallMethod(arg_item, "Append", empty);
            Object_CallMethod(HttpRequestObject_ARGUMENTS(self), "Add", arg_item);
        } else if(ListObject_SIZE(arg_item)==2) {
            Object_CallMethod(HttpRequestObject_ARGUMENTS(self), "Add", arg_item);
        }
        Object_DECREF(index);
        Object_DECREF(arg_item);
    }
    Object_DECREF(and);
    Object_DECREF(eq);
    Object_DECREF(empty);
    Object_DECREF(arg_lst);
    return Object_NULL;
}

static Object *httprequest_build_request_method(Object *self, Object *content) {
    Object *newline = StrObject_FromStr("\r\n");
    Object *start = IntObject_FromInt(0);
    Object *idx = Object_CallMethod(content, "Index", newline);
    Object *size = IntObject_FromInt(2);
    Object *lst = ListObject_New(size);
    Object_CallMethod(lst, "Append", start);
    Object_CallMethod(lst, "Append", idx);
    Object *substr = Object_CallMethod(content, "Substr", lst);
    Object_DECREF(newline);
    Object_DECREF(lst);
    Object_DECREF(start);
    start = Object_CallMethod(idx, "Add", size);
    Object_DECREF(idx);
    Object_DECREF(size);
    Object *space = StrObject_FromStr(" ");
    Object *_i0 = IntObject_FromInt(0);
    Object *_i1 = IntObject_FromInt(1);
    Object *_i2 = IntObject_FromInt(2);
    lst = Object_CallMethod(substr, "Split", space);
    Object_DECREF(substr);
    Object_DECREF(space);
    // 获取请求方式
    HttpRequestObject_METHOD(self) = Object_CallMethod(lst, "Get", _i0);
    Object_INCREF(HttpRequestObject_METHOD(self));
    // 获取请求URL
    Object *qst = StrObject_FromStr("?");
    Object *url_args = Object_CallMethod(Object_CallMethod(lst, "Get", _i1), "Split", qst);
    HttpRequestObject_URL(self) = Object_CallMethod(url_args, "Get", _i0);
    Object_INCREF(HttpRequestObject_URL(self));
    HttpRequestObject_ARGUMENTS(self) = KeyValueObject_New();
    httprequest_build_request_argument(self, Object_CallMethod(url_args, "Get", _i1));
    Object_DECREF(url_args);
    Object_DECREF(qst);
    // 获取协议与版本
    Object *line = StrObject_FromStr("/");
    Object *pv_lst = Object_CallMethod(Object_CallMethod(lst, "Get", _i2), "Split", line);
    HttpRequestObject_PROTOCOL(self) = Object_CallMethod(pv_lst, "Get", _i0);
    Object_INCREF(HttpRequestObject_PROTOCOL(self));
    HttpRequestObject_VERSION(self) = Object_CallMethod(pv_lst, "Get", _i1);
    Object_INCREF(HttpRequestObject_VERSION(self));
    Object_DECREF(pv_lst);
    Object_DECREF(line);

    Object_DECREF(lst);
    Object_DECREF(_i0);
    Object_DECREF(_i1);
    Object_DECREF(_i2);
    return start;
}

static Object *httprequest_build_request_header(Object *self, Object *start, Object *content) {
    Object *size = IntObject_FromInt(2);
    Object *len = IntObject_FromInt(4);
    Object *new2line = StrObject_FromStr("\r\n\r\n");
    Object *idx = Object_CallMethod(content, "Index", new2line);
    Object_DECREF(new2line);
    Object *lst = ListObject_New(size);
    Object_DECREF(size);
    Object_CallMethod(lst, "Append", start);
    Object_CallMethod(lst, "Append", idx);
    Object *substr = Object_CallMethod(content, "Substr", lst);
    HttpRequestObject_HEADERS(self) = HttpHeaderObject_New(substr);
    start = Object_CallMethod(idx, "Add", len);
    Object_DECREF(idx);
    Object_DECREF(len);
    Object_DECREF(substr);
    Object_DECREF(lst);
    return start;
}

static Object *httprequest_build_request_body(Object *self, Object *start, Object *content) {
    HttpRequestObject_BODY(self) = Object_CallMethod(content, "Substr", start);
    Object *content_type = StrObject_FromStr("application/x-www-form-urlencoded");
    Object *key = StrObject_FromStr("Content-Type");
    Object *value = Object_CallMethod(HttpRequestObject_HEADERS(self), "Get", key);
    if(Object_Equal(content_type, value)) {
        httprequest_build_request_argument(self, HttpRequestObject_BODY(self));
    }
    Object_DECREF(content_type);
    Object_DECREF(key);
    return Object_NULL;
}

static int httprequest_init(Object *self, Object *args) {
    assert(StrObject_CHECK(args) && StrObject_SIZE(args));
    Object_Extend(self, &Object_Type, sizeof(Object));
    Object_Init(Object_BASE(self), Object_NULL);
    Object *start = httprequest_build_request_method(self, args);
    Object *start2  = httprequest_build_request_header(self, start, args);
    httprequest_build_request_body(self, start2, args);
    Object_DECREF(start);
    Object_DECREF(start2);
    Object *out = StdoutObject_New();
    Object_CallMethod(out, "Writeline", HttpRequestObject_METHOD(self));
    Object_CallMethod(out, "Writeline", HttpRequestObject_URL(self));
    Object_CallMethod(out, "Writeline", HttpRequestObject_ARGUMENTS(self));
    Object_CallMethod(out, "Writeline", HttpRequestObject_PROTOCOL(self));
    Object_CallMethod(out, "Writeline", HttpRequestObject_VERSION(self));
    Object_CallMethod(out, "Writeline", HttpRequestObject_HEADERS(self));
    Object_CallMethod(out, "Writeline", HttpRequestObject_BODY(self));
    Object *key = StrObject_FromStr("Content-Type");
    Object_CallMethod(out, "Writeline", key);
    Object_DECREF(key);
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

Object *HttpRequestObject_New(Object *content) {
    Object *_request = Object_Malloc(&HttpRequest_Type, sizeof(HttpRequestObject));
    Object_Init(_request, content);
    return _request;
}

