NAME = Game
CC = cc

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
					 -DDEBUG \
					 -I../ext/ \
					 -I../ext/sdl2/inc \
					 -Wall \
					 -Wextra \
					 -Wpedantic \
					 -Wno-language-extension-token \
					 -Wno-missing-braces \
					 -Wno-initializer-overrides \

LDFLAGS_R = -Lext/sdl2/lib \
						-lsdl2 \
						ext/glad/glad.c \

LDFLAGS_D = -L../ext/sdl2/lib \
					  -lsdl2 \
						../ext/glad/glad.c \

.PHONY: all compile_r compile_d

all: compile_r
	./$(NAME)

compile_r:
	@echo "Compiling project..."
	@./ParseShaders
	@$(CC) $(CFLAGS_R) $(LDFLAGS_R) src/_target.c -o $(NAME)
	@echo "Compilation complete!"

compile_d:
	@echo "Compiling debug..."
	@cd dbg; \
		$(CC) $(CFLAGS_D) -L../ext/sdl2/lib -lsdl2 ../ext/glad/glad.c ../src/_target.c -g
	@echo "Compilation complete!"
