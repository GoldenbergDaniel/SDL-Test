run:
	./build/SDLTest
compile:
	cmake --build ./build
test:
	cmake --build ./build
	./build/SDLTest
