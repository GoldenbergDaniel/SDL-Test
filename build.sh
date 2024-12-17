#!/bin/bash
set -e

# --- CONFIGURE ---------------------------------------------------------------------

MODE="dev"
if [[ $1 != "" ]]; then MODE=$1; fi
if [[ $MODE != "dev" && $MODE != "debug" && $MODE != "release" ]]; then
  echo "Failed to build. '$MODE' is not valid mode."
  exit 1
fi

TARGET="linux_amd64"
if [[ $2 != "" ]]; then TARGET=$2; fi
if [[ $TARGET != "darwin_amd64" && $TARGET != "darwin_arm64" && $TARGET != "linux_amd64" ]]; then
  echo "Failed to build. '$TARGET' is not a valid target."
  exit 1
fi

CFLAGS="-std=gnu17 -Iext"
if [[ $MODE == "dev"     ]]; then CFLAGS="$CFLAGS -O0 -DDEBUG"; fi
if [[ $MODE == "debug"   ]]; then CFLAGS="$CFLAGS -Og -g -DDEBUG"; fi
if [[ $MODE == "release" ]]; then CFLAGS="$CFLAGS -O2 -fno-strict-aliasing"; fi

WFLAGS=""
if [[ $MODE == "debug"   ]]; then WFLAGS="-Wall -Wno-missing-braces"; fi

LFLAGS="-Lext/sokol/lib"
if [[ $TARGET == "darwin_amd64"  ]]; then LFLAGS="$LFLAGS -lsokol_darwin -framework OpenGL -framework Cocoa"; fi
if [[ $TARGET == "linux_amd64"   ]]; then LFLAGS="$LFLAGS -lsokol_linux -lX11 -lXi -lXcursor -lEGL -lGL -ldl -lpthread -lm"; fi

echo "[target:$TARGET]"
echo "[mode:$MODE]"

# --- PREPROCESS --------------------------------------------------------------------

# echo "[preprocess]"

# shadertoh src/shaders/ src/render/shaders.h

# --- BUILD -------------------------------------------------------------------------
# -target arm64-apple-macos14

echo "[build]"

if [[ ! -d "out" ]]; then mkdir out; fi
cc src/main.c -o out/undeadwest $CFLAGS $WFLAGS $LFLAGS
if [[ $MODE == "dev" ]]; then out/undeadwest; fi
