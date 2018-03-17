CFLAGS = -g -O0 -m32

all:
	gcc $(CFLAGS) *.c -I../include -L../libs -lutil -o hello
	sudo chown root hello
	sudo chgrp root hello
	sudo chmod u+s hello
	sudo chmod g+s hello
clean:
	-rm *.o *.a *.out *.txt *.log
