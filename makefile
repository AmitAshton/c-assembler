mycalc: main.o
	gcc -pedantic -ansi -Wall main.o -o mycalc
main.o: main.c
	gcc -c -pedantic -ansi -Wall main.c -o main.o




