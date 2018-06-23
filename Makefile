CFLAGS  = -g -std=c99 -O3
LDLIBS = -l hiredis -l readline

all: campo lista grafo princ outro

campo:
	gcc $(CFLAGS) -c campo.c -o campo.o $(LDLIBS)

lista:
	gcc $(CFLAGS) -c lista.c -o lista.o

grafo:
	gcc $(CFLAGS) -c grafo.c -o grafo.o

princ:
	gcc $(CFLAGS) -c main.c -o main.o $(LDLIBS)

outro:
	gcc  main.o grafo.o lista.o campo.o -o main $(LDLIBS)


clean :
	$(RM) main *.o
