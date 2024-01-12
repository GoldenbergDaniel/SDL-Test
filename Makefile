NAME = UndeadWest
CC = cc

CFLAGS_R = -std=gnu17 \
					 -O0 \
					 -Iext/ \
					 -Iext/sdl2/inc \
					 -Wno-initializer-overrides \

CFLAGS_D = -std=gnu17 \
           -g \
					 -DDEBUG \
					 -I../ext/ \
					 -I../ext/sdl2/inc \
					 -Wall \
					 -Wpedantic \
					 -Wno-missing-braces \
					 -Wno-unused-function \
					 -Wno-initializer-overrides \

LDFLAGS_R = -Lext/sdl2/lib \
						-lsdl2 \

LDFLAGS_D = -L../ext/sdl2/lib \
					  -lsdl2 \

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
	@./ParseShaders
	@cd dbg; \
		$(CC) $(CFLAGS_D) $(LDFLAGS_D) ../src/_target.c -o $(NAME)_D
	@echo "Compilation complete!"
