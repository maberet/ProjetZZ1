CC=gcc 

main:main.o
	$(CC) -o main main.o -lm -lSDL2
	@echo "=> Lancer le programme avec :  ./main --taille --fichier" 
	@echo "=> Flèche de droite : avance la simulation" 
	@echo "=> Touche c : vide la grille" 
	@echo "exemple : ./main (grille de 20x20)" 
	@echo "exemple : ./main 30 (grille de 30x30)" 
	@echo "exemple : ./main 20 niveau.txt (grille de 20x20 chargé depuis niveau.txt)" 

main.o:main.c
	$(CC) -c main.c -g -Wall -Wextra

clean:
	rm -rf main main.o
