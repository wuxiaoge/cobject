#include "includes/object.h"

int main(int argc, char *args[]) {
    printf("int\n");
    Object *i = IntObject_FromInt(10);
    printf("int : %d\n", IntObject_AsINT(i));
    Object *is = Object_Str(i);
    printf(" =====>> %s\n", StrObject_AsSTR(is));
    Object_DECREF(is);
    Object_DECREF(i);

    printf("str\n");
    Object *s = StrObject_FromStr("hello world !!!");
    printf("lengh : %ld\n", StrObject_SIZE(s));
    printf("value : %s Type : %s\n", StrObject_VALUE(s), Object_TYPE_NAME(s));
    Object_DECREF(s);

    printf("float\n");
    Object *f = FloatObject_FromFloat(1.6f);
    printf("value : %f\n", FloatObject_AsFLOAT(f));
    Object *fs = Object_Str(f);
    printf(" =====>> %s\n", StrObject_AsSTR(fs));
    Object_DECREF(fs);
    Object_DECREF(f);

    printf("long\n");
    Object *l = LongObject_FromLong(9L);
    printf("value : %ld Type Name : %s\n", LongObject_AsLONG(l), Object_TYPE_NAME(l));
    Object *ls = Object_Str(l);
    printf(" =====>> %s\n", StrObject_AsSTR(ls));
    Object_DECREF(l);
    Object_DECREF(ls);

    Object *a = IntObject_FromInt(1);
    printf("int a : %d\n", IntObject_AsINT(a));
    Object *b = IntObject_FromInt(11);
    printf("int b : %d\n", IntObject_AsINT(b));
    Object *c = Object_CallMethod(a, "Add_Int", b);
    Object_CallMethod(a, "GetTypeName", Object_NULL);
    printf("int c : %d\n", c ? IntObject_AsINT(c) : 0);
    Object_DECREF(a);
    Object_DECREF(b);
    Object_DECREF(c);

    return 0;
}


