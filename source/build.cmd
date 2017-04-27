@echo off
gcc -Wextra -o main main.c
IF %ERRORLEVEL% EQU 0 echo ---- Build successful ----
IF %ERRORLEVEL% EQU 9009 echo ---- gcc does not exist ----
