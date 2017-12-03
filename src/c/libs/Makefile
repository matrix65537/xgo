
all:
	gcc -c -g -I../include *.c
	ar -rcs libutil.a *.o

debug:
	gcc -DDEBUG_UTIL -g -I../include *.c
	ar -rcs libutil.a *.o
clean:
	rm *.o
	rm *.a
