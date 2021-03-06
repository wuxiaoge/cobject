#include "object.h"

static Object *io_method_read(Object *self, Object *ob) {
    assert(IntObject_CHECK(ob));
    int size = IntObject_AsINT(ob);
    char *buf = (char *)malloc(size * sizeof(char));
    int rsize = fread(buf, sizeof(char), size, IoObject_VALUE(self));
    Object *_s = StrObject_FromStrAndSize(buf, rsize);
    free(buf);
    return _s;
}

static Object *io_method_write(Object *self, Object *ob) {
    Object *_s = Object_Str(ob);
    fwrite(StrObject_AsSTR(_s), sizeof(char), StrObject_SIZE(_s), IoObject_VALUE(self));
    Object_DECREF(_s);
    return Object_NULL;
}

static Object *io_method_writeline(Object *self, Object* ob) {
    io_method_write(self, ob);
    fwrite("\n", 1, 1, IoObject_VALUE(self));
    return Object_NULL;
}

static Object *io_method_flush(Object *self, Object *ob) {
    fflush(IoObject_VALUE(self));
    return Object_NULL;
}

static Object *io_method_reopen(Object *self, Object *args) {
    assert(ListObject_CHECK(args) && ListObject_SIZE(args)==2);
    Object *zero = IntObject_FromInt(0);
    Object *fname = Object_CallMethod(args, "Get", zero);
    Object_DECREF(zero);
    assert(StrObject_CHECK(fname));
    Object *one = IntObject_FromInt(1);
    Object *fmode = Object_CallMethod(args, "Get", one);
    Object_DECREF(one);
    assert(StrObject_CHECK(fmode));
    FILE *tmp = freopen(StrObject_AsSTR(fname), StrObject_AsSTR(fmode), IoObject_VALUE(self));
    assert(tmp);
    return self;
}

static MethodDef io_methods[] = {
    {"Read", io_method_read},
    {"Write", io_method_write},
    {"Writeline", io_method_writeline},
    {"Flush", io_method_flush},
    {"Reopen", io_method_reopen},
    {Object_NULL, Object_NULL}
};

static int io_init(Object *self, Object *args) {
    Object_Extend(self, &Object_Type, sizeof(Object));
    Object_Init(Object_BASE(self), Object_NULL);
    IoObject_VALUE(self) = (FILE *)args;
    return Object_OK;
}

static int io_deinit(Object *self) {
    if(IoObject_VALUE(self)) {
        fclose(IoObject_VALUE(self));
    }
    return Object_OK;
}

static long io_hash(Object *self) {
    return -1L;
}

static BOOL io_equal(Object *self, Object *ob) {
    return Object_Equal(Object_BASE(self), Object_BASE(ob));
}

static Object *io_str(Object *self){
    return Object_Str(Object_BASE(self));
}

TypeObject Io_Type = {
    .tp_name = "io",
    .tp_init = io_init,
    .tp_deinit = io_deinit,
    .tp_hash = io_hash,
    .tp_equal = io_equal,
    .tp_str = io_str,
    .tp_methods = io_methods
};

Object *IoObject_FromFILE(FILE *file) {
    Object *_io = Object_Malloc(&Io_Type, sizeof(IoObject));
    Object_Init(_io, Object_CONVERT(file));
    return _io;
}

