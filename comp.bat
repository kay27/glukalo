@set name=glukalo
@set ndkbuild=d:\Android\android-ndk-r10e\ndk-build.cmd
@set android=d:\Android\sdk\tools\android.bat
@set ant=d:\Android\apache-ant-1.9.6\bin\ant.bat
@set adb=d:\Android\sdk\platform-tools\adb.exe
@set target=debug
@set platform=android-23
@echo off

set step=%1
if "%step%" equ "" set step=0
if "%step:~-1%"==" " set step=%step:~0,-1%
set only=%2

if %step% leq 0 (
  set ccmd=clear
  call:callandlog "Step 0: Clear" nul
  if errorlevel 1 exit /b %errorlevel%
  if "%only%" equ "only" exit /b 0
)
if %step% leq 1 (
  set ccmd=%android% update project --name %name% --path . --target "%platform%"
  call:callandlog "Step 1: Android project update" update.log
  if errorlevel 1 exit /b %errorlevel%
  if "%only%" equ "only" exit /b 0
)
if %step% leq 2 (
  set ccmd=%ndkbuild%
  call:callandlog "Step 2: NDK Build" ndkbuild.log
  if errorlevel 1 exit /b %errorlevel%
  if "%only%" equ "only" exit /b 0
)
if %step% leq 3 (
  set ccmd=%ant% %target%
  call:callandlog "Step 3: Ant %target%" ant.log
  if errorlevel 1 exit /b %errorlevel%
  if "%only%" equ "only" exit /b 0
)
if %step% leq 4 (
  set ccmd=%adb% install bin\glukalo-%target%.apk
  call:callandlog "Step 4: ADB" adb.log
  if errorlevel 1 exit /b %errorlevel%
  if "%only%" equ "only" exit /b 0
)
exit /b 0

:callandlog
  echo %~1
  call %ccmd%>%~2
  if errorlevel 1 (
    type %~2
    exit /b %errorlevel%
  )
  echo Done. See %~2 for details
  exit /b 0
