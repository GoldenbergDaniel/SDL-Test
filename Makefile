NAME = Game
CC = clang

CFLAGS_R = -std=gnu17 \
					 -O0 \
					 -Iext/ \
					 -Iext/sdl2/inc \
					 -Wall \
					 -Wpedantic \
					 -Wno-missing-braces \
					 -Wno-unused-function \
					 -Wno-language-extension-token \

CFLAGS_D = -std=gnu17 \
					 -DDEBUG \
					 -I../ext/ \
					 -Wall \
					 -Wextra \
					 -Wpedantic \
					 -Wno-missing-braces \
					 -Wno-language-extension-token \

LDFLAGS = -Lext/sdl2/lib \
					-lsdl2 \
					ext/glad/glad.c \

.PHONY: all compile compile_t run test debug

all: compile run

compile:
	@echo "Compiling project..."
	@./ParseShaders
	@$(CC) $(CFLAGS_R) $(LDFLAGS) src/_target.c -o $(NAME)
	@echo "Compilation complete!"

compile_t:
	@echo "Timing compilation..."
	@time $(CC) $(CFLAGS_R) $(LDFLAGS) src/_target.c -o $(NAME)

run:
	./$(NAME)

test:
	@echo "Compiling test..."
	@$(CC) $(CFLAGS_R) test/test.c src/base_math.c -o Test1
	./Test1

debug:
	@echo "Compiling debug..."
	@cd debug; \
		$(CC) $(CFLAGS_D) -lsdl2 ../ext/glad/glad.c ../src/_target.c -g
	@echo "Compilation complete!"
