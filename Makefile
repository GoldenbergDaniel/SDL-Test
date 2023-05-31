EXE_PATH = build/Game
DEPS = -lSDL2
IGNORE = -Wno-switch
OPTIMIZATIONS = -O0

all:
	cc src/*.c -o $(EXE_PATH) $(DEPS) $(IGNORE) $(OPTIMIZATIONS)
	@$(EXE_PATH)
compile:
	cc src/*.c -o $(EXE_PATH) $(DEPS) $(IGNORE) $(OPTIMIZATIONS)
run:
	@$(EXE_PATH)
