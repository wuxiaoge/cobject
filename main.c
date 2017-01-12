#include "object.h"
#include "modules/threadpoolobject.h"

int main(int argc, char *args[]) {
    Object *size = IntObject_FromInt(4);
    Object *pool = ThreadPoolObject_New(size, TRUE);
    Object *ip = StrObject_FromStr("0.0.0.0");
    Object *port = IntObject_FromInt(8808);
    Object *sock = SockObject_Open(ip, port);
    Object_CallMethod(sock, "Listen", Object_NULL);
    Object_DECREF(ip);
    Object_DECREF(port);
    Object *out = StdoutObject_New();
    Object *len = IntObject_FromInt(1024);
    Object *sck = Object_NULL;
    Object *txt = Object_NULL;
    while(1) {
        sck = Object_CallMethod(sock, "Accept", Object_NULL);
        txt = Object_CallMethod(sck, "Read", len);
        Object_CallMethod(out, "Write", txt);
        Object_CallMethod(sck, "Writeline", SockObject_IP(sck));
        Object_CallMethod(sck, "Writeline", SockObject_PORT(sck));
        Object_CallMethod(pool, "Add", sck);
        Object_DECREF(txt);
        Object_DECREF(sck);
    }
    Object_DECREF(len);
    Object_DECREF(out);
    Object_DECREF(sock);
    Object_DECREF(size);
    Object_DECREF(pool);
    return 0;
}


