#include "object.h"
#include "modules/threadpoolobject.h"
#include "modules/httpheaderobject.h"
#include "modules/httprequestobject.h"
#include "modules/httpserverobject.h"

int main(int argc, char *args[]) {
    Object *ip = StrObject_FromStr("0.0.0.0");
    Object *port = IntObject_FromInt(1024);
    Object *server = HttpServerObject_New(ip, port);
    Object_CallMethod(server, "Start", port);
    Object_DECREF(ip);
    Object_DECREF(port);
    Object_DECREF(server);
    return 0;
}


