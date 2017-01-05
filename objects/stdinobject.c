#include "object.h"

static Object *stdin_method_write(Object *self, Object *ob) {
    fprintf(stderr, "Error : stdin not call write function !!!\n");
    return Object_NULL;
}

static MethodDef stdin_methods[] = {
    {"Write", stdin_method_write},
    {Object_NULL, Object_NULL}
};

static int stdin_init(Object *self, Object *args) {
    Object_Extend(self, &Io_Type, sizeof(IoObject));
    Object_Init(Object_BASE(self), Object_CONVERT(stdin));
    return Object_OK;
}

static int stdin_deinit(Object *self) {
    return Object_OK;
}

static long stdin_hash(Object *self) {
    return -1L;
}

static BOOL stdin_equal(Object *self, Object *ob) {
    return Object_Equal(Object_BASE(self), Object_BASE(ob));
}

static Object *stdin_str(Object *self) {
    return Object_Str(Object_BASE(self));
}

TypeObject Stdin_Type = {
    .tp_name = "stdin",
    .tp_init = stdin_init,
    .tp_deinit = stdin_deinit,
    .tp_hash = stdin_hash,
    .tp_equal = stdin_equal,
    .tp_str = stdin_str,
    .tp_methods = stdin_methods
};

Object *StdinObject_New() {
    Object *_io = Object_Malloc(&Stdin_Type, sizeof(StdinObject));
    Object_Init(_io, Object_NULL);
    return _io;
}

