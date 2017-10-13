CFLAGS = -g -Wall -Wno-unused-result 

all: floodit floodit_exaustivo util heuristicas

util: geramapa verifica anima

heuristicas: floodit_h1 floodit_h2 floodit_h3 floodit_h4 floodit_h5 floodit_h6

floodit: mapa.h mapa.c floodit.c

floodit_exaustivo: mapa.h mapa.c floodit_exaustivo.c

floodit_h1: mapa.h mapa.c floodit_h1.c

floodit_h2: mapa.h mapa.c floodit_h2.c

floodit_h3: mapa.h mapa.c floodit_h3.c

floodit_h4: mapa.h mapa.c floodit_h4.c

floodit_h5: mapa.h mapa.c floodit_h5.c

floodit_h6: mapa.h mapa.c floodit_h6.c

geramapa: mapa.h mapa.c geramapa.c

verifica: mapa.h mapa.c verifica.c

anima: mapa.h mapa.c anima.c

clean:
	rm -f floodit floodit_exaustivo geramapa verifica anima *.o
	rm -f floodit_h1 floodit_h2 floodit_h3 floodit_h4 floodit_h5 floodit_h6
