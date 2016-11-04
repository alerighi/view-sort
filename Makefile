CC=gcc
CFLAGS=-O3 -lpthread -lSDL2
SRC=view_sort.c
EXE=view_sort

all: ${EXE}

${EXE}: ${SRC}
	${CC} view_sort.c -o ${EXE} ${CFLAGS}

clean:
	rm -rf ${EXE}
