@echo off
setlocal

set NAME= UndeadWest
set CC= cl
set MODE= %1%

set CFLAGS_R= /std:c17 /Od /I extern\ /I extern\sdl3\inc
set LDFLAGS_R= /link /LIBPATH:extern\sdl3\lib SDL3.lib

set CFLAGS_D= /std:c17 /Od /Zi /W4 /I ..\extern\ /I ..\extern\sdl3\inc
set LDFLAGS_D= /link /LIBPATH:..\extern\sdl3\lib SDL3.lib

if "%MODE%"==" d" (
  echo Building debug...
  mkdir debug
  copy SDL3.dll debug\ /b
  pushd debug
  %CC% %CFLAGS_D% ..\src\_target.c /FeUndeadWest.exe %LDFLAGS_D%
  del _target.obj
  del UndeadWest.ilk
  del vc140.pdb
  popd
) else (
  echo Building...
  %CC% %CFLAGS_R% src\_target.c /FeUndeadWest.exe %LDFLAGS_R%
  del _target.obj
)

echo Done!
