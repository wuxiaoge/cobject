#include "object.h"
#include <netdb.h>

int main(int argc, char *args[]) {
    Object *ip = StrObject_FromStr("0.0.0.0");
    Object *port = IntObject_FromInt(8808);
    Object *sock = SockObject_Open(ip, port);
    Object_CallMethod(sock, "Listen", 0);
    Object *size = IntObject_FromInt(1024);
    Object *out = StdoutObject_New();
    while(1) {
        Object *io = Object_CallMethod(sock, "Accept", 0);
        Object *str = Object_CallMethod(io, "Read", size);
        Object_CallMethod(out, "Writeline", str);
        Object_CallMethod(io, "Writeline", SockObject_IP(io));
        Object_CallMethod(io, "Writeline", SockObject_PORT(io));
        Object_DECREF(str);
        Object_DECREF(io);
    }
    Object_DECREF(out);
    Object_DECREF(size);
    Object_DECREF(ip);
    Object_DECREF(port);
    Object_DECREF(sock);
    struct hostent *host = gethostbyname("baidu.com");
    int i = 0;
    for(; i < host->h_length; i++) {
        char buf[32] = {0};
        inet_ntop(host->h_addrtype, *(host->h_addr_list + i), buf, sizeof(buf));
        printf("%s\n", buf);
    }
    return 0;
}


