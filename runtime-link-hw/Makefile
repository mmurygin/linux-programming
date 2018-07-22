all: exe

exe: solution.c lib
	gcc solution.c -ldl -o solution

lib: multiplier.c
	gcc -shared -fPIC multiplier.c -o libMultiplier.so

clean:
	rm -f solution *.so *.o
