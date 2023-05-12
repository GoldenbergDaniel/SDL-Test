all:
	cc src/*.c -o build/Game -lSDL2 -lSDL2_image
	build/Game
compile:
	cc src/*.c -o build/Game -lSDL2 -lSDL2_image
run:
	build/Game
