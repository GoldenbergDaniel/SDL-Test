NAME = Game
LDFLAGS = -lSDL2
CFLAGS = -std=c17 -O1 -Wall -Wextra -Wpedantic
CC = cc

SOURCES = \
	main.c \
	game.c \
	util.c \
	platform.c \
	component.c \
	entity.c \

OBJECTS = $(SOURCES:%.c=build/obj/%.o)

all: $(NAME)
	./$(NAME)

compile: $(NAME)

recompile: clean $(NAME)
	./$(NAME)

run: 
	./$(NAME)

clean:
	@rm build/obj/*.o
	@echo "Cleaned build directory"

.PHONY: all compile recompile run clean

$(NAME): $(OBJECTS)
	@$(CC) $(LDFLAGS) -o $(NAME) $^

$(OBJECTS): build/obj/%.o: src/%.c
	@$(CC) -c $(CFLAGS) -o $@ $<
	@echo $(subst src/,,$<)
