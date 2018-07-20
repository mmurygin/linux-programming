all: exe

lib: solution.h solution.c
	gcc -shared -fPIC solution.c -o libSolution.so

exe: main.c lib
	gcc main.c -L. -lSolution -o strstat

clean:
	rm -f strstat *.so *.o
