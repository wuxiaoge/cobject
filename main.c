#include "object.h"
#include "modules/threadpoolobject.h"
#include "modules/httpheaderobject.h"
#include "modules/httprequestobject.h"
#include "modules/httpserverobject.h"
#include "modules/httphandlerobject.h"

int main(int argc, char *args[]) {
    Object *size = IntObject_FromInt(2);
    Object *url_handler = Object_NULL;
    Object *url = Object_NULL;
    Object *handler = Object_NULL;
    url_handler = ListObject_New(size);
    Object_DECREF(size);
    url = StrObject_FromStr("/user");
    Object_CallMethod(url_handler, "Append", url);
    Object_DECREF(url);
    handler = HttpHandlerObject_New();
    Object_CallMethod(url_handler, "Append", handler);
    Object_DECREF(handler);

    Object *ip = StrObject_FromStr("0.0.0.0");
    Object *port = IntObject_FromInt(1024);
    Object *server = HttpServerObject_New(ip, port);
    Object_CallMethod(server, "AddHandler", url_handler);
    Object_CallMethod(server, "Start", port);
    Object_DECREF(ip);
    Object_DECREF(port);
    Object_DECREF(server);
    Object_DECREF(url_handler);
    return 0;
}


