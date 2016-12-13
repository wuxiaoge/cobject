#ifndef STR_OBJECT_H
#define STR_OBJECT_H
#ifdef __cplusplus
extern "C" {
#endif

typedef struct _strobject {
    Object_VAR_HEAD
    char ob_sval[1];
} StrObject;

extern TypeObject Str_Type;

#define StrObject_CONVERT(ob) ((StrObject *)(ob))
#define StrObject_VALUE(ob) StrObject_CONVERT(ob)->ob_sval
#define StrObject_SIZE(ob) StrObject_CONVERT(ob)->ob_size
#define StrObject_AsSTR(ob) (const char *)StrObject_VALUE(ob)

Object *StrObject_FromStrAndSize(const char *, size_t);
Object *StrObject_FromStr(const char *);

#ifdef __cplusplus
}
#endif
#endif
