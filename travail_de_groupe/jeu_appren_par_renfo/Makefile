CC=gcc

LDFLAG=$(shell sdl2-config --cflags --libs) -lm -lSDL2_ttf -D_REENTRANT -lSDL2_image -pthread
CFLAG=-Wall $(shell sdl2-config --cflags --libs)

EXEC=run
SRC=$(wildcard src/*.c)
OBJ=$(SRC:.c=.o)

all:$(EXEC)

$(EXEC):$(OBJ)
	$(CC) -o $@ $^ $(LDFLAG)
	mv $^ bin/

%.o:%.c
	$(CC) -o $@ -c $< $(CFLAG)

.PHONY:clean 

clean:
	rm -rf bin/*.o
	rm -rf $(EXEC) 