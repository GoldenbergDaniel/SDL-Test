@echo off
setlocal

set SRC=src\main.c
set OUT=undeadwest.exe

set MODE=dev
if "%1%"=="d"    set MODE=dbg
if "%1%"=="r"    set MODE=rls
if "%1%"=="push" set MODE=push

if "%MODE%"=="push" (
  echo - Pushing to Itch.io -
  pushd build || exit /b 1
    if not exist undead-west-windows mkdir undead-west-windows
    pushd undead-west-windows
      move ..\%OUT% .
      xcopy ..\..\res\ .\res\ /E /y
    popd
    butler push undead-west-windows goldenbergdev/undead-west:windows --userversion 0.7-dev || /b exit 1
  popd
  exit /b 0
)

if "%1%"=="d" (
  if "%2%"=="fsan" set FSAN= /fsanitize=address
)

set COMMON= /std:c17 /nologo /I..\extern\

if "%MODE%"=="dev" set CFLAGS= /Od /DDEV_MODE
if "%MODE%"=="dbg" set CFLAGS= /Od /Z7 /W1 /DDEBUG /DDEV_MODE
if "%MODE%"=="rls" set CFLAGS= /O2 /DRELEASE

set LFLAGS= /link /incremental:no /libpath:..\extern\sokol\lib sokol.lib

echo [mode:%MODE%]

if     "%2"=="fsan" echo [fsan:on]
if not "%2"=="fsan" echo [fsan:off]

echo - Processing -
shadertoh src\shaders\ src\render\shaders.h

set RUN=0
if "%MODE%"=="dev" set RUN=1

echo - Building -
if not exist build mkdir build 
pushd build
  cl %COMMON% %CFLAGS% %FSAN% ..\%SRC% /Fe%OUT% %LFLAGS% || exit /b 1
  del *.obj
  if "%RUN%"=="1" (
    echo %OUT%
    %OUT%
  )
popd
