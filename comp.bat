@set ant=G:\Android\apache-ant-1.9.6\bin\ant.bat
@set target=debug
@set platform=android-23
@echo off

call clear
call android update project --name glukalo --path . --target "%platform%"
if errorlevel 1 exit /b %errorlevel%
call ndk-build
if errorlevel 1 exit /b %errorlevel%
call %ant% %target%
if errorlevel 1 exit /b %errorlevel%
