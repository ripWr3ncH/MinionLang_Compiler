@echo off
REM =====================================
REM MinionLang Full Demo Runner
REM =====================================

setlocal EnableDelayedExpansion

set PASS_COUNT=0
set FAIL_COUNT=0
set "TOOLS=%~dp0GnuWin32\bin"

call :step "Build parser" "set PATH=%TOOLS%;%PATH% && bison -d -v src\minionlang.y -o src\minionlang.tab.c && flex src\minionlang_parser.l && gcc -o minionparser.exe src\minionlang.tab.c src\lex.yy.c -I src" success

call :step "Build lexer" "set PATH=%TOOLS%;%PATH% && flex src\minionlang.l && gcc -o minionlexer.exe lex.yy.c" success

call :step "Build translator" "set PATH=%TOOLS%;%PATH% && flex src\minionlang_to_c.l && gcc -o minion2c.exe lex.yy.c" success

call :step "Build IR generator" "gcc -o minionir.exe src\minionlang_ir.c" success

call :step "Lexer sample test" ".\minionlexer.exe tests\sample.minion tests\sample.tokens" success

call :step "Parser valid syntax" ".\minionparser.exe tests\valid_syntax.minion" success

call :step "Parser syntax error 1 expected-fail" ".\minionparser.exe tests\syntax_error1.minion" fail

call :step "Parser syntax error 2 expected-fail" ".\minionparser.exe tests\syntax_error2.minion" fail

call :step "Parser syntax error 3 expected-fail" ".\minionparser.exe tests\syntax_error3.minion" fail

call :step "Semantic error 1 undeclared expected-fail" ".\minionparser.exe tests\semantic_error1_undeclared.minion" fail

call :step "Semantic error 2 redeclaration expected-fail" ".\minionparser.exe tests\semantic_error2_redeclaration.minion" fail

call :step "Semantic error 3 type mismatch expected-fail" ".\minionparser.exe tests\semantic_error3_type_mismatch.minion" fail

call :step "Semantic error 4 non-array indexing expected-fail" ".\minionparser.exe tests\semantic_error4_non_array_index.minion" fail

call :step "Semantic error 5 scope-out expected-fail" ".\minionparser.exe tests\semantic_error5_scope_out.minion" fail

call :step "Semantic error 6 void-return-value expected-fail" ".\minionparser.exe tests\semantic_error6_void_return_value.minion" fail

call :step "Semantic error 7 nonvoid-missing-return expected-fail" ".\minionparser.exe tests\semantic_error7_nonvoid_missing_return.minion" fail

call :step "Semantic error 8 func-arg-count expected-fail" ".\minionparser.exe tests\semantic_error8_func_arg_count.minion" fail

call :step "Semantic error 9 func-arg-type expected-fail" ".\minionparser.exe tests\semantic_error9_func_arg_type.minion" fail

call :step "Semantic error 10 undefined-function expected-fail" ".\minionparser.exe tests\semantic_error10_undefined_function.minion" fail

call :step "Semantic error 11 return-type-mismatch expected-fail" ".\minionparser.exe tests\semantic_error11_return_type_mismatch.minion" fail

call :step "Translator sample" ".\minion2c.exe tests\sample.minion tests\sample_translated.c" success

call :step "IR demo generation" ".\minionir.exe tests\ir_demo.minion tests\ir_demo.ir" success

echo.
echo =====================================
echo MinionLang Full Demo Summary
echo =====================================
echo Passed checks: !PASS_COUNT!
echo Failed checks: !FAIL_COUNT!
echo =====================================

if not exist tests\sample.tokens (
  echo ERROR: tests\sample.tokens was not generated
  set /a FAIL_COUNT+=1
)
if not exist tests\sample_translated.c (
  echo ERROR: tests\sample_translated.c was not generated
  set /a FAIL_COUNT+=1
)
if not exist tests\ir_demo.ir (
  echo ERROR: tests\ir_demo.ir was not generated
  set /a FAIL_COUNT+=1
)

if !FAIL_COUNT! GTR 0 (
  echo.
  echo FULL DEMO STATUS: FAIL
  exit /b 1
) else (
  echo.
  echo FULL DEMO STATUS: PASS
  exit /b 0
)

goto :eof

:step
set "STEP_NAME=%~1"
set "STEP_CMD=%~2"
set "STEP_EXPECT=%~3"

echo.
echo [RUN] %STEP_NAME%
cmd /c "%STEP_CMD%"
set STEP_EXIT=!ERRORLEVEL!

if /i "%STEP_EXPECT%"=="success" goto :expect_success
goto :expect_fail

:expect_success
if !STEP_EXIT! EQU 0 (
  echo [OK] %STEP_NAME%
  set /a PASS_COUNT+=1
) else (
  echo [FAIL] %STEP_NAME% - unexpected error
  set /a FAIL_COUNT+=1
)
goto :eof

:expect_fail
if !STEP_EXIT! NEQ 0 (
  echo [OK] %STEP_NAME% - failed as expected
  set /a PASS_COUNT+=1
) else (
  echo [FAIL] %STEP_NAME% - unexpected success
  set /a FAIL_COUNT+=1
)

goto :eof
