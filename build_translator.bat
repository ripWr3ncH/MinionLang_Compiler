@echo off
REM =====================================
REM MinionLang Translator Build Script
REM =====================================

echo =====================================
echo MinionLang to C Translator Build
echo =====================================
echo.

set FLEX_PATH=%~dp0GnuWin32\bin\flex.exe
set PATH=%~dp0GnuWin32\bin;%PATH%

if not exist "%FLEX_PATH%" (
    echo ERROR: flex not found at %FLEX_PATH%
    pause
    exit /b 1
)

where gcc >nul 2>nul
if %ERRORLEVEL% NEQ 0 (
    echo ERROR: gcc is not installed or not in PATH
    pause
    exit /b 1
)

echo [1/2] Generating translator lexer...
"%FLEX_PATH%" src\minionlang_to_c.l
if %ERRORLEVEL% NEQ 0 (
    echo ERROR: flex generation failed
    pause
    exit /b 1
)

echo [2/2] Compiling translator...
gcc -o minion2c.exe lex.yy.c
if %ERRORLEVEL% NEQ 0 (
    echo ERROR: Compilation failed
    pause
    exit /b 1
)

echo.
echo Build complete.
echo Usage:
echo   minion2c.exe ^<input.minion^> [output.c]
echo Example:
echo   minion2c.exe tests\sample.minion tests\sample_translated.c
echo.
pause
