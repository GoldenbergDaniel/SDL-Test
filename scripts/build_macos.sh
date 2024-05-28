NAME="undeadwest"
MODE=$1
CC="cc"

CFLAGS="-std=c17 -Iextern/ -Wno-initializer-overrides"
CFLAGS_D="-std=c17 -g -fsanitize=address -fsanitize=undefined
          -I../extern/ -Wall -Wpedantic
          -Wno-initializer-overrides -Wno-missing-braces"

LDFLAGS="-L./extern/sokol -lSokol -framework OpenGL -framework Cocoa"
LDFLAGS_D="-L../extern/sokol -lSokol -framework OpenGL -framework Cocoa"

set -e

shadertoh src/shaders/ src/render/shaders.h

if [[ $MODE == "r" || $MODE == "-r" ]]
then
  echo "Building macOS release..."
  $CC $CFLAGS $LDFLAGS -O2 -DRELEASE src/_target.c -o $NAME
elif [[ $MODE == "d" || $MODE == "-d" ]]
then
  echo "Building macOS debug..."
  mkdir debug
  pushd debug
  $CC $CFLAGS_D $LDFLAGS_D -O0 -DDEBUG ../src/_target.c -o $NAME
  popd
else
  echo "Building macOS..."
  $CC $CFLAGS $LDFLAGS -O0 src/_target.c -o $NAME
  ./$NAME
fi
