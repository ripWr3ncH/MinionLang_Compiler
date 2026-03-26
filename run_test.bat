@echo off
REM =====================================
REM Run MinionLang Lexer Test
REM =====================================

if not exist minionlexer.exe (
    echo ERROR: minionlexer.exe not found!
    echo Please run build.bat first
    pause
    exit /b 1
)

echo Running lexer on tests\sample.minion...
echo Output will be saved to tests\sample.tokens
echo.

minionlexer.exe tests\sample.minion tests\sample.tokens

echo.
echo Done! Check tests\sample.tokens for output.
pause
