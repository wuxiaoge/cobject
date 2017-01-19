#include "object.h"
#include "httprequestobject.h"
#include "httpresponseobject.h"
#include "httpserverobject.h"
#include "epollobject.h"

static Object *httpserver_method_gethandler(Object *self, Object *url) {
    assert(StrObject_CHECK(url) && StrObject_SIZE(url) > 0);
    struct url_handler *_url_handler = HttpServerObject_HANDERS(self);
    while(_url_handler) {
        if(Object_Equal(url, _url_handler->url)) {
            return _url_handler->handler;
        }
        _url_handler = _url_handler->next;
    }
    return Object_NULL;
}

static Object *httpserver_method_addhandler(Object *self, Object *handler) {
    assert(ListObject_CHECK(handler) && ListObject_SIZE(handler)==2);
    struct url_handler *_url_handler = (struct url_handler *)malloc(sizeof(struct url_handler));
    struct url_handler *tmp = NULL;
    Object *_i0 = IntObject_FromInt(0);
    _url_handler->url = Object_CallMethod(handler, "Get", _i0);
    Object_INCREF(_url_handler->url);
    Object_DECREF(_i0);
    Object *_i1 = IntObject_FromInt(1);
    _url_handler->handler = Object_CallMethod(handler, "Get", _i1);
    Object_INCREF(_url_handler->handler);
    Object_DECREF(_i1);
    if(HttpServerObject_HANDERS(self)) {
        tmp = HttpServerObject_HANDERS(self);
        _url_handler->next = tmp;
        HttpServerObject_HANDERS(self) = _url_handler;
    } else {
        _url_handler->next = NULL;
        HttpServerObject_HANDERS(self) = _url_handler;
    }
    return Object_NULL;
}

static Object *build_response(Object *self, Object *request) {
    Object *status_code = StrObject_FromStr("200");
    Object *status_text = StrObject_FromStr("OK");
    Object *size = IntObject_FromInt(2);
    Object *body = Object_NULL;
    Object *response = Object_NULL;
    Object *params = Object_NULL;
    Object *handler = httpserver_method_gethandler(self,
        HttpRequestObject_URL(request));
    if(handler) {
        response = HttpResponseObject_New(status_code, status_text);
        params = ListObject_New(size);
        Object_CallMethod(params, "Append", request);
        Object_CallMethod(params, "Append", response);
        body = Object_CallMethod(handler, "Run", params);
        Object_DECREF(params);
    } else {
        Object_DECREF(status_code);
        Object_DECREF(status_text);
        status_code = StrObject_FromStr("404");
        status_text = StrObject_FromStr("Not Found");
        response = HttpResponseObject_New(status_code, status_text);
        Object_DECREF(HttpResponseObject_BODY(response));
        body = StrObject_FromStr("404 Not Found");
    }
    Object_CallMethod(response, "SetBody", body);
    Object_DECREF(body);
    Object_DECREF(size);
    Object_DECREF(status_code);
    Object_DECREF(status_text);
    return response;
}

static void httpserver_request_log(Object *request, Object *response, Object *ip, struct timeval start) {
    struct timeval now_tv;
    gettimeofday(&now_tv, NULL);
    long request_ts = ((long)now_tv.tv_sec * 1000 + (long)now_tv.tv_usec / 1000) - ((long)start.tv_sec * 1000 + (long)start.tv_usec / 1000);
    char *s = ctime(&start.tv_sec);
    Object *ob_now = StrObject_FromStrAndSize(s, strlen(s) - 1);
    const char *now = StrObject_AsSTR(ob_now);
    const char *url = StrObject_AsSTR(HttpRequestObject_URL(request));
    const char *method = StrObject_AsSTR(HttpRequestObject_METHOD(request));
    const char *scode = StrObject_AsSTR(HttpResponseObject_STATUS_CODE(response));
    ssize_t byte = StrObject_SIZE(HttpResponseObject_BODY(response));
    Object *query = HttpRequestObject_QUERYSTR(request);
    printf("%s [%s] %s %s%s%s generated %ld bytes in %ld msecs (HTTP/1.1 %s) \n",
        StrObject_AsSTR(ip), now, method, url, (query ? "?" : ""),
        (query ? StrObject_AsSTR(query) : ""), byte, request_ts, scode);
    Object_DECREF(ob_now);
}

static Object *httpserver_method_start(Object *self, Object *args) {
    assert(IntObject_CHECK(args));
    Object_CallMethod(HttpServerObject_SOCK(self), "Listen", args);
    Object *sock = Object_NULL;
    Object *content = Object_NULL;
    Object *request = Object_NULL;
    Object *response = Object_NULL;
    Object *size = IntObject_FromInt(1024);
    Object *epsize = Object_NULL;
    Object *epoll = HttpServerObject_EPOLL(self);
    Object_CallMethod(epoll, "InAdd", HttpServerObject_SOCK(self));
    struct epoll_event *event = NULL;
    struct timeval start;
    int iepsize = 0, i;
    Object *read_content = Object_NULL;
    Object *tmp = Object_NULL;
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
                content = StrObject_FromStr("");
                do {
                    read_content = Object_CallMethod(sock, "Read", size);
                    tmp = Object_CallMethod(content, "Concat", read_content);
                    Object_DECREF(content);
                    Object_DECREF(read_content);
                    content = tmp;
                } while(content && StrObject_SIZE(content) == IntObject_AsINT(size));
                if(content && StrObject_SIZE(content)) {
                    gettimeofday(&start, NULL);
                    request = HttpRequestObject_New(content);
                    response = build_response(self, request);
                    Object_CallMethod(sock, "Write", response);
                    httpserver_request_log(request, response, SockObject_IP(sock), start);
                    Object_DECREF(request);
                    Object_DECREF(response);
                }
                Object_DECREF(content);
                Object_CallMethod(epoll, "Delete", sock);
                Object_DECREF(sock);
            }
        }
    }
    Object_DECREF(size);
    return Object_NULL;
}

static MethodDef httpserver_methods[] = {
    {"Start", httpserver_method_start},
    {"AddHandler", httpserver_method_addhandler},
    {"GetHandler", httpserver_method_gethandler},
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
    HttpServerObject_HANDERS(self) = NULL;
    Object_DECREF(size);
    Object_CallMethod(HttpServerObject_SOCK(self), "SetNonblocking", Object_NULL);
    HttpServerObject_EPOLL(self) = EpollObject_FromSize(1024);
    return Object_OK;
}

static int httpserver_deinit(Object *self) {
    Object_DECREF(HttpServerObject_EPOLL(self));
    Object_DECREF(HttpServerObject_SOCK(self));
    struct url_handler *_url_handler = HttpServerObject_HANDERS(self);
    struct url_handler * tmp = NULL;
    while(_url_handler) {
        tmp = _url_handler->next;
        Object_DECREF(_url_handler->url);
        Object_DECREF(_url_handler->handler);
        free(_url_handler);
        _url_handler = tmp;
    }
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
