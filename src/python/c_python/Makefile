all:
	gcc -o libc1.so c1.c -shared -fPIC 
	gcc -o libpythonso.so pythonso.c -shared -fPIC -I/usr/include/python2.7 -L/usr/lib/python2.7/config -lpython2.7
	gcc -o callpy main.c -L. -lpythonso

	sudo mv libpythonso.so /usr/lib
