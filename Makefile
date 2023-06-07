NAME = Game
LDFLAGS = -lSDL2
CFLAGS = -std=c17 -O0 -Wall -Wno-switch -Wpedantic

SOURCES = \
	main.c \
	game.c \
	util.c \
	draw.c \
	player.c \
	enemy.c

OBJECTS = $(SOURCES:%.c=build/obj/%.o)

all: $(NAME)
	build/$(NAME)

build: $(NAME)

clean:
	@rm build/obj/*.o
	@echo "Cleaned build directory"

.PHONY: all build clean

$(NAME): $(OBJECTS)
	@cc $(LDFLAGS) -o build/$(NAME) $^

$(OBJECTS): build/obj/%.o: src/%.c
	@echo $(subst src/,,$<)
	@cc -c $(CFLAGS) -o $@ $<
