CFLAGS  = -g -std=c99 -O3

all: lista grafo princ outro

lista:
	gcc $(CFLAGS) -c lista.c -o lista.o

grafo:
	gcc $(CFLAGS) -c grafo.c -o grafo.o

princ:
	gcc $(CFLAGS) -c main.c -o main.o

outro:
	gcc  main.o grafo.o lista.o -o main


clean :
	$(RM) main *.o
