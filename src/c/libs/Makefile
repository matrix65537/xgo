CFLAGS = -g -O0 -m32
all:
	gcc $(CFLAGS) -c -g -I../include *.c
	ar -rcs libutil.a *.o

debug:
	gcc $(CFLAGS) -DDEBUG_UTIL -g -I../include *.c
	ar -rcs libutil.a *.o
clean:
	-rm *.o *.a *.out *.log *.txt
