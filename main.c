#include "includes/object.h"

int main(int argc, char *args[]) {
    Object *s = StrObject_FromStr("Hello World !!!");
    printf("s %s     %s     %ld\n", Object_TYPE_NAME(s), StrObject_AsSTR(s), Object_VAR_SIZE(s));
    Object *lo = ListObject_New(2);
    Object *start = IntObject_FromInt(1);
    Object *end = IntObject_FromInt(5);
    Object_CallMethod(lo, "Append", start);
    Object_CallMethod(lo, "Append", end);
    Object *ss = Object_CallMethod(s, "Substr", lo);
    printf("ss : %s\n", StrObject_AsSTR(ss));
    Object_DECREF(s);
    Object_DECREF(ss);
    Object_DECREF(start);
    Object_DECREF(end);
    Object_DECREF(lo);
    return 0;
}


