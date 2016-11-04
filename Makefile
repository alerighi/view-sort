CC=gcc
CFLAGS=-O3 -lpthread -lSDL2
SRC=view_sort.c
EXE=view_sort
RM=rm -rf

all: $(EXE)

$(EXE): $(SRC)
	$(CC) $(SRC) -o $(EXE) $(CFLAGS)

clean:
	$(RM) $(EXE)
