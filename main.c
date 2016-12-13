#include "includes/object.h"

int main(int argc, char *args[]) {
    Object *a = LongObject_FromLong(2L);
    Object *as = Object_Str(a);
    printf("%s a : %s\n", Object_TYPE_NAME(a), StrObject_AsSTR(as));
    Object_REFCNT(as);

    Object *b = DoubleObject_FromDouble(11.11);
    Object *bs = Object_Str(b);
    printf("%s b : %s\n", Object_TYPE_NAME(b), StrObject_AsSTR(bs));
    Object_DECREF(bs);

    Object *c = Object_CallMethod(a, "Add", b);
    Object *cs = Object_Str(c);
    printf("%s c : %s\n", Object_TYPE_NAME(c), cs ? StrObject_AsSTR(cs) : "0");
    Object_DECREF(cs);

    Object *d = Object_CallMethod(c, "Div", b);
    Object *ds = Object_Str(d);
    printf("%s d : %s\n", Object_TYPE_NAME(d), ds ? StrObject_AsSTR(ds) : "0");
    Object_DECREF(ds);

    Object *e = Object_CallMethod(d, "Mul", b);
    Object *es = Object_Str(e);
    printf("%s e : %s\n", Object_TYPE_NAME(e), es ? StrObject_AsSTR(es) : "0");
    Object_DECREF(es);

    Object *f = Object_CallMethod(a, "Sub", e);
    Object *fs = Object_Str(f);
    printf("%s f : %s\n", Object_TYPE_NAME(f), fs ? StrObject_AsSTR(fs) : "0");
    Object_DECREF(fs);

    Object_DECREF(a);
    Object_DECREF(b);
    Object_DECREF(c);
    Object_DECREF(e);
    Object_DECREF(f);

    return 0;
}


