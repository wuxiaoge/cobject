#include <stdarg.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#ifndef OBJECT_H
#define OBJECT_H
#ifdef __cplusplus
extern "C" {
#endif

#define BOOL int
#define TRUE 1
#define FALSE 0
#define Object_OK 0
#define Object_ERROR -1

#define Object_HEAD \
    size_t ob_refcnt; \
    struct _typeobject *ob_type; \
    struct _object *ob_base;

#define Object_VAR_HEAD \
    Object_HEAD \
    size_t ob_size;

typedef struct _object {
    Object_HEAD
} Object;

typedef struct _varobject {
    Object_VAR_HEAD
} VarObject;

typedef int (*initfunc)(Object *, Object *);
typedef int (*deinitfunc)(Object *);
typedef long (*hashfunc)(Object *);
typedef BOOL (*equalfunc)(Object *, Object *);
typedef Object *(*strfunc)(Object *);
typedef Object *(*methodfunc)(Object *, Object *);

typedef struct _methoddef {
    const char * md_name;
    methodfunc md_func;
} MethodDef;

typedef struct _typeobject {
    const char * tp_name;
    initfunc tp_init;
    deinitfunc tp_deinit;
    hashfunc tp_hash;
    equalfunc tp_equal;
    strfunc tp_str;
    struct _methoddef *tp_methods;
} TypeObject;

extern TypeObject Object_Type;

#define Object_CONVERT(ob) ((Object *)(ob))
#define Object_NULL ((void *)0)
#define Object_MALLOC(size) Object_CONVERT(malloc((size)))
#define Object_EXTEND(ob, super) Object_BASE(ob) = (super)
#define Object_TYPE(ob) Object_CONVERT(ob)->ob_type
#define Object_TYPE_NAME(ob) Object_TYPE(ob)->tp_name
#define Object_BASE(ob) Object_CONVERT(ob)->ob_base
#define Object_REFCNT(ob) Object_CONVERT(ob)->ob_refcnt
#define Object_INCREF(ob) Object_IncRef(Object_CONVERT(ob))
#define Object_DECREF(ob) Object_DecRef(Object_CONVERT(ob))
#define Object_CHECK(ob, type) Object_Check((ob), (type))

#define Object_INIT(ob, type) \
    Object_REFCNT(ob) = 1; \
    Object_TYPE(ob) = (struct _typeobject *)(type); \
    Object_BASE(ob) = Object_NULL

#define Object_VAR_CONVERT(ob) ((VarObject *)(ob))
#define Object_VAR_SIZE(ob) Object_VAR_CONVERT(ob)->ob_size

int TypeObject_Init(Object *, TypeObject *, Object *);
int TypeObject_Deinit(Object *, TypeObject *);
long TypeObject_Hash(Object *, TypeObject *);
BOOL TypeObject_Equal(Object *, TypeObject *, Object *);
Object *TypeObject_Str(Object *, TypeObject *);

Object *Object_Malloc(TypeObject *, size_t);
int Object_Extend(Object *, TypeObject *, size_t);
int Object_Init(Object *, Object *);
int Object_Deinit(Object *);
long Object_Hash(Object *);
BOOL Object_Equal(Object *, Object *);
Object *Object_Str(Object *);
BOOL Object_Check(Object *, TypeObject *);
methodfunc Object_GetMethod(Object *, Object *, Object **);
Object *Object_CallMethod(Object *, const char *, Object *);
size_t Object_IncRef(Object *);
size_t Object_DecRef(Object *);
void Object_Free(Object **);

#ifdef __cplusplus
}
#endif
#endif

#include "intobject.h"
#include "longobject.h"
#include "floatobject.h"
#include "doubleobject.h"
#include "strobject.h"
#include "listobject.h"
#include "ioobject.h"
#include "stdinobject.h"
#include "stdoutobject.h"
#include "stderrobject.h"
#include "fileobject.h"
#include "sockobject.h"


