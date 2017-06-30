@echo off

echo --- Building display ---
gcc  -O3 -c -o objs/display.o inc/display.c -Wextra
if %ERRORLEVEL% NEQ 0 goto Failed

echo --- Building dump ---
gcc -O3 -c -o objs/dump.o inc/dump.c  -Wextra
if %ERRORLEVEL% NEQ 0 goto Failed

gcc -O3 -o ../Builds/bytion main.c objs/display.o objs/dump.o -Wextra
IF %ERRORLEVEL% EQU 0 echo --- Build display successful ---
goto End


:Failed
echo --- Building failed ---
:End
