CC=gcc 

main:main.o
	$(CC) -o main main.o -lm -lSDL2
	@echo "=> Lancer le programme avec  ./main" 

main.o:main.c
	$(CC) -c main.c -g -Wall -Wextra
clean:
	rm -rf main main.o