#include "object.h"
#include "threadpoolobject.h"
#include <pthread.h>
#include <unistd.h>

static int threads_foreach_detach(Object *index, Object *item) {
    pthread_detach(ThreadObject_TID(item));
    return Object_OK;
}

static Object *threadpool_methods_detach(Object *self, Object *args) {
    Object_CallMethod(ThreadPoolObject_THREADS(self), "Foreach",
                      Object_CONVERT(threads_foreach_detach));
    return Object_NULL;
}

static Object *threads_map_join(Object *index, Object *item) {
    Object *ret = Object_NULL;
    pthread_join(ThreadObject_TID(item), (void **)&ret);
    return ret;
}

static Object *threadpool_methods_join(Object *self, Object *args) {
    Object *ret = Object_CallMethod(ThreadPoolObject_THREADS(self),
                      "Map", Object_CONVERT(threads_map_join));
    return ret;
}

static Object *threadpool_methods_add(Object *self, Object *ob) {
    static int _task_index = 0;
    assert(ob);
    struct _task *task = *(ThreadPoolObject_TASK(self) + _task_index);
    _task_index = (++_task_index) % ListObject_SIZE(ThreadPoolObject_THREADS(self));
    struct _job *job = (struct _job *)malloc(sizeof(struct _job));
    job->jb_next = NULL;
    job->jb_ob = ob;
    Object_INCREF(ob);
    pthread_mutex_lock(&(task->th_lock));
    if(task->th_end) {
        task->th_end->jb_next = job;
        task->th_end = job;
    } else {
        task->th_start = job;
        task->th_end = task->th_start;
    }
    pthread_mutex_unlock(&(task->th_lock));
    return Object_NULL;
}

static MethodDef threadpool_methods[] = {
    {"Detach", threadpool_methods_detach},
    {"Join", threadpool_methods_join},
    {"Add", threadpool_methods_add},
    {Object_NULL, Object_NULL}
};

static Object *threadpool_callback(Object *thread, Object *args) {
    assert(ListObject_CHECK(args) && ListObject_SIZE(args)==2);
    Object *_i0 = IntObject_FromInt(0);
    Object *pool = Object_CallMethod(args, "Get", _i0);
    Object_DECREF(_i0);
    Object *_i1 = IntObject_FromInt(1);
    Object *thread_idx = Object_CallMethod(args, "Get", _i1);
    Object_DECREF(_i1);
    int ithread_idx = IntObject_AsINT(thread_idx);
    struct _task *task = *(ThreadPoolObject_TASK(pool) + ithread_idx);
    struct _job *job = NULL;
    Object *ret = Object_NULL;
    do {
        int _status = pthread_mutex_trylock(&(task->th_lock));
        if(_status) {continue;}
        job = task->th_start;
        if(job) {
            if(job == task->th_end) {
                task->th_start = NULL;
                task->th_end = task->th_start;
            } else {
                task->th_start = job->jb_next;
            }
        }
        pthread_mutex_unlock(&(task->th_lock));
        if(job) {
            printf("thread %d : ==>\n", ithread_idx);
            ret = Object_CallMethod(job->jb_ob, "Run", Object_NULL);
            if(!ThreadPoolObject_LOOP(pool)) {
                Object_DECREF(ret);
            }
            Object_DECREF(job->jb_ob);
            free(job);
        }
    } while(ThreadPoolObject_LOOP(pool));
    return ret;
}

static int threadpool_init(Object *self, Object *args) {
    Object_Extend(self, &Object_Type, sizeof(Object));
    Object_Init(Object_BASE(self), Object_NULL);
    assert(ListObject_CHECK(args) && ListObject_SIZE(args)==2);
    Object *i0 = IntObject_FromInt(0);
    Object *size = Object_CallMethod(args, "Get", i0);
    Object_DECREF(i0);
    Object *i1 = IntObject_FromInt(1);
    Object *loop = Object_CallMethod(args, "Get", i1);
    Object_DECREF(i1);
    // 生成线程集合
    Object *lst = ListObject_New(size);
    ThreadPoolObject_THREADS(self) = lst;
    ThreadPoolObject_LOOP(self) = IntObject_AsINT(loop);
    int isize = IntObject_AsINT(size);
    if(isize < 1) {
        isize = sysconf(_SC_NPROCESSORS_ONLN);
    }
    ThreadPoolObject_TASK(self) = (struct _task **)malloc(sizeof(struct _task *) * isize);
    Object *thread = Object_NULL;
    Object *_args = Object_NULL;
    Object *thread_idx = Object_NULL;
    Object *arg_size = IntObject_FromInt(2);
    struct _task *task = NULL;
    int i = 0;
    for(; i < isize; i++){
        _args = ListObject_New(arg_size);
        Object_CallMethod(_args, "Append", self);
        thread_idx = IntObject_FromInt(i);
        Object_CallMethod(_args, "Append", thread_idx);
        Object_DECREF(thread_idx);
        thread = ThreadObject_New(threadpool_callback, _args);
        Object_DECREF(_args);
        Object_CallMethod(lst, "Append", thread);
        Object_DECREF(thread);
        task = (struct _task *)malloc(sizeof(struct _task));
        task->th_idx = i;
        task->th_size = 0;
        task->th_start = NULL;
        task->th_end = NULL;
        pthread_mutex_init(&(task->th_lock), NULL);
        *(ThreadPoolObject_TASK(self) + i) = task;
    }
    Object_DECREF(arg_size);
    return Object_OK;
}

static int threadpool_deinit(Object *self) {
    struct _task *task = NULL;
    struct _job *job = NULL;
    struct _job *tmp = NULL;
    int isize = ListObject_SIZE(ThreadPoolObject_THREADS(self));
    Object_DECREF(ThreadPoolObject_THREADS(self));
    int i = 0;
    for(; i < isize; i++) {
        task = *(ThreadPoolObject_TASK(self) + i);
        job = task->th_start;
        tmp = NULL;
        while(job) {
            tmp = job->jb_next;
            Object_DECREF(job->jb_ob);
            free(job);
            job = tmp;
        }
        pthread_mutex_destroy(&(task->th_lock));
        free(task);
    }
    free(ThreadPoolObject_TASK(self));
    return Object_Deinit(Object_BASE(self));
}

static long threadpool_hash(Object *self) {
    return -1L;
}

static BOOL threadpool_equal(Object *self, Object *ob) {
    return Object_Equal(Object_BASE(self), Object_BASE(ob));
}

static Object *threadpool_str(Object *self) {
    return Object_Str(Object_BASE(self));
}

TypeObject ThreadPool_Type = {
    .tp_name = "threadpool",
    .tp_init = threadpool_init,
    .tp_deinit = threadpool_deinit,
    .tp_hash = threadpool_hash,
    .tp_equal = threadpool_equal,
    .tp_str = threadpool_str,
    .tp_methods = threadpool_methods
};

Object *ThreadPoolObject_New(Object *pool_size, BOOL loop) {
    Object *_threadpool = Object_Malloc(&ThreadPool_Type, sizeof(ThreadPoolObject));
    Object *two = IntObject_FromInt(2);
    Object *lst = ListObject_New(two);
    Object_DECREF(two);
    Object_CallMethod(lst, "Append", pool_size);
    Object *is_loop = IntObject_FromInt(loop);
    Object_CallMethod(lst, "Append", is_loop);
    Object_DECREF(is_loop);
    Object_Init(_threadpool, lst);
    Object_DECREF(lst);
    return _threadpool;
}

