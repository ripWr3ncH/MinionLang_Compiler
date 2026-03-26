@echo off
REM =====================================
REM MinionLang Parser Build Script
REM Builds both Lexer and Parser (Flex + Bison)
REM =====================================

echo =====================================
echo MinionLang Parser Build Script
echo Flex + Bison Integration
echo =====================================
echo.

REM Set paths
set FLEX_PATH=%~dp0GnuWin32\bin\flex.exe
set BISON_PATH=%~dp0GnuWin32\bin\bison.exe
set PATH=%~dp0GnuWin32\bin;%PATH%

REM Check if flex is available
if not exist "%FLEX_PATH%" (
    echo ERROR: flex not found at %FLEX_PATH%
    echo Please ensure GnuWin32 is properly installed
    pause
    exit /b 1
)

REM Check if bison is available
if not exist "%BISON_PATH%" (
    echo WARNING: bison not found at %BISON_PATH%
    echo Attempting to use system bison...
    set BISON_PATH=bison
)

REM Check if gcc is available
where gcc >nul 2>nul
if %ERRORLEVEL% NEQ 0 (
    echo ERROR: gcc is not installed or not in PATH
    pause
    exit /b 1
)

echo [1/4] Generating parser from minionlang.y...
cd src
"%BISON_PATH%" -d -v minionlang.y -o minionlang.tab.c
if %ERRORLEVEL% NEQ 0 (
    echo ERROR: bison failed to generate parser
    cd ..
    pause
    exit /b 1
)
echo       Done: minionlang.tab.c and minionlang.tab.h generated
cd ..

echo.
echo [2/4] Generating lexer from minionlang_parser.l...
cd src
"%FLEX_PATH%" minionlang_parser.l
if %ERRORLEVEL% NEQ 0 (
    echo ERROR: flex failed to generate lexer
    cd ..
    pause
    exit /b 1
)
echo       Done: lex.yy.c generated
cd ..

echo.
echo [3/4] Compiling parser...
gcc -o minionparser.exe src\minionlang.tab.c src\lex.yy.c -I src
if %ERRORLEVEL% NEQ 0 (
    echo ERROR: Compilation failed
    pause
    exit /b 1
)
echo       Done: minionparser.exe created

echo.
echo [4/4] Build complete!
echo.
echo =====================================
echo Usage:
echo   minionparser.exe ^<input.minion^>
echo.
echo Example:
echo   minionparser.exe tests\sample.minion
echo   minionparser.exe tests\syntax_error.minion
echo =====================================
echo.
pause
