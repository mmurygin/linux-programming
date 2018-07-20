all: exe

exe: main.c hello.h lib
	gcc main.c -L. -lHello -o hello

lib: hello.h hello.c
	gcc -shared -fPIC hello.c -o libHello.so

clean:
	rm -f hello *.so *.o
