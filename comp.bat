@set ndkbuild=D:\Android\android-ndk-r10e\ndk-build.cmd
@set android=D:\Android\sdk\tools\android.bat
@set ant=D:\Android\apache-ant-1.9.6\bin\ant.bat
@set target=debug
@set platform=android-23
@echo off

set step=%1 && if "%step%" equ "" set step=0

if %step% leq 0 (
  set ccmd=clear
  call:callandlog "Step 0: Clear" clear.log
)
if %step% leq 1 (
  set ccmd=%android% update project --name glukalo --path . --target "%platform%"
  call:callandlog "Step 1: Android project update" update.log
)
if %step% leq 2 (
  set ccmd=%ndkbuild%
  call:callandlog "Step 2: NDK Build" ndkbuild.log
)
if %step% leq 3 (
  set ccmd=%ant% %target%
  call:callandlog "Step 3: Ant %target%" ant.log
)
goto:eof

:callandlog
  echo %~1
  call %ccmd%>%~2
  if errorlevel 1 (
    type %~2
    exit /b %errorlevel%
  )
  echo Done. See %~2 for details
  echo.
goto:eof

