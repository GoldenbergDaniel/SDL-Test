NAME = Game
CC = cc

CFLAGS_R = -std=c17 \
					 -O0 \
					 -Iext/ \
					 -I/usr/local/Cellar/sdl2/2.28.4/include \
					 -Wall \
					 -Wpedantic \
					 -Wno-missing-braces \
					 -Wno-unused-function \

CFLAGS_D = -std=c17 \
					 -DDEBUG \
					 -I../ext/ \
					 -I/usr/local/Cellar/sdl2/2.28.4/include \
					 -Wall \
					 -Wextra \
					 -Wpedantic \
					 -Wno-missing-braces \

LDFLAGS = -lsdl2 \
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
		$(CC) $(CFLAGS_D) $(LDFLAGS) ../ext/glad/glad.c ../src/_target.c -g
	@echo "Compilation complete!"
