#include "includes/object.h"

int main(int argc, char *args[]) {
    Object *s = StrObject_FromStr("Hello World !!!");
    printf("s %s     %s     %ld\n", Object_TYPE_NAME(s), StrObject_AsSTR(s), Object_VAR_SIZE(s));
    Object *ss = StrObject_FromStr("Hello Game Fire !!!");
    printf("ss %s     %s     %ld\n", Object_TYPE_NAME(ss), StrObject_AsSTR(ss), Object_VAR_SIZE(ss));
    Object *as = Object_CallMethod(s, "Concat", ss);
    printf("as %s     %s     %ld\n", Object_TYPE_NAME(as), StrObject_AsSTR(as), Object_VAR_SIZE(as));
    Object *start = IntObject_FromInt(21);
    Object *end = IntObject_FromInt(25);
    IntObject objs[] = {*(IntObject *)start, *(IntObject *)end};
    Object *us = Object_CallMethod(as, "Substr", (Object *)objs);
    if(us) {
        printf("us %s     %s     %ld\n", Object_TYPE_NAME(us), StrObject_AsSTR(us), Object_VAR_SIZE(us));
    }
    Object *sub = StrObject_FromStr("Game");
    printf("sub %s     %s     %ld\n", Object_TYPE_NAME(sub), StrObject_AsSTR(sub), Object_VAR_SIZE(sub));
    Object *idx = Object_CallMethod(as, "Index", sub);
    printf("idx %s     %d\n", Object_TYPE_NAME(idx), IntObject_AsINT(idx));
    Object_DECREF(sub);
    Object_DECREF(idx);

    Object_DECREF(start);
    Object_DECREF(end);
    Object_DECREF(s);
    Object_DECREF(ss);
    Object_DECREF(as);
    Object_DECREF(us);
    return 0;
}


