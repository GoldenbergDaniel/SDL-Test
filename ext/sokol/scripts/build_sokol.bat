@echo off
setlocal

set SOURCES= sokol_app.c sokol_audio.c sokol_time.c 
set DEFINES= /DSOKOL_IMPL /DSOKOL_NO_ENTRY /DSOKOL_GLCORE

cl /c /EHsc /O2 %DEFINES% %SOURCES%
lib sokol_app.obj sokol_audio.obj sokol_time.obj /OUT:sokol.lib
del *obj
