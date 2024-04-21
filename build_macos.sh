NAME="undeadwest"
CC="cc"
MODE=$1

CFLAGS_R="-std=c17 
          -Iextern/ -Iextern/sdl3/include 
          -Wno-initializer-overrides -Wno-static-in-inline"

LDFLAGS_R="-L./ -lsdl3"

CFLAGS_D="-std=c17 -g -DDEBUG -fsanitize=address -fsanitize=undefined
          -I../extern/ -I../extern/sdl3/include -Wall -Wpedantic
          -Wno-initializer-overrides -Wno-static-in-inline -Wno-missing-braces"

LDFLAGS_D="-L../ -lsdl3"

./ShaderToC

if [[ $MODE == "r" || $MODE == "-r" ]]
then
  echo "Building macOS release..."
  $CC $CFLAGS_R $LDFLAGS_R -O2 src/_target.c -o $NAME
elif [[ $MODE == "d" || $MODE == "-d" ]]
then
  echo "Building macOS debug..."
  mkdir debug
  cp libSDL3.dylib debug/libSDL3.dylib
  pushd debug
  $CC $CFLAGS_D $LDFLAGS_D -O0 ../src/_target.c -o $NAME
  popd
else
  echo "Building macOS..."
  $CC $CFLAGS_R $LDFLAGS_R -O0 src/_target.c -o $NAME
  ./$NAME
fi

echo "Done!"
