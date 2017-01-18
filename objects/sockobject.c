#include "object.h"
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

static Object *sock_method_connect(Object *self, Object *args) {
    Object *ip = SockObject_IP(self);
    Object *port = SockObject_PORT(self);
    const char *_ip = StrObject_AsSTR(ip);
    int _port = IntObject_AsINT(port);
    struct sockaddr_in ipaddr;
    memset(&ipaddr, 0, sizeof(ipaddr));
    ipaddr.sin_family = AF_INET;
    ipaddr.sin_port = htons(_port);
    ipaddr.sin_addr.s_addr = inet_addr(_ip);
    int sock = fileno(IoObject_AsFILE(Object_BASE(self)));
    if(connect(sock, (struct sockaddr *)&ipaddr, sizeof(ipaddr)) < 0) {
        fprintf(stderr, "Socket Connect Fail !!!\n");
        exit(1);
    }
    return Object_NULL;
}

static Object *sock_method_listen(Object *self, Object *args) {
    assert(args && IntObject_CHECK(args));
    Object *ip = SockObject_IP(self);
    Object *port = SockObject_PORT(self);
    const char *_ip = StrObject_AsSTR(ip);
    int _port = IntObject_AsINT(port);
    struct sockaddr_in ipaddr;
    memset(&ipaddr, 0, sizeof(ipaddr));
    ipaddr.sin_family = AF_INET;
    ipaddr.sin_port = htons(_port);
    ipaddr.sin_addr.s_addr = inet_addr(_ip);
    int sock = fileno(IoObject_AsFILE(Object_BASE(self)));
    int on = 1;
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
    int res = bind(sock, (struct sockaddr *)&ipaddr, sizeof(ipaddr));
    if(res < 0) {
        fprintf(stderr, "Socket Bind Fail !!!\n");
        exit(1);
    }
    int queue_size = IntObject_AsINT(args);
    if(listen(sock, queue_size ? queue_size : 1024) < 0) {
        fprintf(stderr, "Socket Listen Fail !!!\n");
        exit(1);
    }
    return Object_NULL;
}

static Object *sock_method_accept(Object *self, Object *args) {
    int sock = fileno(IoObject_AsFILE(Object_BASE(self)));
    struct sockaddr_in client_addr;
    socklen_t length = sizeof(client_addr);
    int conn = accept(sock, (struct sockaddr *)&client_addr, &length);
    int nZero = 0;
    setsockopt(conn, SOL_SOCKET, SO_RCVBUF, &nZero, sizeof(nZero));
    setsockopt(conn, SOL_SOCKET, SO_SNDBUF, &nZero, sizeof(nZero));
    if(conn < 0) {
        fprintf(stderr, "Socket Accept Fail !!!\n");
        exit(1);
    }
    FILE *fsio = fdopen(conn, "r+");
    Object *sio = Object_Malloc(&Sock_Type, sizeof(SockObject));
    Object_Extend(sio, &Io_Type, sizeof(IoObject));
    Object_Init(Object_BASE(sio), Object_CONVERT(fsio));
    SockObject_IP(sio) = StrObject_FromStr(inet_ntoa(client_addr.sin_addr));
    SockObject_PORT(sio) = IntObject_FromInt(client_addr.sin_port);
    return sio;
}

static Object *sock_method_read(Object *self, Object *args) {
    assert(IntObject_CHECK(args));
    int size = IntObject_AsINT(args);
    char *buf = (char *)malloc(size);
    int sock = fileno(IoObject_AsFILE(Object_BASE(self)));
    int rsize = recv(sock, buf, size, 0);
    Object *_s = Object_NULL;
    if(rsize > 0) {
        _s = StrObject_FromStrAndSize(buf, rsize);
    }
    free(buf);
    return _s;
}

static Object *sock_method_write(Object *self, Object *args) {
    int sock = fileno(IoObject_AsFILE(Object_BASE(self)));
    Object *_s = Object_Str(args);
    send(sock, StrObject_AsSTR(_s), StrObject_SIZE(_s), 0);
    Object_DECREF(_s);
    return Object_NULL;
}

static Object *sock_method_setnonblocking(Object *self, Object *args) {
    int fd = fileno(IoObject_AsFILE(Object_BASE(self)));
    int opts = fcntl(fd, F_GETFL);
    if(opts < 0) {
        fprintf(stderr, "Get FL Fail !!!\n");
        exit(1);
    }
    opts = opts | O_NONBLOCK;
    if(fcntl(fd, F_SETFL, opts) < 0) {
        fprintf(stderr, "Set FL Fail !!!\n");
        exit(1);
    }
    return Object_NULL;
}

static MethodDef sock_methods[] = {
    {"Connect", sock_method_connect},
    {"Listen", sock_method_listen},
    {"Accept", sock_method_accept},
    {"Read", sock_method_read},
    {"Write", sock_method_write},
    {"SetNonblocking", sock_method_setnonblocking},
    {Object_NULL, Object_NULL}
};

static int sock_init(Object *self, Object *args) {
    assert(ListObject_CHECK(args) && ListObject_SIZE(args)==2);
    Object_Extend(self, &Io_Type, sizeof(IoObject));
    Object *_i0 = IntObject_FromInt(0);
    Object *ip = Object_CallMethod(args, "Get", _i0);
    Object_DECREF(_i0);
    Object *_i1 = IntObject_FromInt(1);
    Object *port = Object_CallMethod(args, "Get", _i1);
    Object_DECREF(_i1);
    SockObject_IP(self) = ip;
    Object_INCREF(ip);
    SockObject_PORT(self) = port;
    Object_INCREF(port);
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    FILE *fsock = fdopen(sock, "r+");
    Object_Init(Object_BASE(self), Object_CONVERT(fsock));
    return Object_OK;
}

static int sock_deinit(Object *self) {
    Object_DECREF(SockObject_IP(self));
    Object_DECREF(SockObject_PORT(self));
    return Object_Deinit(Object_BASE(self));
}

static long sock_hash(Object *self) {
    return -1L;
}

static BOOL sock_equal(Object *self, Object *ob) {
    return Object_Equal(Object_BASE(self), Object_BASE(ob));
}

static Object *sock_str(Object *self) {
    return Object_Str(Object_BASE(self));
}

TypeObject Sock_Type = {
    .tp_name = "sock",
    .tp_init = sock_init,
    .tp_deinit = sock_deinit,
    .tp_hash = sock_hash,
    .tp_equal = sock_equal,
    .tp_str = sock_str,
    .tp_methods = sock_methods
};

Object *SockObject_Open(Object *ip, Object *port) {
    Object *_sock = Object_Malloc(&Sock_Type, sizeof(SockObject));
    Object *_size = IntObject_FromInt(2);
    Object *_lst= ListObject_New(_size);
    Object_DECREF(_size);
    Object_CallMethod(_lst, "Append", ip);
    Object_CallMethod(_lst, "Append", port);
    Object_Init(_sock, _lst);
    Object_DECREF(_lst);
    return _sock;
}

