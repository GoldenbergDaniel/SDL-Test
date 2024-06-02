#!/bin/bash
set -e

SRC="src/main.c"
OUT="undeadwest"

MODE="dev"
if [[ $1 == "d" ]]; then MODE="debug"; fi
if [[ $1 == "r" ]]; then MODE="release"; fi

ROOT="."
if [[ $MODE == "debug" ]]; then ROOT=".."; fi

if [[ $MODE == "dev"     ]]; then CFLAGS="-std=c17 -O0"; fi
if [[ $MODE == "debug"   ]]; then CFLAGS="-std=c17 -O0 -g -fsanitize=address -fsanitize=undefined -DDEBUG"; fi
if [[ $MODE == "release" ]]; then CFLAGS="-std=c17 -O2 -DRELEASE"; fi

if [[ $MODE == "dev"     ]]; then WFLAGS="-Wpedantic -Wno-initializer-overrides"; fi
if [[ $MODE == "debug"   ]]; then WFLAGS="-Wall -Wpedantic -Wextra -Wno-initializer-overrides -Wno-missing-braces"; fi
if [[ $MODE == "release" ]]; then WFLAGS="-Wno-initializer-overrides"; fi

LFLAGS="-I${ROOT}/extern/ -L${ROOT}/extern/sokol/lib -lsokol -framework OpenGL -framework Cocoa"

shadertoh src/shaders/ src/render/shaders.h

if [[ $MODE == "dev" ]]
then
  echo "Building..."
  cc $CFLAGS $WFLAGS $LFLAGS $SRC -o $OUT
  ./$OUT
fi

if [[ $MODE == "debug" ]]
then
  echo "Building debug..."
  if [[ ! -d "debug" ]]; then mkdir debug; fi
  pushd debug
    cc $CFLAGS $WFLAGS $LFLAGS ../$SRC -o $OUT
  popd
fi

if [[ $MODE == "release" ]]
then
  echo "Building release..."
  cc $CFLAGS $WFLAGS $LFLAGS $SRC -o $OUT
fi
