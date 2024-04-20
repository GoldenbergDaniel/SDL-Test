@REM call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"

@echo off
setlocal

set NAME= UndeadWest

set CFLAGS_R= /std:c17 /O2 /I extern\ /I extern\sdl2\inc
set CFLAGS_D= /std:c17 /Od /Zi /W4 /I ..extern\ /I ..\extern\sdl2\inc
set LDFLAGS_R= /link /LIBPATH:extern\sdl2\lib SDL2.lib
set LDFLAGS_D= /link /LIBPATH:..extern\sdl2\lib SDL2.lib

echo Compiling project...
@REM cl %CFLAGS_R% src\_target.c /FeUndeadWest.exe /link /LIBPATH:extern\sdl2\lib SDL2.lib
cl /std:c17 /Od /I extern\ /I extern\sdl2\inc src\_target.c /Zi /FeUndeadWest.exe /link /LIBPATH:extern\sdl2\lib SDL2.lib
del _target.obj
echo Compilation complete.
