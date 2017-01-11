#include "object.h"

int main(int argc, char *args[]) {
    Object *size = IntObject_FromInt(4);
    Object *pool = ThreadPoolObject_New(size, FALSE);
    sleep(1);
    Object *lst = Object_CallMethod(pool, "Join", Object_NULL);
    /*printf("....%d\n", 1);
    Object_CallMethod(pool, "Add", size);
    printf("....%d\n", 2);
    Object_CallMethod(pool, "Add", size);
    printf("....%d\n", 3);
    Object_CallMethod(pool, "Add", size);
    printf("....%d\n", 4);
    Object_CallMethod(pool, "Add", size);*/
    Object_DECREF(size);
    Object_DECREF(lst);
    Object_DECREF(pool);
    return 0;
}


