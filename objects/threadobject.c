#include "object.h"

static Object *thread_methods_detach(Object *self, Object *args) {
    pthread_detach(ThreadObject_TID(self));
    return Object_NULL;
}

static Object *thread_methods_join(Object *self, Object *args) {
    Object *ret = Object_NULL;
    pthread_join(ThreadObject_TID(self), (void **)&ret);
    return ret;
}

static MethodDef thread_methods[] = {
    {"Detach", thread_methods_detach},
    {"Join", thread_methods_join},
    {Object_NULL, Object_NULL}
};

static void *thread_run(void *ptr) {
    assert(ListObject_CHECK(ptr) && ListObject_SIZE(ptr)==2);
    Object *_lst = Object_CONVERT(ptr);
    Object *_i0 = IntObject_FromInt(0);
    Object *self = Object_CallMethod(_lst, "Get", _i0);
    Object_DECREF(_i0);
    Object *_i1 = IntObject_FromInt(1);
    Object *args = Object_CallMethod(_lst, "Get", _i1);
    Object *ret = ThreadObject_CALLBACK(self)(self, args);
    Object_DECREF(_i1);
    Object_DECREF(_lst);
    return ret;
}

static int thread_init(Object *self, Object *args) {
    Object_Extend(self, &Object_Type, sizeof(Object));
    Object_Init(Object_BASE(self), Object_NULL);
    Object *_lst = ListObject_New(2);
    Object_CallMethod(_lst, "Append", self);
    Object_CallMethod(_lst, "Append", args);
    int ret = pthread_create(&(ThreadObject_TID(self)), NULL, thread_run, (void *)_lst);
    if(ret != 0) {
        fprintf(stderr, "Thread Create Fail !!!\n");
        exit(1);
    }
    return Object_OK;
}

static int thread_deinit(Object *self) {
    return Object_Deinit(Object_BASE(self));
}

static long thread_hash(Object *self) {
    return -1L;
}

static BOOL thread_equal(Object *self, Object *ob) {
    return Object_Equal(Object_BASE(self), Object_BASE(ob));
}

static Object *thread_str(Object *self) {
    return Object_Str(Object_BASE(self));
}

TypeObject Thread_Type = {
    .tp_name = "thread",
    .tp_init = thread_init,
    .tp_deinit = thread_deinit,
    .tp_hash = thread_hash,
    .tp_equal = thread_equal,
    .tp_str = thread_str,
    .tp_methods = thread_methods
};

Object *ThreadObject_New(thread_callback callback, Object *args) {
    Object *_thread = Object_Malloc(&Thread_Type, sizeof(ThreadObject));
    ThreadObject_CALLBACK(_thread) = callback;
    Object_Init(_thread, args);
    return _thread;
}

