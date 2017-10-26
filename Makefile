CFLAGS  = -g -std=c99

all: lista mapa grafo princ outro

lista: 
	gcc $(CFLAGS) -c lista.c -o lista.o

mapa: 
	gcc $(CFLAGS) -c mapa.c -o mapa.o

grafo: 
	gcc $(CFLAGS) -c grafo.c -o grafo.o

princ:
	gcc $(CFLAGS) -c main.c -o main.o 

outro: 
	gcc  main.o grafo.o mapa.o lista.o -o main


clean :
	$(RM) main *.o 