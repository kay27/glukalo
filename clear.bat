rem got from https://habrahabr.ru/post/210584/ with thanks
del src\com\example\testapp\R.java
del *.keystore
del /s /f /q bin\*.*
rmdir /s /q obj
md obj