SRC="src/_entry.c"
OUT="undeadwest"

MODE=$1

CFLAGS="-std=c17 -Iextern/ -Wno-initializer-overrides"
CFLAGS_D="-std=c17 -g -fsanitize=address -fsanitize=undefined
          -I../extern/ -Wall -Wpedantic
          -Wno-initializer-overrides -Wno-missing-braces"

LFLAGS="-L./extern/sokol -lSokol -framework OpenGL -framework Cocoa"
LFLAGS_D="-L../extern/sokol -lSokol -framework OpenGL -framework Cocoa"

set -e

shadertoh src/shaders/ src/render/shaders.h

if [[ $MODE == "r" || $MODE == "-r" ]]
then
  echo "Building macOS release..."
  cc $CFLAGS $LFLAGS -O2 -DRELEASE src/$SRC -o $OUT
elif [[ $MODE == "d" || $MODE == "-d" ]]
then
  echo "Building macOS debug..."
  mkdir debug
  pushd debug
  cc $CFLAGS_D $LFLAGS_D -O0 -DDEBUG ../$SRC -o $OUT
  popd
else
  echo "Building macOS..."
  cc $CFLAGS $LFLAGS -O0 $SRC -o $OUT
  ./$OUT
fi
