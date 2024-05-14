CC=cc
LD_FLAGS="-framework Cocoa -framework QuartzCore -framework OpenGL"
SOURCES="extern/sokol/sokol_app.h extern/sokol/sokol_time.h"

$CC $LD_FLAGS -x objective-c $SOURCES -O2 -shared -o libSokol.dylib
