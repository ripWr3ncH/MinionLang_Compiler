@echo off
REM =====================================
REM Run MinionLang Semantic Tests
REM =====================================

if not exist minionparser.exe (
    echo ERROR: minionparser.exe not found!
    echo Please run build_parser.bat first
    pause
    exit /b 1
)

echo =====================================
echo MinionLang Semantic Test Suite
echo =====================================
echo.

echo [Test 1] Undeclared Variable
.\minionparser.exe tests\semantic_error1_undeclared.minion
echo.

echo [Test 2] Redeclaration
.\minionparser.exe tests\semantic_error2_redeclaration.minion
echo.

echo [Test 3] Type Mismatch
.\minionparser.exe tests\semantic_error3_type_mismatch.minion
echo.

echo [Test 4] Non-array Indexing
.\minionparser.exe tests\semantic_error4_non_array_index.minion
echo.

echo [Test 5] Out-of-scope Variable Usage
.\minionparser.exe tests\semantic_error5_scope_out.minion
echo.

echo [Test 6] Void Function Returns Value
.\minionparser.exe tests\semantic_error6_void_return_value.minion
echo.

echo [Test 7] Non-void Missing Return
.\minionparser.exe tests\semantic_error7_nonvoid_missing_return.minion
echo.

echo [Test 8] Function Argument Count Mismatch
.\minionparser.exe tests\semantic_error8_func_arg_count.minion
echo.

echo [Test 9] Function Argument Type Mismatch
.\minionparser.exe tests\semantic_error9_func_arg_type.minion
echo.

echo [Test 10] Undefined Function Call
.\minionparser.exe tests\semantic_error10_undefined_function.minion
echo.

echo [Test 11] Return Type Mismatch
.\minionparser.exe tests\semantic_error11_return_type_mismatch.minion
echo.

echo =====================================
echo Semantic tests completed!
echo =====================================
pause
