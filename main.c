#include "object.h"
#include "modules/threadpoolobject.h"
#include "modules/httpheaderobject.h"
#include "modules/httprequestobject.h"

int main(int argc, char *args[]) {
    Object *out = StdoutObject_New();
    Object *size = IntObject_FromInt(2);
    Object *hs = StrObject_FromStr("Server: LQS/1.0\r\nAccept: */*\r\nContent-Type: text/html\r\n");

    Object *ip = StrObject_FromStr("0.0.0.0");
    Object *port = IntObject_FromInt(1024);
    Object *listen = SockObject_Open(ip, port);
    Object_CallMethod(listen, "Listen", port);
    Object_DECREF(ip);

//    while(1) {
        Object *header = HttpHeaderObject_New(hs);
        Object *htp = StrObject_FromStr("HTTP/1.1 200 OK\r\n");
        Object *body = StrObject_FromStr("\r\nSuccess !!!");

        Object *item1 = ListObject_New(size);
        Object *key1 = StrObject_FromStr("Content-Length");
        Object_CallMethod(item1, "Append", key1);
        Object_DECREF(key1);
        Object *value1 = StrObject_FromStr("11");
        Object_CallMethod(item1, "Append", value1);
        Object_DECREF(value1);
        Object_CallMethod(header, "Add", item1);

        Object *sock = Object_CallMethod(listen, "Accept", Object_NULL);
        Object *str = Object_CallMethod(sock, "Read", port);
        Object *req = HttpRequestObject_New(str);
        printf("=====================\n");
        Object_CallMethod(out, "Write", str);
        Object_DECREF(str);
        Object_CallMethod(sock, "Write", htp);
        Object_CallMethod(sock, "Write", header);
        Object_CallMethod(sock, "Write", body);
        Object_DECREF(req);
        Object_DECREF(sock);

        Object_DECREF(item1);
        Object_DECREF(body);
        Object_DECREF(htp);
        Object_DECREF(header);
//    }

    Object_DECREF(listen);
    Object_DECREF(port);
    Object_DECREF(hs);
    Object_DECREF(size);
    Object_DECREF(out);
    return 0;
}


