NAME = SpaceGame
CC = cc

CFLAGS = -Ilib/ \
				 -std=c17 -O1 \
				 -Wall -Wextra -Wpedantic \
				 -Wno-missing-braces \
				 -Wno-unused-function

LDFLAGS = -framework OpenGL \
					-lsdl2 \

LIB = lib/glad/glad.c \

SRC = src/main.c \
			src/base_os.c \
			src/base_arena.c \
			src/base_math.c \
			src/render.c \
			src/draw.c \
			src/util.c \
			src/game.c \
			src/entity.c

.PHONY: all compile compile_t run test debug combine

all: compile run

compile:
	@echo "Compiling project..."
	@./ParseShaders
	@$(CC) $(CFLAGS) $(LDFLAGS) $(LIB) $(SRC) -o $(NAME)
	@echo "Compilation complete!"

compile_t:
	@echo "Compiling timed compilation..."
	@time $(CC) $(CFLAGS) $(LDFLAGS) $(LIB) $(SRC) -o $(NAME)
	@echo "Compilation complete!"

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

combine: $(SRC)
	cat $^ > project.c
