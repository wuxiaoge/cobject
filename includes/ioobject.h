#ifndef IO_OBJECT_H
#define IO_OBJECT_H
#ifdef __cplusplus
extern "C" {
#endif

typedef struct _ioobject {
    Object_HEAD
    FILE *ob_fdval;
} IoObject;

extern TypeObject Io_Type;

#define IoObject_CHECK(ob) Object_CHECK(ob, &Io_Type)
#define IoObject_CONVERT(ob) ((IoObject *)(ob))
#define IoObject_VALUE(ob) IoObject_CONVERT(ob)->ob_fdval
#define IoObject_AsFILE(ob) IoObject_VALUE(ob)

Object *IoObject_FromFILE(FILE *);

#ifdef __cplusplus
}
#endif
#endif
