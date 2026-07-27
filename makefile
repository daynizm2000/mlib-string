all:
	gcc -c src/*.c
	ar rcs libmlib_string.a *.o
	rm *.o