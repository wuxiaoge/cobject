#include "object.h"
#include "modules/threadpoolobject.h"
#include "modules/httpheaderobject.h"
#include "modules/httprequestobject.h"
#include "modules/httpserverobject.h"
#include "modules/httphandlerobject.h"

int main(int argc, char *args[]) {
    /*Object *size = IntObject_FromInt(2);
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
    Object_DECREF(url_handler);*/
    Object *sql = StrObject_FromStr("SELECT * FROM users");
    Object *sql2 = StrObject_FromStr("INSERT INTO users(name) VALUES('well')");
    Object *dbname = StrObject_FromStr("/home/lq/testdb.db");
    Object *sqlite = Sqlite3Object_Open(dbname);
//    Object_CallMethod(sqlite, "Begin", Object_NULL);
//    Object_CallMethod(sqlite, "Exec", sql2);
//    Object_CallMethod(sqlite, "Commit", Object_NULL);
    Object *res = Object_CallMethod(sqlite, "Query", sql);
    Object *out = StdoutObject_New();
    Object_CallMethod(out, "Writeline", res);
    Object_DECREF(out);
    Object_DECREF(res);
    Object_DECREF(sqlite);
    Object_DECREF(dbname);
    Object_DECREF(sql2);
    Object_DECREF(sql);
    return 0;
}


