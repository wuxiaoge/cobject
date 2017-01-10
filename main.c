#include "object.h"

int main(int argc, char *args[]) {
    Object *t = ThreadObject_New(NULL);
    Object *r = Object_CallMethod(t, "Join", Object_NULL);
    Object_DECREF(r);
    Object_DECREF(t);
    return 0;
}


