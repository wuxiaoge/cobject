#ifndef FILE_OBJECT_H
#define FILE_OBJECT_H
#ifdef __cplusplus
extern "C" {
#endif

typedef struct _fileobject {
    Object_HEAD
    Object *ob_filename;
} FileObject;

extern TypeObject File_Type;

#define FileObject_CHECK(ob) Object_CHECK(ob, &File_Type)
#define FileObject_CONVERT(ob) ((FileObject *)(ob))
#define FileObject_FILENAME(ob) FileObject_CONVERT(ob)->ob_filename

Object *FileObject_Open(Object *filename, Object *status);

#ifdef __cplusplus
}
#endif
#endif
