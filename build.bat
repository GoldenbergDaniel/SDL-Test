@echo off
setlocal

set SRC=src\main.c
set OUT=undeadwest.exe

set MODE= dev
if "%1%"=="d" set MODE= debug
if "%1%"=="r" set MODE= release
if "%1%"=="g" set MODE= git

if "%MODE%"==" git" (
  git add .
  git status
  git commit -m %2%
  git push
  exit /b 0
)

set PUSH= 0
if "%2%"=="push" set PUSH= 1

if "%MODE%"==" dev"     set CFLAGS= /std:c17 /Iextern\
if "%MODE%"==" debug"   set CFLAGS= /std:c17 /Od /Z7 /W1 /DDEBUG /I..\extern\
if "%MODE%"==" release" set CFLAGS= /std:c17 /O2 /DRELEASE /I..\extern\

if "%MODE%"==" dev"     set LFLAGS= /link /INCREMENTAL:NO /LIBPATH:extern\sokol\lib sokol.lib
if "%MODE%"==" debug"   set LFLAGS= /link /INCREMENTAL:NO /LIBPATH:..\extern\sokol\lib sokol.lib
if "%MODE%"==" release" set LFLAGS= /link /INCREMENTAL:NO /LIBPATH:..\extern\sokol\lib sokol.lib

shadertoh src\shaders\ src\render\shaders.h

if "%MODE%"==" dev" (
  echo Building...
  cl %CFLAGS% %SRC% /Fe%OUT% %LFLAGS% || exit /b 1
  del *.obj
  %OUT%
) else if "%MODE%"==" debug" (
  echo Building debug...
  if not exist debug mkdir debug
  pushd debug
    cl %CFLAGS% ..\%SRC% /Fe%OUT% %LFLAGS% || exit /b 1
    del *.obj
  popd
) else if "%MODE%"==" release" (
  echo Building release...
  if not exist release mkdir release
  pushd release
    cl %CFLAGS% ..\%SRC% /Fe%OUT% %LFLAGS% || exit /b 1
    del *.obj
    if not exist undead-west-windows mkdir undead-west-windows
    pushd undead-west-windows
      move ..\%OUT% .
      xcopy ..\..\res\ .\res\ /E /y
    popd
    if %PUSH% ==1 (
      echo Pushing release to itch...
      butler push undead-west-windows goldenbergdev/undead-west:windows --userversion 0.5-dev
    )
  popd
)
