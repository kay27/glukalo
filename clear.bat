del build.xml
del local.properties
del proguard-project.txt
del project.properties
del ant.log
del ndkbuild.log
del update.log
del adb.log

rmdir /s /q bin
rmdir /s /q obj
rmdir /s /q libs
rmdir /s /q gen

exit /b 0
