#include "object.h"
#include "httpresponseobject.h"
#include "epollobject.h"

static Object *epoll_method_in_add(Object *self, Object *sock) {
    assert(SockObject_CHECK(sock));
    int fd = fileno(IoObject_AsFILE(Object_BASE(sock)));
    struct epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.ptr = sock;
    epoll_ctl(EpollObject_FD(self), EPOLL_CTL_ADD, fd, &ev);
    return Object_NULL;
}

static Object *epoll_method_out_modify(Object *self, Object *response) {
    assert(HttpResponseObject_CHECK(response));
    Object *sock = HttpResponseObject_SOCK(response);
    int fd = fileno(IoObject_AsFILE(Object_BASE(sock)));
    struct epoll_event ev;
    ev.events = EPOLLOUT;
    ev.data.ptr = response;
    epoll_ctl(EpollObject_FD(self), EPOLL_CTL_MOD, fd, &ev);
    return Object_NULL;
}

static Object *epoll_method_delete(Object *self, Object *sock) {
    assert(SockObject_CHECK(sock));
    int fd = fileno(IoObject_AsFILE(Object_BASE(sock)));
    epoll_ctl(EpollObject_FD(self), EPOLL_CTL_DEL, fd, NULL);
    return Object_NULL;
}

static Object *epoll_method_wait(Object *self, Object *args) {
    int nfds = epoll_wait(EpollObject_FD(self), EpollObject_EVENTS(self), EpollObject_EPSIZE(self), -1);
    if(nfds == -1) {
        fprintf(stderr, "Epoll Wait Fail !!!\n");
        exit(1);
    }
    return IntObject_FromInt(nfds);
}

static MethodDef epoll_methods[] = {
    {"InAdd", epoll_method_in_add},
    {"OutModify", epoll_method_out_modify},
    {"Delete", epoll_method_delete},
    {"Wait", epoll_method_wait},
    {Object_NULL, Object_NULL}
};

static int epoll_init(Object *self, Object *args) {
    assert(IntObject_CHECK(args));
    Object_Extend(self, &Object_Type, sizeof(Object));
    Object_Init(Object_BASE(self), Object_NULL);
    if(IntObject_AsINT(args)) {
        EpollObject_EPSIZE(self) = IntObject_AsINT(args);
    } else {
        EpollObject_EPSIZE(self) = 1024;
    }
    EpollObject_FD(self) = epoll_create(EpollObject_EPSIZE(self));
    if(EpollObject_FD(self) == -1) {
        fprintf(stderr, "Epoll Create Fail !!!\n");
        exit(1);
    }
    EpollObject_EVENTS(self) = malloc(EpollObject_EPSIZE(self) * sizeof(struct epoll_event));
    return Object_OK;
}

static int epoll_deinit(Object *self) {
    free(EpollObject_EVENTS(self));
    return Object_Deinit(Object_BASE(self));
}

static long epoll_hash(Object *self) {
    return -1L;
}

static BOOL epoll_equal(Object *self, Object *ob) {
    return Object_Equal(Object_BASE(self), Object_BASE(ob));
}

static Object *epoll_str(Object *self) {
    return Object_Str(Object_BASE(self));
}

TypeObject Epoll_Type = {
    .tp_name = "epoll",
    .tp_init = epoll_init,
    .tp_deinit = epoll_deinit,
    .tp_hash = epoll_hash,
    .tp_equal = epoll_equal,
    .tp_str = epoll_str,
    .tp_methods = epoll_methods
};

Object *EpollObject_FromSize(ssize_t size) {
    Object *_epoll = Object_Malloc(&Epoll_Type, sizeof(EpollObject));
    Object *_size = IntObject_FromInt(size);
    Object_Init(_epoll, _size);
    Object_DECREF(_size);
    return _epoll;
}

