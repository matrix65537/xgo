CFLAGS = -g -O0
all:
	gcc $(CFLAGS) -c -g -I../include *.c
	ar -rcs libutil.a *.o

debug:
	gcc $(CFLAGS) -c -D__DEBUG__ -g -I../include *.c
	ar -rcs libutil.a *.o
	gcc $(CFLAGS) -g -I../include main.c -L. -lutil
clean:
	-rm *.o *.a *.out *.log *.txt
