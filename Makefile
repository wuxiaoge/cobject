objects = main.o object.o intobject.o longobject.o floatobject.o doubleobject.o \
strobject.o listobject.o ioobject.o stdinobject.o stdoutobject.o stderrobject.o \
fileobject.o sockobject.o threadobject.o sqlite3object.o\
threadpoolobject.o keyvalueobject.o httpheaderobject.o httprequestobject.o \
httpresponseobject.o httpserverobject.o epollobject.o httphandlerobject.o \
notfound.o

main: $(objects)
	gcc -o main $(objects) -lpthread -lsqlite3

main.o: main.c
	gcc -c main.c -I includes
object.o: objects/object.c
	gcc -c objects/object.c -I includes
intobject.o: objects/intobject.c
	gcc -c objects/intobject.c -I includes
longobject.o: objects/longobject.c
	gcc -c objects/longobject.c -I includes
floatobject.o: objects/floatobject.c
	gcc -c objects/floatobject.c -I includes
doubleobject.o: objects/doubleobject.c
	gcc -c objects/doubleobject.c -I includes
strobject.o: objects/strobject.c
	gcc -c objects/strobject.c -I includes
listobject.o: objects/listobject.c
	gcc -c objects/listobject.c -I includes
ioobject.o: objects/ioobject.c
	gcc -c objects/ioobject.c -I includes
stdinobject.o: objects/stdinobject.c
	gcc -c objects/stdinobject.c -I includes
stdoutobject.o: objects/stdoutobject.c
	gcc -c objects/stdoutobject.c -I includes
stderrobject.o: objects/stderrobject.c
	gcc -c objects/stderrobject.c -I includes
fileobject.o: objects/fileobject.c
	gcc -c objects/fileobject.c -I includes
sockobject.o: objects/sockobject.c
	gcc -c objects/sockobject.c -I includes
threadobject.o: objects/threadobject.c
	gcc -c objects/threadobject.c -I includes
sqlite3object.o: objects/sqlite3object.c
	gcc -c objects/sqlite3object.c -I includes

threadpoolobject.o: modules/threadpoolobject.c
	gcc -c modules/threadpoolobject.c -I includes
keyvalueobject.o: modules/keyvalueobject.c
	gcc -c modules/keyvalueobject.c -I includes
httpheaderobject.o: modules/httpheaderobject.c
	gcc -c modules/httpheaderobject.c -I includes
httprequestobject.o: modules/httprequestobject.c
	gcc -c modules/httprequestobject.c -I includes
httpresponseobject.o: modules/httpresponseobject.c
	gcc -c modules/httpresponseobject.c -I includes
httpserverobject.o: modules/httpserverobject.c
	gcc -c modules/httpserverobject.c -I includes -I handlers
epollobject.o: modules/epollobject.c
	gcc -c modules/epollobject.c -I includes
httphandlerobject.o: modules/httphandlerobject.c
	gcc -c modules/httphandlerobject.c -I includes

notfound.o: handlers/notfound.c
	gcc -c handlers/notfound.c -I includes -I modules

.PHONY: clean
clean:
	rm -f main $(objects)

