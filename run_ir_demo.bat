@echo off
REM =====================================
REM Run MinionLang IR Demo
REM =====================================

if not exist minionir.exe (
    echo minionir.exe not found. Building first...
    call build_ir.bat
)

if not exist minionir.exe (
    echo ERROR: could not build minionir.exe
    pause
    exit /b 1
)

.\minionir.exe tests\ir_demo.minion tests\ir_demo.ir
if %errorlevel% neq 0 (
    echo ERROR: IR generation failed
    pause
    exit /b 1
)

echo.
echo IR output written to tests\ir_demo.ir
echo.
type tests\ir_demo.ir
pause
