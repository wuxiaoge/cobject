#include <sys/epoll.h>
#ifndef EPOLL_OBJECT_H
#define EPOLL_OBJECT_H
#ifdef __cplusplus
extern "C" {
#endif

typedef struct _epollobject {
    Object_HEAD
    int ob_epfd;
    ssize_t ob_epsize;
    struct epoll_event *ob_events;
} EpollObject;

extern TypeObject Epoll_Type;

#define EpollObject_CHECK(ob) Object_CHECK(ob, &Epoll_Type)
#define EpollObject_CONVERT(ob) ((EpollObject *)(ob))
#define EpollObject_FD(ob) EpollObject_CONVERT(ob)->ob_epfd
#define EpollObject_EPSIZE(ob) EpollObject_CONVERT(ob)->ob_epsize
#define EpollObject_EVENTS(ob) EpollObject_CONVERT(ob)->ob_events

Object *EpollObject_FromSize(ssize_t);

#ifdef __cplusplus
}
#endif
#endif
