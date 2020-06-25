
FLAGS = -Wall -Werror -g

all : test.bin

boolArrayList.o : boolArrayList.c boolArrayList.h
	gcc -c boolArrayList.c $(FLAGS) -o boolArrayList.o

hintUart.o : hintUart.c hintUart.h
	gcc -c hintUart.c $(FLAGS) -o hintUart.o

pbm.o : pbm.c pbm.h
	gcc -c pbm.c $(FLAGS) -o pbm.o

test.o : test.c
	gcc -c test.c $(FLAGS) -o test.o

test.bin : test.o boolArrayList.o hintUart.o pbm.o
	gcc test.o boolArrayList.o hintUart.o pbm.o $(FLAGS) -o test.bin

test : test.bin
	./test.bin

clean :
	rm -f *.o
	rm -f *.bin
	rm -f *.pbm

