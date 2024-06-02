SOURCES="sokol_app.h sokol_audio.h sokol_time.h sokol_log.h"
DEFINES="-DSOKOL_IMPL -DSOKOL_NO_ENTRY -DSOKOL_GLCORE"

pushd extern/sokol
cc $DEFINES -x objective-c $SOURCES -O2 -c
ar -crs lib/libsokol.a sokol_app.o sokol_audio.o sokol_time.o sokol_log.o
