@echo off
setlocal

set NAME= undeadwest
set SRC= src\_target.c
set MODE= %1%

set CFLAGS_R= /std:c17 /I extern\ 
set CFLAGS_D= /std:c17 /Od /Zi /W1 /I ..\extern\ 

set LDFLAGS_R= /link /INCREMENTAL:NO /LIBPATH:.\extern\sokol\lib sokol.lib
set LDFLAGS_D= /link /INCREMENTAL:NO /LIBPATH:..\extern\sokol\lib sokol.lib

shadertoh src\shaders\ src\render\shaders.h

if "%MODE%"==" d" (
  echo Building debug...
  mkdir debug
  copy SDL3.dll debug\ /b
  pushd debug
  cl %CFLAGS_D% /DDEBUG ..\%SRC% /Feundeadwest.exe %LDFLAGS_D% || exit /b 1
  del _target.obj
  del vc140.pdb
  popd
) else (
  if "%MODE%"==" r" (
    echo Building release...
    cl %CFLAGS_R% /O2 /DRELEASE %SRC% /Feundeadwest.exe %LDFLAGS_R% || exit /b 1
    del _target.obj
  ) else (
    echo Building...
    cl %CFLAGS_R% /Od %SRC% /Feundeadwest.exe %LDFLAGS_R% || exit /b 1
    del _target.obj
    %NAME%
  )
)
