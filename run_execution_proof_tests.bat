@echo off
REM =====================================
REM MinionLang Execution Proof Runner
REM Translates MinionLang -> C, compiles, runs, compares output
REM =====================================

setlocal EnableDelayedExpansion

set "TOOLS=%~dp0GnuWin32\bin"
set "GEN_DIR=tests\generated"

set PASS_COUNT=0
set FAIL_COUNT=0

if not exist "%GEN_DIR%" mkdir "%GEN_DIR%"

echo [1/2] Building translator...
set PATH=%TOOLS%;%PATH%
flex src\minionlang_to_c.l
if errorlevel 1 (
  echo ERROR: flex failed for translator
  exit /b 1
)

gcc -o minion2c.exe lex.yy.c
if errorlevel 1 (
  echo ERROR: gcc failed to build translator
  exit /b 1
)

echo [2/2] Running execution-proof test cases...
call :run_case exec1_if_else tests\exec1_if_else.minion tests\exec1_if_else.expected.txt
call :run_case exec2_loops tests\exec2_loops.minion tests\exec2_loops.expected.txt
call :run_case exec3_functions tests\exec3_functions.minion tests\exec3_functions.expected.txt

echo.
echo =====================================
echo Execution Proof Summary
echo =====================================
echo Passed: !PASS_COUNT!
echo Failed: !FAIL_COUNT!

if !FAIL_COUNT! GTR 0 (
  echo STATUS: FAIL
  exit /b 1
) else (
  echo STATUS: PASS
  exit /b 0
)

goto :eof

:run_case
set "CASE_NAME=%~1"
set "SRC_FILE=%~2"
set "EXPECT_FILE=%~3"
set "C_FILE=%GEN_DIR%\%CASE_NAME%.c"
set "EXE_FILE=%GEN_DIR%\%CASE_NAME%.exe"
set "OUT_FILE=%GEN_DIR%\%CASE_NAME%.out.txt"

echo.
echo [RUN] %CASE_NAME%

.\minion2c.exe "%SRC_FILE%" "%C_FILE%"
if errorlevel 1 (
  echo [FAIL] %CASE_NAME% - translation failed
  set /a FAIL_COUNT+=1
  goto :eof
)

gcc "%C_FILE%" -o "%EXE_FILE%"
if errorlevel 1 (
  echo [FAIL] %CASE_NAME% - C compilation failed
  set /a FAIL_COUNT+=1
  goto :eof
)

"%EXE_FILE%" > "%OUT_FILE%"
if errorlevel 1 (
  echo [FAIL] %CASE_NAME% - executable failed
  set /a FAIL_COUNT+=1
  goto :eof
)

fc "%EXPECT_FILE%" "%OUT_FILE%" >nul
if errorlevel 1 (
  echo [FAIL] %CASE_NAME% - output mismatch
  echo        Expected: %EXPECT_FILE%
  echo        Actual  : %OUT_FILE%
  set /a FAIL_COUNT+=1
  goto :eof
)

echo [OK] %CASE_NAME%
set /a PASS_COUNT+=1

goto :eof
