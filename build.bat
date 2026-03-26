@echo off
REM =====================================
REM MinionLang Lexer Build Script
REM =====================================

echo =====================================
echo MinionLang Lexer Build Script
echo =====================================
echo.

REM Use local GnuWin32 flex installation
set FLEX_PATH=%~dp0GnuWin32\bin\flex.exe

REM Check if flex is available
if not exist "%FLEX_PATH%" (
    echo ERROR: flex not found at %FLEX_PATH%
    echo Please ensure GnuWin32 is properly installed
    pause
    exit /b 1
)

REM Check if g++ is available
where g++ >nul 2>nul
if %ERRORLEVEL% NEQ 0 (
    echo ERROR: g++ is not installed or not in PATH
    echo Please install MinGW-w64 or similar
    pause
    exit /b 1
)

echo [1/3] Generating lexer from minionlang.l...
"%FLEX_PATH%" src\minionlang.l
if %ERRORLEVEL% NEQ 0 (
    echo ERROR: flex failed to generate lexer
    pause
    exit /b 1
)
echo       Done: lex.yy.c generated

echo.
echo [2/3] Compiling lexer...
gcc -o minionlexer.exe lex.yy.c
if %ERRORLEVEL% NEQ 0 (
    echo ERROR: Compilation failed
    pause
    exit /b 1
)
echo       Done: minionlexer.exe created

echo.
echo [3/3] Build complete!
echo.
echo =====================================
echo Usage:
echo   minionlexer.exe ^<input.minion^> [output.tokens]
echo.
echo Example:
echo   minionlexer.exe tests\sample.minion output.tokens
echo =====================================
echo.
pause
