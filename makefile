
FLAGS = -Wall -Werror -g

all : boolArrayList.o hintUart.o

boolArrayList.o : boolArrayList.c boolArrayList.h
	gcc -c boolArrayList.c $(FLAGS) -o boolArrayList.o

hintUart.o : hintUart.c hintUart.h
	gcc -c hintUart.c $(FLAGS) -o hintUart.o

clean :
	rm -f *.o

