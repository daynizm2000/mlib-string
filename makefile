all:
	gcc -c string/src/*.c
	ar rcs libmlib_string.a *.o
	rm *.o