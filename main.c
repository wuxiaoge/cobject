#include "includes/object.h"

int callback(Object *idx, Object *it) {
    printf("%d    ", IntObject_AsINT(idx));
    printf("%d\n", IntObject_AsINT(it));
    return 0;
}

Object *map_cb(Object *idx, Object *it) {
    Object *_i = IntObject_FromInt(2);
    Object *_ri = Object_CallMethod(idx, "Add", _i);
    Object *ret = Object_CallMethod(it, "Mul" ,_ri);
    Object_DECREF(_i);
    Object_DECREF(_ri);
    return ret;
}

BOOL filter_cb(Object *idx, Object *it) {
    int _i = IntObject_AsINT(idx);
    return !_i ? TRUE : FALSE;
}

int main(int argc, char *args[]) {
   Object *s = StrObject_FromStr("hello world !!!");
   Object *start = IntObject_FromInt(1);
   Object *end = IntObject_FromInt(8);
   Object *end1 = IntObject_FromInt(8);
   Object *lst = ListObject_New(2);
   Object_CallMethod(lst, "Append", start);
   Object_CallMethod(lst, "Append", end);
   Object *ss = Object_CallMethod(s, "Substr", lst);
   printf("%s\n", StrObject_AsSTR(s));
   printf("%s\n", StrObject_AsSTR(ss));
   Object_CallMethod(lst, "Foreach", Object_CONVERT(callback));
   printf("===== %d\n", Object_Equal(end, end1));
   Object *ls = Object_CallMethod(lst, "Map", Object_CONVERT(map_cb));
   Object_CallMethod(ls, "Foreach", Object_CONVERT(callback));
   Object *str = Object_Str(ls);
   printf(".......%s\n", StrObject_AsSTR(str));
   Object *fs = Object_CallMethod(ls, "Filter", Object_CONVERT(filter_cb));
   Object_CallMethod(fs, "Foreach", Object_CONVERT(callback));
   Object_DECREF(str);
   Object_DECREF(fs);
   Object_DECREF(ls);
   Object_DECREF(ss);
   Object_DECREF(start);
   Object_DECREF(end);
   Object_DECREF(end1);
   Object_DECREF(lst);
   Object_DECREF(s);

   return 0;
}


