CFLAGS = -g -O0 -m32

all:
	gcc $(CFLAGS) *.c -I../include -L../libs -lutil -lcrypt -o auth
	sudo chown root auth
	sudo chgrp root auth
	sudo chmod u+s auth
	sudo chmod g+s auth
clean:
	-rm *.o *.a *.out *.txt *.log
