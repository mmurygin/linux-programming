all: exe

exe: main.c lib
	gcc main.c -ldl -o hello

lib: hello.c
	gcc -shared -fPIC hello.c -o libHello.so

clean:
	rm -f hello *.so *.o
