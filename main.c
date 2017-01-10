#include "object.h"

static Object *callback(Object *self, Object *args) {
    Object *out = StdoutObject_New();
    Object_CallMethod(out, "Writeline", args);
    Object_DECREF(out);
    return Object_NULL;
}

int main(int argc, char *args[]) {
    Object *lst = ListObject_New(2);
    Object *i0 = IntObject_FromInt(0);
    Object *i1 = IntObject_FromInt(1);
    Object_CallMethod(lst, "Append", i0);
    Object_CallMethod(lst, "Append", i1);
    Object_DECREF(i0);
    Object_DECREF(i1);
    Object *t = ThreadObject_New(callback, lst);
    Object *t2 = ThreadObject_New(callback, lst);
    Object *r = Object_CallMethod(t, "Join", Object_NULL);
    Object *r2 = Object_CallMethod(t2, "Join", Object_NULL);
    Object_DECREF(lst);
    Object_DECREF(r);
    Object_DECREF(t);
    Object_DECREF(r2);
    Object_DECREF(t2);
    return 0;
}


