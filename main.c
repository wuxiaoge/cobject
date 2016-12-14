#include "includes/object.h"

int main(int argc, char *args[]) {
   Object *s = StrObject_FromStr("hello world !!!");
   Object *start = IntObject_FromInt(1);
   Object *end = IntObject_FromInt(8);
   Object *lst = ListObject_New(2);
   Object_CallMethod(lst, "Append", start);
   Object_CallMethod(lst, "Append", end);
   Object *ss = Object_CallMethod(s, "Substr", lst);
   printf("%s\n", StrObject_AsSTR(s));
   printf("%s\n", StrObject_AsSTR(ss));
   Object_DECREF(ss);
   Object_DECREF(start);
   Object_DECREF(end);
   Object_DECREF(lst);
   Object_DECREF(s);
   return 0;
}


