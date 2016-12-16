#include "object.h"

int main(int argc, char *args[]) {
   Object *out = StdoutObject_New();
   Object *s = StrObject_FromStr("hello world !!!");
   Object *i1 = IntObject_FromInt(1);
   Object *i2 = IntObject_FromInt(5);
   Object *lst = ListObject_New(2);
   Object_CallMethod(lst, "Append", i1);
   Object_CallMethod(lst, "Append", i2);
   Object *ss = Object_CallMethod(s, "Substr", lst);
   Object *sss = Object_CallMethod(s, "Concat", ss);
   Object_CallMethod(out, "Writeline", s);
   Object_CallMethod(out, "Writeline", ss);
   Object_CallMethod(out, "Writeline", lst);
   Object_DECREF(i2);
   Object_DECREF(i1);
   Object_DECREF(lst);
   Object_DECREF(sss);
   Object_DECREF(ss);
   Object_DECREF(s);
   Object_DECREF(out);

   return 0;
}


