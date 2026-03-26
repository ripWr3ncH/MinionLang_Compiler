@echo off
REM =====================================
REM Build MinionLang IR Generator
REM =====================================

gcc -o minionir.exe src\minionlang_ir.c
if %errorlevel% neq 0 (
    echo.
    echo Build failed!
    pause
    exit /b 1
)

echo.
echo Build successful!
echo Executable created: minionir.exe
pause
