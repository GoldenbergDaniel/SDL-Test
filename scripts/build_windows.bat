@echo off
setlocal

set SRC=src\_target.c
set OUT=undeadwest.exe

set MODE= dev
if "%1%"=="d" set MODE= debug
if "%1%"=="r" set MODE= release

if "%MODE%"==" dev"     set CFLAGS= /std:c17 /I extern\
if "%MODE%"==" debug"   set CFLAGS= /std:c17 /Od /DRELEASE /I ..\extern\
if "%MODE%"==" release" set CFLAGS= /std:c17 /O2 /DDEBUG /I ..\extern\

if "%MODE%"==" dev"     set LDFLAGS= /link /INCREMENTAL:NO /LIBPATH:extern\sokol\lib sokol.lib
if "%MODE%"==" debug"   set LDFLAGS= /link /INCREMENTAL:NO /LIBPATH:..\extern\sokol\lib sokol.lib
if "%MODE%"==" release" set LDFLAGS= /link /INCREMENTAL:NO /LIBPATH:..\extern\sokol\lib sokol.lib

shadertoh src\shaders\ src\render\shaders.h

if "%MODE%"==" dev" (
  echo Building...
  cl %CFLAGS% %SRC% /Fe%OUT% %LDFLAGS% || exit /b 1
  del _target.obj
  %OUT%
)

if "%MODE%"==" debug" (
  echo Building debug...
  if not exist debug mkdir debug
  pushd debug
    cl %CFLAGS% ..\%SRC% /Fe%OUT% %LDFLAGS% || exit /b 1
    del _target.obj
  popd
)

if "%MODE%"==" release" (
  echo Building release...
  if not exist release mkdir release
  pushd release
    cl %CFLAGS% ..\%SRC% /Fe%OUT% %LDFLAGS% || exit /b 1
    del _target.obj
    if not exist undead-west-windows mkdir undead-west-windows
    pushd undead-west-windows
      move ..\%OUT% .
      xcopy ..\..\res\ .\res\ /E /y
    popd
    butler push undead-west-windows goldenbergdev/undead-west:windows --userversion-file ..\version.txt
  popd
)
