NAME = UndeadWest
CC = cc

CFLAGS_R = -std=gnu17 \
					 -O0 \
					 -Iextern/ \
					 -Iextern/sdl2/inc \
					 -Wno-initializer-overrides \
					 -Wno-static-in-inline \

CFLAGS_D = -std=gnu17 \
           -g \
					 -DDEBUG \
					 -I../extern/ \
					 -I../extern/sdl2/inc \
					 -Wall \
					 -Wpedantic \
					 -Wno-missing-braces \
					 -Wno-unused-function \
					 -Wno-initializer-overrides \
					 -Wno-static-in-inline \

LDFLAGS_R = -Lextern/sdl2/lib \
						-lsdl2 \

LDFLAGS_D = -L../extern/sdl2/lib \
					  -lsdl2 \

.PHONY: all compile_r compile_d

all: compile_r
	./$(NAME)

compile_r:
	@echo "Compiling project..."
	@./ShaderToC
	@$(CC) $(CFLAGS_R) $(LDFLAGS_R) src/_target.c -o $(NAME)
	@echo "Compilation complete!"

compile_d:
	@echo "Compiling debug..."
	@./ShaderToC
	@cd debug; \
		$(CC) $(CFLAGS_D) $(LDFLAGS_D) ../src/_target.c -o $(NAME)_D
	@echo "Compilation complete!"
