#include "object.h"
#include <sys/socket.h>
#include <netinet/in.h>

static Object *sock_method_connect(Object *self, Object *args) {
    Object *ip = SockObject_IP(self);
    Object *port = SockObject_PORT(self);
    const char *_ip = StrObject_AsSTR(ip);
    int _port = IntObject_AsINT(port);
    printf("IP : %s, PORT : %d \n", _ip, _port);
    struct sockaddr_in ipaddr;
    memset(&ipaddr, 0, sizeof(ipaddr));
    ipaddr.sin_family = AF_INET;
    ipaddr.sin_port = htons(_port);
    ipaddr.sin_addr.s_addr = inet_addr(_ip);
    int sock = fileno(IoObject_AsFILE(Object_BASE(self)));
    if(connect(sock, (struct sockaddr *)&ipaddr, sizeof(ipaddr)) < 0) {
        fprintf(stderr, "Socket Connect Fail !!!");
        exit(1);
    }
    return Object_NULL;
}

static Object *sock_method_listen(Object *self, Object *args) {
    return Object_NULL;
}

static Object *sock_method_accept(Object *self, Object *args) {
    return Object_NULL;
}

static MethodDef sock_methods[] = {
    {"Connect", sock_method_connect},
    {"Listen", sock_method_listen},
    {"Accept", sock_method_accept},
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
    int sock = socket(AF_INET,SOCK_STREAM, 0);
    FILE *fsock = fdopen(sock, "r+");
    Object_Init(Object_BASE(self), Object_CONVERT(fsock));
    return Object_OK;
}

static int sock_deinit(Object *self) {
    printf("sock_deinit...\n");
    Object_DECREF(SockObject_IP(self));
    printf("sock_deinit...ip\n");
    Object_DECREF(SockObject_PORT(self));
    printf("sock_deinit...port\n");
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
    Object *_lst= ListObject_New(2);
    Object_CallMethod(_lst, "Append", ip);
    Object_CallMethod(_lst, "Append", port);
    Object_Init(_sock, _lst);
    Object_DECREF(_lst);
    return _sock;
}

