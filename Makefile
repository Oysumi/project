SRC=Jeu.c
HDR=jeu.h
FCT=fonctions.c
BIN=game

$(BIN): $(SRC) $(HDR) $(FCT)
	gcc -Wall -g -std=c99 $(SRC) $(HDR) $(FCT) `sdl-config --cflags --libs` -o $(BIN) -lm

clean:
	rm -f $(BIN)
