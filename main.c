#include "object.h"

int main(int argc, char *args[]) {
   Object *first = IntObject_FromInt(3);
   Object *second = IntObject_FromInt(9);
   Object *lst = ListObject_New(2);
   Object_CallMethod(lst, "Append", first);
   Object_DECREF(first);
   Object_CallMethod(lst, "Append", second);
   Object_DECREF(second);
   Object *str = StrObject_FromStr("hello world !!!");
   Object *out = StdoutObject_New();
   Object_CallMethod(out, "Writeline", lst);
   Object_CallMethod(out, "Writeline", str);
   Object *sub = Object_CallMethod(str, "Substr", lst);
   Object_CallMethod(out, "Writeline", sub);
   Object_DECREF(sub);
   Object_DECREF(lst);
   Object_DECREF(str);
   Object_DECREF(out);
   return 0;
}


