#include "includes/object.h"

int main(int argc, char *args[]) {
    Object *lo = ListObject_New(6);
    Object *i0= IntObject_FromInt(0);
    Object *i1= IntObject_FromInt(1);
    Object *i2= IntObject_FromInt(2);
    Object *i3= IntObject_FromInt(3);
    Object *i4= IntObject_FromInt(4);
    Object_CallMethod(lo, "Append", i0);
    Object_CallMethod(lo, "Append", i1);
    Object_CallMethod(lo, "Append", i2);
    Object_CallMethod(lo, "Append", i3);
    Object_CallMethod(lo, "Append", i4);
    printf("list size %ld, alloc size %ld\n", ListObject_SIZE(lo), ListObject_ALLOCATED(lo));
    printf("item 1 : %d\n", IntObject_AsINT(ListObject_GetITEM(lo, 1)));
    Object *la = ListObject_New(2);
    Object_CallMethod(la, "Append", i1);
    Object_CallMethod(la, "Append", i3);
    Object *sl = Object_CallMethod(lo, "Slice", la);
    printf("list size %ld, alloc size %ld\n", ListObject_SIZE(sl), ListObject_ALLOCATED(sl));
    printf("item 0 : %d\n", IntObject_AsINT(ListObject_GetITEM(sl, 1)));
    Object_DECREF(lo);
    Object_DECREF(i0);
    Object_DECREF(i1);
    Object_DECREF(i2);
    Object_DECREF(i3);
    Object_DECREF(i4);
    Object_DECREF(la);
    Object_DECREF(sl);
    return 0;
}


