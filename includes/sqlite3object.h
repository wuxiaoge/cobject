#include <sqlite3.h>
#ifndef SQLITE3_OBJECT_H
#define SQLITE3_OBJECT_H
#ifdef __cplusplus
extern "C" {
#endif

typedef struct _sqlite3object {
    Object_HEAD
    sqlite3 *ob_db;
} Sqlite3Object;

extern TypeObject Sqlite3_Type;

#define Sqlite3Object_CHECK(ob) Object_CHECK(ob, &Sqlite3_Type)
#define Sqlite3Object_CONVERT(ob) ((Sqlite3Object *)(ob))
#define Sqlite3Object_DB(ob) Sqlite3Object_CONVERT(ob)->ob_db

Object *Sqlite3Object_Open(Object *);

#ifdef __cplusplus
}
#endif
#endif
