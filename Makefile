objects = main.o object.o intobject.o longobject.o floatobject.o doubleobject.o \
strobject.o listobject.o ioobject.o stdinobject.o stdoutobject.o stderrobject.o \
fileobject.o sockobject.o threadobject.o \
threadpoolobject.o keyvalueobject.o httpheaderobject.o httprequestobject.o \
httpresponseobject.o httpserverobject.o epollobject.o httphandlerobject.o \
notfound.o

main: $(objects)
	gcc -o main $(objects) -lpthread

main.o: main.c includes/object.h
	gcc -c main.c -I includes
object.o: objects/object.c includes/object.h
	gcc -c objects/object.c -I includes
intobject.o: objects/intobject.c includes/object.h
	gcc -c objects/intobject.c -I includes
longobject.o: objects/longobject.c includes/object.h
	gcc -c objects/longobject.c -I includes
floatobject.o: objects/floatobject.c includes/object.h
	gcc -c objects/floatobject.c -I includes
doubleobject.o: objects/doubleobject.c includes/object.h
	gcc -c objects/doubleobject.c -I includes
strobject.o: objects/strobject.c includes/object.h
	gcc -c objects/strobject.c -I includes
listobject.o: objects/listobject.c includes/object.h
	gcc -c objects/listobject.c -I includes
ioobject.o: objects/ioobject.c includes/object.h
	gcc -c objects/ioobject.c -I includes
stdinobject.o: objects/stdinobject.c includes/object.h
	gcc -c objects/stdinobject.c -I includes
stdoutobject.o: objects/stdoutobject.c includes/object.h
	gcc -c objects/stdoutobject.c -I includes
stderrobject.o: objects/stderrobject.c includes/object.h
	gcc -c objects/stderrobject.c -I includes
fileobject.o: objects/fileobject.c includes/object.h
	gcc -c objects/fileobject.c -I includes
sockobject.o: objects/sockobject.c includes/object.h
	gcc -c objects/sockobject.c -I includes
threadobject.o: objects/threadobject.c includes/object.h
	gcc -c objects/threadobject.c -I includes

threadpoolobject.o: modules/threadpoolobject.c includes/object.h
	gcc -c modules/threadpoolobject.c -I includes
keyvalueobject.o: modules/keyvalueobject.c includes/object.h
	gcc -c modules/keyvalueobject.c -I includes
httpheaderobject.o: modules/httpheaderobject.c includes/object.h
	gcc -c modules/httpheaderobject.c -I includes
httprequestobject.o: modules/httprequestobject.c includes/object.h
	gcc -c modules/httprequestobject.c -I includes
httpresponseobject.o: modules/httpresponseobject.c includes/object.h
	gcc -c modules/httpresponseobject.c -I includes
httpserverobject.o: modules/httpserverobject.c includes/object.h
	gcc -c modules/httpserverobject.c -I includes -I handlers
epollobject.o: modules/epollobject.c includes/object.h
	gcc -c modules/epollobject.c -I includes
httphandlerobject.o: modules/httphandlerobject.c includes/object.h
	gcc -c modules/httphandlerobject.c -I includes

notfound.o: handlers/notfound.c includes/object.h
	gcc -c handlers/notfound.c -I includes -I modules

.PHONY: clean
clean:
	rm -f main $(objects)

