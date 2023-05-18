EXE_PATH = build/Game
DEPS = -lSDL2
IGNORE = -Wno-switch

all:
	cc src/*.c -o $(EXE_PATH) $(DEPS) $(IGNORE)
	@$(EXE_PATH)
compile:
	cc src/*.c -o $(EXE_PATH) $(DEPS) $(IGNORE)
run:
	@$(EXE_PATH)
