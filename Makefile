objects = main.o object.o intobject.o longobject.o floatobject.o doubleobject.o strobject.o

main: $(objects)
	gcc -o main $(objects)

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

.PHONY: clean
clean:
	rm main $(objects)

