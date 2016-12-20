#include "object.h"

static Object *io_method_reopen(Object *self, Object *args) {
    assert(ListObject_CHECK(args));
    Object *old_filename = FileObject_FILENAME(self);
    Object *zero = IntObject_FromInt(0);
    Object *fname = Object_CallMethod(args, "Get", zero);
    assert(StrObject_CHECK(fname));
    Object_INCREF(fname);
    FileObject_FILENAME(self) = fname;
    Object_DECREF(zero);
    Object_DECREF(old_filename);
    return Object_CallMethod(Object_BASE(self), "Reopen", args);
}

static MethodDef file_methods[] = {
    {"Reopen", io_method_reopen},
    {Object_NULL, Object_NULL}
};

static int file_init(Object *self, Object *args) {
    if(!ListObject_CHECK(args)) {
        return Object_ERROR;
    }
    Object_Extend(self, &Io_Type, sizeof(IoObject));
    Object *_i0 = IntObject_FromInt(0);
    Object *ofilename = Object_CallMethod(args, "Get", _i0);
    Object *_i1 = IntObject_FromInt(1);
    Object *omode = Object_CallMethod(args, "Get", _i1);
    const char *filename = StrObject_AsSTR(ofilename);
    const char *mode = StrObject_AsSTR(omode);
    FILE *_file = fopen(filename, mode);
    Object_Init(Object_BASE(self), Object_CONVERT(_file));
    Object_INCREF(ofilename);
    FileObject_FILENAME(self) = ofilename;
    Object_DECREF(_i0);
    Object_DECREF(_i1);
    return Object_OK;
}

static int file_deinit(Object *self) {
    Object_DECREF(FileObject_FILENAME(self));
    return Object_Deinit(Object_BASE(self));
}

static long file_hash(Object *self) {
    return -1L;
}

static BOOL file_equal(Object *self, Object *ob) {
    return Object_Equal(Object_BASE(self), Object_BASE(ob));
}

static Object *file_str(Object *self) {
    return Object_Str(Object_BASE(self));
}

TypeObject File_Type = {
    .tp_name = "file",
    .tp_init = file_init,
    .tp_deinit = file_deinit,
    .tp_hash = file_hash,
    .tp_equal = file_equal,
    .tp_str = file_str,
    .tp_methods = file_methods
};

Object *FileObject_Open(Object *filename, Object *mode) {
    Object *_io = Object_Malloc(&File_Type, sizeof(FileObject));
    Object *_lst = ListObject_New(2);
    Object_CallMethod(_lst, "Append", filename);
    Object_CallMethod(_lst, "Append", mode);
    Object_Init(_io, _lst);
    Object_DECREF(_lst);
    return _io;
}

