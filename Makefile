NAME = Game
LDFLAGS = -lSDL2
CFLAGS = -std=c17 -O0 -Wall -Wpedantic

SOURCES = \
	main.c \
	game.c \
	util.c \
	draw.c \
	entity.c

OBJECTS = $(SOURCES:%.c=build/obj/%.o)

all: $(NAME)
	./$(NAME)

build: $(NAME)

run: 
	./$(NAME)

clean:
	@rm build/obj/*.o
	@echo "Cleaned build directory"

.PHONY: all build run clean

$(NAME): $(OBJECTS)
	@cc $(LDFLAGS) -o $(NAME) $^

$(OBJECTS): build/obj/%.o: src/%.c src/%.h
	@echo $(subst src/,,$<)
	@cc -c $(CFLAGS) -o $@ $<
