NAME = Game
CC = clang

CFLAGS_R = -std=gnu17 \
					 -O0 \
					 -Iext/ \
					 -Iext/sdl2/inc \
					 -Wall \
					 -Wpedantic \
					 -Wno-language-extension-token \
					 -Wno-missing-braces \
					 -Wno-unused-function \
					 -Wno-initializer-overrides \

CFLAGS_D = -std=gnu17 \
					 -DEBUG \
					 -DDEBUG \
					 -I../ext/ \
					 -I../ext/sdl2/inc \
					 -Wall \
					 -Wextra \
					 -Wpedantic \
					 -Wno-language-extension-token \
					 -Wno-missing-braces \
					 -Wno-unused-function \
					 -Wno-unused-parameter \
					 -Wno-initializer-overrides \

LDFLAGS = -Lext/sdl2/lib \
					-lsdl2 \
					ext/glad/glad.c \

.PHONY: all compile compile_t run debug

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

debug:
	@echo "Compiling debug..."
	@cd dbg; \
		$(CC) $(CFLAGS_D) -L../ext/sdl2/lib -lsdl2 ../ext/glad/glad.c ../src/_target.c -g
	@echo "Compilation complete!"
