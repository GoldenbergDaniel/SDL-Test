@echo off
setlocal

set SRC=main.c
set OUT=undeadwest.exe

@REM --- CONFIGURE ------------------------------------------------------------------

set MODE=dev
if "%1%"=="debug"   set MODE=debug
if "%1%"=="release" set MODE=release
if "%1%"=="push"    set MODE=push

if "%MODE%"=="push" (
  echo [pushing to Itch.io]
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

set TARGET=windows_amd64
if not "%2%"=="" (
  set TARGET=%2%
)

set COMMON= /std:c17 /nologo /Iext\

if "%MODE%"=="dev"     set CFLAGS= /Od /DDEV_MODE
if "%MODE%"=="debug"   set CFLAGS= /Od /Z7 /W1 /DDEBUG /DDEV_MODE
if "%MODE%"=="release" set CFLAGS= /O2 /DRELEASE

set LFLAGS= /link /incremental:no /libpath:ext\sokol\lib sokol.lib

echo [target:%TARGET%]
echo [mode:%MODE%]

@REM --- PREPROCESS -----------------------------------------------------------------

@REM echo [process]
@REM shadertoh src\shaders\ src\render\shaders.h

@REM --- BUILD ----------------------------------------------------------------------

echo [build]
if not exist out mkdir out 
cl %COMMON% %CFLAGS% %FSAN% src\%SRC% /Feout\%OUT% %LFLAGS% || exit /b 1
del *.obj
if "%MODE%"=="dev" out\%OUT%
