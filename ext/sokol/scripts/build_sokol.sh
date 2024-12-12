#!/bin/bash

SOURCES="sokol_app.h sokol_audio.h sokol_time.h"
DEFINES="-DSOKOL_IMPL -DSOKOL_NO_ENTRY -DSOKOL_GLCORE"

TARGET="linux_amd64"
if [[ $1 != "" ]]; then TARGET=$2; fi

if [[ $TARGET == "darwin_amd64" ]]; 
then
  cc $DEFINES -x objective-c $SOURCES -O2 -c
  ar -crs lib/libsokol_darwin.a sokol_app.o sokol_audio.o sokol_time.o
  rm -f *.o
fi

if [[ $TARGET == "linux_amd64" ]]; 
then
  cc $DEFINES -x c $SOURCES -O2 -c
  ar -crs lib/libsokol_linux.a sokol_app.o sokol_audio.o sokol_time.o
  rm -f *.o
fi
