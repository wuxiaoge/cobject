#ifndef NOTFOUND_H
#define NOTFOUND_H
#ifdef __cplusplus
extern "C" {
#endif

typedef struct _notfoundhandler {
    Object_HEAD
} NotFoundHandler;

extern TypeObject NotFoundHandler_Type;

#define NotFoundHandler_CHECK(ob) Object_CHECK(ob, &NotFoundHandler_Type)
#define NotFoundHandler_CONVERT(ob) ((NotFoundHandler *)(ob))

Object *NotFoundHandler_New();

#define NOT_FOUND_404_CONTENT "<!DOCTYPE html>" \
"<html>" \
    "<head>" \
        "<title>404 Not Found</title>" \
    "</head>" \
    "<body>" \
        "<h1>404 Not Found</h1>" \
    "</body>" \
"</html>"

#ifdef __cplusplus
}
#endif
#endif
