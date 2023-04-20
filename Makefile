all:
	cc src/main.c src/game.c -o build/Game -lSDL2
	build/Game
compile:
	cc src/main.c src/game.c -o Game -lSDL2
run:
	build/Game
