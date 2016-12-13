#include "includes/object.h"

int main(int argc, char *args[]) {
    Object *s1 = StrObject_FromStr("Hello");
    Object *s2 = StrObject_FromStr("World");
    Object *s = Object_CallMethod(s1, "Concat", s2);
    printf("s %s     %s     %ld\n", Object_TYPE_NAME(s), StrObject_AsSTR(s), Object_VAR_SIZE(s));
    Object *lo = ListObject_New();
    Object_CallMethod(lo, "Append", s1);
    Object_CallMethod(lo, "Append", s2);
    Object_CallMethod(lo, "Append", s);
    Object *i = IntObject_FromInt(1);
    Object *tmp = Object_CallMethod(lo, "Get", i);
    printf("s %s     %s     %ld\n", Object_TYPE_NAME(tmp), StrObject_AsSTR(tmp), Object_VAR_SIZE(tmp));
    Object_DECREF(s1);
    Object_DECREF(s2);
    Object_DECREF(s);
    Object_DECREF(lo);
    Object_DECREF(i);
    Object_DECREF(tmp);
    return 0;
}


