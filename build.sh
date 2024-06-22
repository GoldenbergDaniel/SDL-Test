#!/bin/bash
set -e

SRC="src/main.c"
OUT="undeadwest"

MODE="dev"
if [[ $1 == "d" || $1 == "debug"   ]]; then MODE="debug"; fi
if [[ $1 == "r" || $1 == "release" ]]; then MODE="release"; fi
if [[ $1 == "g" || $1 == "git"     ]]; then MODE="git"; fi

if [[ $1 == "fsan" ]]; then FSAN="-fsanitize=undefined"; fi

if [[ $MODE == "git" ]];
then
  git add .
  git status
  git commit -m "$2"
  git push
  exit 0
fi

if [[ $MODE == "dev"     ]]; then CFLAGS="-std=c17 -O0"; fi
if [[ $MODE == "debug"   ]]; then CFLAGS="-std=c17 -Og -g -DDEBUG"; fi
if [[ $MODE == "release" ]]; then CFLAGS="-std=c17 -O2 -DRELEASE"; fi

if [[ $MODE == "dev"     ]]; then WFLAGS="-Wpedantic -Wno-initializer-overrides"; fi
if [[ $MODE == "debug"   ]]; then WFLAGS="-Wall -Wpedantic -Wno-initializer-overrides -Wno-missing-braces"; fi
if [[ $MODE == "release" ]]; then WFLAGS="-Wno-initializer-overrides"; fi

LFLAGS="-I../extern/ -L../extern/sokol/lib -lsokol -framework OpenGL -framework Cocoa"

echo "[mode:$MODE]"
if [[ $2 == "fsan" ]]; then echo "[fsan:on]"; fi
if [[ $2 != "fsan" ]]; then echo "[fsan:off]"; fi

# echo "- Processing -"
shadertoh src/shaders/ src/render/shaders.h

BUILD="0"
if [[ $MODE == "dev" || $MODE == "debug" || $MODE == "release" ]]; then BUILD="1"; fi

RUN="0"
if [[ $MODE == "dev" ]]; then RUN="1"; fi

# -target arm64-apple-macos14
if [[ $BUILD == "1" ]]
then
  echo "- Building -"
  echo $SRC
  if [[ ! -d "build" ]]; then mkdir build; fi
  pushd build
    cc $CFLAGS $WFLAGS $FSAN $LFLAGS ../$SRC -o $OUT
    if [[ $RUN == "1" ]]; then ./$OUT; fi
  popd
fi
