@echo off

echo --- Building display ---
gcc -c -O3 -Wextra -o objs/display.o inc/display.c
if %ERRORLEVEL% NEQ 0 goto Failed

echo --- Building dump ---
gcc -c -O3 -Wextra -o objs/dump.o inc/dump.c
if %ERRORLEVEL% NEQ 0 goto Failed

gcc -O3 -Wextra -o bytion main.c objs/display.o objs/dump.o
IF %ERRORLEVEL% EQU 0 echo --- Build display successful ---
goto End


:Failed
echo --- Building failed ---
:End