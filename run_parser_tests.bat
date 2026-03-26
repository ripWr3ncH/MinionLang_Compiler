@echo off
REM =====================================
REM Run MinionLang Parser Tests
REM =====================================

if not exist minionparser.exe (
    echo ERROR: minionparser.exe not found!
    echo Please run build_parser.bat first
    pause
    exit /b 1
)

echo =====================================
echo MinionLang Parser Test Suite
echo =====================================
echo.

echo [Test 1] Valid Syntax Test
echo --------------------------
.\minionparser.exe tests\valid_syntax.minion
echo.
echo Press any key for next test...
pause >nul
echo.

echo [Test 2] Syntax Error Test 1 (Missing semicolon)
echo -------------------------------------------------
.\minionparser.exe tests\syntax_error1.minion
echo.
echo Press any key for next test...
pause >nul
echo.

echo [Test 3] Syntax Error Test 2 (Missing braces)
echo ----------------------------------------------
.\minionparser.exe tests\syntax_error2.minion
echo.
echo Press any key for next test...
pause >nul
echo.

echo [Test 4] Syntax Error Test 3 (Invalid expressions)
echo ---------------------------------------------------
.\minionparser.exe tests\syntax_error3.minion
echo.

echo =====================================
echo All tests completed!
echo =====================================
pause
