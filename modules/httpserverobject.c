#include "object.h"
#include "httprequestobject.h"
#include "httpresponseobject.h"
#include "httpserverobject.h"
#include "epollobject.h"

static Object *httpserver_method_start(Object *self, Object *args) {
    assert(IntObject_CHECK(args));
    Object_CallMethod(HttpServerObject_SOCK(self), "Listen", args);
    Object *sock = Object_NULL;
    Object *content = Object_NULL;
    Object *request = Object_NULL;
    Object *response = Object_NULL;
    Object *size = IntObject_FromInt(1024);
    Object *status_code = StrObject_FromStr("200");
    Object *status_text = StrObject_FromStr("OK");
    Object *epsize = Object_NULL;
    Object *epoll = HttpServerObject_EPOLL(self);
    Object_CallMethod(epoll, "InAdd", HttpServerObject_SOCK(self));
    struct epoll_event *event = NULL;
    int iepsize = 0, i;
    response = HttpResponseObject_New(status_code, status_text);
    while(TRUE) {
        epsize = Object_CallMethod(epoll, "Wait", Object_NULL);
        iepsize = IntObject_AsINT(epsize);
        Object_DECREF(epsize);
        for(i = 0; i < iepsize; i++) {
            event = (struct epoll_event *)(EpollObject_EVENTS(epoll) + i);
            if(HttpServerObject_SOCK(self) == event->data.ptr) {
                sock = Object_CallMethod(HttpServerObject_SOCK(self), "Accept", Object_NULL);
                Object_CallMethod(epoll, "InAdd", sock);
            }else if(event->events & EPOLLIN) {
                sock = Object_CONVERT(event->data.ptr);
                event->data.ptr = NULL;
                content = Object_CallMethod(sock, "Read", size);
                if(content && StrObject_SIZE(content)) {
                    request = HttpRequestObject_New(content);
                    Object_DECREF(request);
                }
                Object_DECREF(content);
                Object_CallMethod(epoll, "OutModify", sock);
            }else if(event->events & EPOLLOUT) {
                sock = Object_CONVERT(event->data.ptr);
                event->data.ptr = NULL;
                Object_CallMethod(sock, "Write", response);
                Object_CallMethod(epoll, "Delete", sock);
                Object_DECREF(sock);
            }
        }
    }
    Object_DECREF(response);
    Object_DECREF(size);
    Object_DECREF(status_code);
    Object_DECREF(status_text);
    return Object_NULL;
}

static MethodDef httpserver_methods[] = {
    {"Start", httpserver_method_start},
    {Object_NULL, Object_NULL}
};

static int httpserver_init(Object *self, Object *args) {
    assert(ListObject_CHECK(args) && ListObject_SIZE(args)==2);
    Object_Extend(self, &Object_Type, sizeof(Object));
    Object_Init(Object_BASE(self), Object_NULL);
    Object *_i0 = IntObject_FromInt(0);
    Object *_i1 = IntObject_FromInt(1);
    Object *ip = Object_CallMethod(args, "Get", _i0);
    Object *port = Object_CallMethod(args, "Get", _i1);
    Object_DECREF(_i0);
    Object_DECREF(_i1);
    HttpServerObject_SOCK(self) = SockObject_Open(ip, port);
    Object *size = IntObject_FromInt(8);
    HttpServerObject_HANDERS(self) = ListObject_New(size);
    Object_DECREF(size);
    Object_CallMethod(HttpServerObject_SOCK(self), "SetNonblocking", Object_NULL);
    HttpServerObject_EPOLL(self) = EpollObject_FromSize(1024);
    return Object_OK;
}

static int httpserver_deinit(Object *self) {
    Object_DECREF(HttpServerObject_EPOLL(self));
    Object_DECREF(HttpServerObject_SOCK(self));
    Object_DECREF(HttpServerObject_HANDERS(self));
    return Object_Deinit(Object_BASE(self));
}

static long httpserver_hash(Object *self) {
    return -1L;
}

static BOOL httpserver_equal(Object *self, Object *ob) {
    return Object_Equal(Object_BASE(self), Object_BASE(ob));
}

static Object *httpserver_str(Object *self) {
    return Object_Str(Object_BASE(self));
}

TypeObject HttpServer_Type = {
    .tp_name = "httpserver",
    .tp_init = httpserver_init,
    .tp_deinit = httpserver_deinit,
    .tp_hash = httpserver_hash,
    .tp_equal = httpserver_equal,
    .tp_str = httpserver_str,
    .tp_methods = httpserver_methods
};

Object *HttpServerObject_New(Object *ip, Object *port) {
    Object *_server = Object_Malloc(&HttpServer_Type, sizeof(HttpServerObject));
    Object *size = IntObject_FromInt(2);
    Object *lst = ListObject_New(size);
    Object_CallMethod(lst, "Append", ip);
    Object_CallMethod(lst, "Append", port);
    Object_DECREF(size);
    Object_Init(_server, lst);
    Object_DECREF(lst);
    return _server;
}
