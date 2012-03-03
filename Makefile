LIBS = -lSDL -lSDL_image -lSDL_mixer -lSDL_ttf 
FLAGS = -pedantic -O2 -Wall
CFILES  = gk.c gameobject.c camera.c background.c gameworld.c surfacehandler.c
HFILES  = gk.h gameobject.h camera.h background.h gameworld.h surfacehandler.h

gk: ${CFILES} ${HFILES} 
	g++ ${CFILES} -o gk ${LIBS}  ${FLAGS}  


gkdebug: ${CFILES} ${HFILES} 
	g++ -g ${CFILES} -o gkdebug ${LIBS}  -ggdb

Wnone: ${CFILES} ${HFILES} 
	cc ${CFILES} -o gk ${LIBS} -pedantic -O2 -std=c99

clean:
	rm gk

