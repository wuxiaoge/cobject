#ifndef SOCT_OBJECT_H
#define SOCT_OBJECT_H
#ifdef __cplusplus
extern "C" {
#endif

typedef struct _sockobject {
    Object_HEAD
    Object *ip;
    Object *port;
} SockObject;

extern TypeObject Sock_Type;

#define SockObject_CHECK(ob) Object_CHECK(ob, &Sock_Type)
#define SockObject_CONVERT(ob) ((SockObject *)ob)
#define SockObject_IP(ob) SockObject_CONVERT(ob)->ip
#define SockObject_PORT(ob) SockObject_CONVERT(ob)->port

Object *SockObject_Open(Object *, Object *);

#ifdef __cplusplus
}
#endif
#endif
