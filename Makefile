sti : stvm.o stasm.o main.o
	gcc -o sti stvm.o stasm.o main.o
	@rm *o
    
main.o : main.c
	gcc -c -Wall main.c

stvm.o : stvm.c
	gcc -c -Wall stvm.c

stasm.o : stasm.c
	gcc -c -Wall stasm.c
