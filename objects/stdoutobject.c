#include "object.h"

static Object *stdout_method_read(Object *self, Object *ob) {
    fprintf(stderr, "Error : stdout not call read function !!!\n");
    return Object_NULL;
}

static MethodDef stdout_methods[] = {
    {"Read", stdout_method_read},
    {Object_NULL, Object_NULL}
};

static int stdout_init(Object *self, Object *args) {
    Object_Extend(self, &Io_Type, sizeof(IoObject));
    Object_Init(Object_BASE(self), Object_CONVERT(stdout));
    return Object_OK;
}

static int stdout_deinit(Object *self) {
    return Object_OK;
}

static long stdout_hash(Object *self) {
    return -1L;
}

static BOOL stdout_equal(Object *self, Object *ob) {
    return Object_Equal(Object_BASE(self), Object_BASE(ob));
}

static Object *stdout_str(Object *self) {
    return Object_Str(Object_BASE(self));
}

TypeObject Stdout_Type = {
    .tp_name = "stdout",
    .tp_init = stdout_init,
    .tp_deinit = stdout_deinit,
    .tp_hash = stdout_hash,
    .tp_equal = stdout_equal,
    .tp_str = stdout_str,
    .tp_methods = stdout_methods
};

Object *StdoutObject_New() {
    Object *_io = Object_Malloc(&Stdout_Type, sizeof(StdoutObject));
    Object_Init(_io, Object_NULL);
    return _io;
}
