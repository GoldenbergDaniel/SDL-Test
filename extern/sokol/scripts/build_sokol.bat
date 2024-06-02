@echo off
setlocal

set SOURCES= extern\sokol\sokol_app.c extern\sokol\sokol_audio.c extern\sokol\sokol_time.c extern\sokol\sokol_log.c
set DEFINES= /DSOKOL_IMPL /DSOKOL_NO_ENTRY /DSOKOL_GLCORE

cl /c /EHsc %DEFINES% %SOURCES%
lib sokol_app.obj sokol_audio.obj sokol_time.obj sokol_log.obj /OUT:sokol.lib
