NAME = SpaceGame
CC = cc

CFLAGS = -Iextern/ \
				 -std=c17 -O1 \
				 -Wall -Wextra -Wpedantic \
				 -Wno-missing-braces \
				 -Wno-unused-parameter \
				 -Wno-unused-function \

LDFLAGS = -framework OpenGL \
					-lsdl2 \

EXTERN = extern/glad/glad.c \
				 extern/base/base_os.c \
				 extern/base/base_arena.c \
				 extern/base/base_math.c \

SRC = src/gfx/gl_render.c \
			src/gfx/draw.c \
			src/component.c \
			src/entity.c \
			src/game.c \
			src/main.c \

.PHONY: all compile compile_t run test debug combine

all: compile run

compile:
	@echo "Compiling project..."
	@./ParseShaders
	@$(CC) $(CFLAGS) $(LDFLAGS) $(EXTERN) $(SRC) -o $(NAME)
	@echo "Compilation complete!"

compile_t:
	@echo "Compiling timed compilation..."
	@time $(CC) $(CFLAGS) $(LDFLAGS) $(EXTERN) $(SRC) -o $(NAME)
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
