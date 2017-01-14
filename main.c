#include "object.h"
#include "modules/threadpoolobject.h"
#include "modules/httpheaderobject.h"

int main(int argc, char *args[]) {
    Object *out = StdoutObject_New();
    Object *size = IntObject_FromInt(2);
    Object *hs = StrObject_FromStr("Server: LQS/1.0\r\nAccept: */*\r\nContent-Type: text/html\r\n");
    Object *header = HttpHeaderObject_New(hs);
    Object_DECREF(hs);

    Object *item1 = ListObject_New(size);
    Object *key1 = StrObject_FromStr("Content-Lenght");
    Object_CallMethod(item1, "Append", key1);
    Object_DECREF(key1);
    Object *value1 = StrObject_FromStr("1024");
    Object_CallMethod(item1, "Append", value1);
    Object_DECREF(value1);
    Object_CallMethod(header, "Add", item1);
    Object_DECREF(item1);

    Object_CallMethod(out, "Writeline", header);
    Object_DECREF(header);
    Object_DECREF(size);
    Object_DECREF(out);
    return 0;
}


