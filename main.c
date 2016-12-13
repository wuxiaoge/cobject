#include "includes/object.h"

int main(int argc, char *args[]) {
    Object *s1 = StrObject_FromStr("Hello");
    Object *s2 = StrObject_FromStr("World");
    Object *s = Object_CallMethod(s1, "Concat", s2);
    printf("s %s     %s     %ld\n", Object_TYPE_NAME(s), StrObject_AsSTR(s), Object_VAR_SIZE(s));
    Object_DECREF(s1);
    Object_DECREF(s2);
    Object_DECREF(s);
    return 0;
}


