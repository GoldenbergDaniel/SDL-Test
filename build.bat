@echo off
setlocal

set NAME = UndeadWest

set CFLAGS_R = /std:c17 ^
               /O0 ^
               /I ext\ ^
               /I ext\sdl2\inc ^

set CFLAFS_D = /std:c17 ^
               /Zi ^
               /I ..\ext\ ^
               /I ..\ext\sdl2\inc ^
               /W4 ^

set LDFLAGS_R = /LIBPATH:ext\sdl2\lib ^
                /link SDL2.dll ^
                ext\glad\glad.c ^

set LDFLAGS_R = /LIBPATH:..\ext\sdl2\lib ^
                /link SDL2.dll ^
                ..\ext\glad\glad.c ^

if "%1" == "r"
(
  echo Compiling project...
  cl %CFLAGS_R% src\_target.c %LDFLAGS_R% /Fo %NAME%
  echo Compiling project...
)
else if "%1" == "d"
(
  echo Compiling project...
  pushd dbg
  cl %CFLAGS_D% ..\src\_target.c %LDFLAGS_D% /Fo %NAME%
  popd
  echo Compilation complete!
)
else
(
  echo Failed to compile. Invalid argument.
)
