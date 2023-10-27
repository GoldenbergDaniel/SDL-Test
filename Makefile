NAME = SpaceGame
CC = cc

CFLAGS = -Iext/ \
				 -std=c17 \
				 -O0 \
				 -Wall \
				 -Wpedantic \
				 -Wno-missing-braces \
				 -Wno-unused-function \

LDFLAGS = -framework OpenGL \
					-lsdl2 \

EXT = ext/glad/glad.c \

.PHONY: all compile compile_t run test debug

all: compile run

compile:
	@echo "Compiling project..."
	@./ParseShaders
	@$(CC) $(CFLAGS) $(LDFLAGS) $(EXT) src/_target.c -o $(NAME)
	@echo "Compilation complete!"

compile_t:
	@echo "Timing compilation..."
	@time $(CC) $(CFLAGS) $(LDFLAGS) $(EXT) src/_target.c -o $(NAME)

run:
	./$(NAME)

test:
	@echo "Compiling test..."
	@$(CC) $(CFLAGS) test/test.c src/base_math.c -o Test1
	./Test1

debug:
	@echo "Compiling debug..."
	@cd debug; \
	$(CC) -I../lib/ $(LDFLAGS) ../lib/glad/glad.c ../src/*.c -g
	@echo "Compilation complete!"
