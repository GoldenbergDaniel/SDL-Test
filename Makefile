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
			ext/base/base_os.c \
			ext/base/base_arena.c \
			ext/base/base_math.c \

SRC = src/gfx/gl_render.c \
			src/gfx/draw.c \
			src/global.c \
			src/component.c \
			src/event.c \
			src/entity.c \
			src/game.c \
			src/main.c \

.PHONY: all compile compile_t run test debug combine

all: compile run

compile:
	@echo "Compiling project..."
	@./ParseShaders
	@$(CC) $(CFLAGS) $(LDFLAGS) $(EXT) $(SRC) -o $(NAME)
	@echo "Compilation complete!"

compile_t:
	@echo "Timing compilation..."
	@time $(CC) $(CFLAGS) $(LDFLAGS) $(EXT) $(SRC) -o $(NAME)
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
