CC=gcc
CFLAGS=-Wextra -Wall -lm -lSDL2
LDFLAGS=
EXEC=run

all:$(EXEC)

run: main.o render.o map.o
	$(CC) -o $@ $^ $(LDFLAGS)
	@echo "Lancer le programme avec ./run"

map.o : src/map.c src/map.h 
	$(CC) -o $@ -c $< $(CFLAGS)

render.o : src/render.c src/render.h 
	$(CC) -o $@ -c $< $(CFLAGS)

main.o : src/main.c
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -rf *.o

rmExec: clean
	rm -rf $(EXEC)