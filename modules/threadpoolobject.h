#ifndef THREAD_POOL_OBJECT_H
#define THREAD_POOL_OBJECT_H
#ifdef __cplusplus
extern "C" {
#endif

struct _job {
    struct _job *jb_next;
    Object *jb_ob;
};

struct _task {
    int th_idx;
    int th_size;
    struct _job *th_start;
    struct _job *th_end;
    pthread_mutex_t th_lock;
};

typedef struct _threadpoolobject {
    Object_HEAD
    Object *ob_threads;
    struct _task **ob_task;
    BOOL ob_loop;
} ThreadPoolObject;

extern TypeObject ThreadPool_Type;

#define ThreadPoolObject_CHECK(ob) Object_CHECK(ob, &ThreadPool_Type)
#define ThreadPoolObject_CONVERT(ob) ((ThreadPoolObject *)(ob))
#define ThreadPoolObject_THREADS(ob) ThreadPoolObject_CONVERT(ob)->ob_threads
#define ThreadPoolObject_LOOP(ob) ThreadPoolObject_CONVERT(ob)->ob_loop
#define ThreadPoolObject_TASK(ob) ThreadPoolObject_CONVERT(ob)->ob_task

Object *ThreadPoolObject_New(Object *, BOOL);

#ifdef __cplusplus
}
#endif
#endif

