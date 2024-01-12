@echo off
setlocal

set NAME= UndeadWest

set CFLAGS_R= /std:c17 /O2 /I ext\ /I ext\sdl2\inc
set CFLAGS_D= /std:c17 /Od /Zi /W4 /I ..ext\ /I ..\ext\sdl2\inc
set LDFLAGS_R= /link /LIBPATH:ext\sdl2\lib SDL2.lib
set LDFLAGS_D= /link /LIBPATH:..ext\sdl2\lib SDL2.lib

echo Compiling project...
cl %CFLAGS_R% src\_target.c /FeUndeadWest.exe /link /LIBPATH:ext\sdl2\lib SDL2.lib
del _target.obj
echo Compilation complete.
