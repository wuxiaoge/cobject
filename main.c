#include "object.h"

int main(int argc, char *args[]) {
   Object *out = StdoutObject_New();

   Object *fname = StrObject_FromStr("stdout.out");
   Object *fmode = StrObject_FromStr("a+");
   Object *arglst = ListObject_New(2);
   Object_CallMethod(arglst, "Append", fname);
   Object_CallMethod(arglst, "Append", fmode);
   Object_CallMethod(out, "Reopen", arglst);
   Object_DECREF(fname);
   Object_DECREF(fmode);
   Object_DECREF(arglst);

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
   printf("%d\n", Object_Equal(lst, lst));
   Object_CallMethod(out, "Flush", Object_NULL);
   Object_DECREF(i2);
   Object_DECREF(i1);
   Object_DECREF(lst);
   Object_DECREF(sss);
   Object_DECREF(ss);
   Object_DECREF(s);
   Object_DECREF(out);

   return 0;
}


