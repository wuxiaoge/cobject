#include "object.h"

static Object *stderr_method_read(Object *self, Object *ob) {
    fprintf(stderr, "Error : stderr not call read function !!!\n");
    return Object_NULL;
}

static MethodDef stderr_methods[] = {
    {"Read", stderr_method_read},
    {Object_NULL, Object_NULL}
};

static int stderr_init(Object *self, Object *args) {
    Object_Extend(self, &Io_Type, sizeof(IoObject));
    Object_Init(Object_BASE(self), Object_CONVERT(stderr));
    return Object_OK;
}

static int stderr_deinit(Object *self) {
    return Object_Deinit(Object_BASE(self));
}

static long stderr_hash(Object *self) {
    return -1L;
}

static BOOL stderr_equal(Object *self, Object *ob) {
    return Object_Equal(Object_BASE(self), Object_BASE(ob));
}

static Object *stderr_str(Object *self) {
    return Object_Str(Object_BASE(self));
}

TypeObject Stderr_Type = {
    .tp_name = "stderr",
    .tp_init = stderr_init,
    .tp_deinit = stderr_deinit,
    .tp_hash = stderr_hash,
    .tp_equal = stderr_equal,
    .tp_str = stderr_str,
    .tp_methods = stderr_methods
};

Object *StderrObject_New() {
    Object *_io = Object_Malloc(&Stderr_Type, sizeof(StderrObject));
    Object_Init(_io, Object_NULL);
    return _io;
}

