#include "object.h"

int main(int argc, char *args[]) {
    Object *ip = StrObject_FromStr("14.215.177.37");
    Object *port = IntObject_FromInt(80);
    Object *sock = SockObject_Open(ip, port);
    Object_CallMethod(sock, "Connect", 0);
    printf("---------1\n");
    Object *req = StrObject_FromStr("GET / HTTP/1.1\r\nHost: baidu.com\r\nConnection: close\r\n\r\n");
    Object_CallMethod(sock, "Writeline", req);
    Object *out = StdoutObject_New();
    Object_CallMethod(out, "Writeline", req);
    Object *size = IntObject_FromInt(1024 * 16);
    Object *s = Object_CallMethod(sock, "Read", size);
    Object_CallMethod(out, "Writeline", s);
    Object_DECREF(size);
    Object_DECREF(req);
    Object_DECREF(s);
    Object_DECREF(out);
    Object_DECREF(ip);
    Object_DECREF(port);
    Object_DECREF(sock);
    return 0;
}


