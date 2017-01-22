#include "object.h"

static Object *sqlite3_method_exec(Object *self, Object *args) {
    assert(StrObject_CHECK(args));
    char *errMsg = NULL;
    int status;
    status = sqlite3_exec(Sqlite3Object_DB(self),
        StrObject_AsSTR(args), NULL, NULL, &errMsg);
    if(status != SQLITE_OK) {
        fprintf(stderr, "%s\n", errMsg);
    }
    sqlite3_free(errMsg);
    return Object_NULL;
}

static Object *sqlite3_method_query(Object *self, Object *args) {
    assert(StrObject_CHECK(args));
    char *errMsg = NULL;
    char **result = NULL;
    int i, status, nrow, ncol, total;
    status = sqlite3_get_table(Sqlite3Object_DB(self),
        StrObject_AsSTR(args), &result, &nrow, &ncol, &errMsg);
    total = (nrow + 1) * ncol;
    Object *size = IntObject_FromInt(total);
    Object *res = ListObject_New(size);
    Object_DECREF(size);
    Object *item = Object_NULL;
    if(status == SQLITE_OK) {
        for(i = 0; i < total; i++) {
            item = StrObject_FromStr(*(result + i));
            Object_CallMethod(res, "Append", item);
            Object_DECREF(item);
        }
    } else {
        fprintf(stderr, "%s\n", errMsg);
    }
    sqlite3_free_table(result);
    sqlite3_free(errMsg);
    return res;
}

static Object *sqlite3_method_begin(Object *self, Object *args) {
    Object *begin = StrObject_FromStr("BEGIN");
    sqlite3_method_exec(self, begin);
    Object_DECREF(begin);
}

static Object *sqlite3_method_commit(Object *self, Object *args) {
    Object *commit = StrObject_FromStr("COMMIT");
    sqlite3_method_exec(self, commit);
    Object_DECREF(commit);
}

static Object *sqlite3_method_rollback(Object *self, Object *args) {
    Object *rollback = StrObject_FromStr("ROLLBACK");
    sqlite3_method_exec(self, rollback);
    Object_DECREF(rollback);
}

static MethodDef sqlite3_methods[] = {
    {"Exec", sqlite3_method_exec},
    {"Query", sqlite3_method_query},
    {"Begin", sqlite3_method_begin},
    {"Commit", sqlite3_method_commit},
    {"Rollback", sqlite3_method_rollback},
    {Object_NULL, Object_NULL}
};

static int sqlite3_init(Object *self, Object *args) {
    assert(StrObject_CHECK(args));
    sqlite3_initialize();
    int status = sqlite3_open(StrObject_AsSTR(args), &(Sqlite3Object_DB(self)));
    if(status != SQLITE_OK) {
        return Object_ERROR;
    }
    return Object_OK;
}

static int sqlite3_deinit(Object *self) {
    sqlite3_close(Sqlite3Object_DB(self));
    sqlite3_shutdown();
    return Object_OK;
}

static long sqlite3_hash(Object *self) {
    return -1L;
}

static BOOL sqlite3_equal(Object *self, Object *ob) {
    return Object_Equal(Object_BASE(self), Object_BASE(ob));
}

static Object *sqlite3_str(Object *self) {
    return Object_Str(Object_BASE(self));
}

TypeObject Sqlite3_Type = {
    .tp_name = "sqlite3",
    .tp_init = sqlite3_init,
    .tp_deinit = sqlite3_deinit,
    .tp_hash = sqlite3_hash,
    .tp_equal = sqlite3_equal,
    .tp_str = sqlite3_str,
    .tp_methods = sqlite3_methods
};

Object *Sqlite3Object_Open(Object *dbname) {
    Object *_db = Object_Malloc(&Sqlite3_Type, sizeof(Sqlite3Object));
    if(Object_OK != Object_Init(_db, dbname)) {
        Object_DECREF(_db);
        _db = Object_NULL;
    }
    return _db;
}

