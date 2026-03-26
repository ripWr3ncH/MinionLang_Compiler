# MinionLang Testing and Debugging Guide

This guide provides exact build/run commands and expected outcomes for lexer, parser, semantic checks, and optional translation.

## 1) Lexer (Flex only)

### Build
```bash
flex src/minionlang.l
gcc lex.yy.c -o minionlexer.exe
```

### Run
```bash
./minionlexer.exe tests/sample.minion tests/sample.tokens
```

### Expected
- `tests/sample.tokens` contains token stream in format:
  - `<TOKEN_TYPE, lexeme, line_number>`
- Invalid token example (`=` instead of `:=`) prints:
  - `LEXICAL ERROR ... Use ':=' for assignment, not '='`

## 2) Parser (Flex + Bison)

### Build
```bash
bison -d src/minionlang.y -o src/minionlang.tab.c
flex src/minionlang_parser.l
gcc src/minionlang.tab.c src/lex.yy.c -I src -o minionparser.exe
```

### Run Valid Test
```bash
./minionparser.exe tests/valid_syntax.minion
```

### Expected
- Ends with:
  - `[OK] PARSING + BASIC SEMANTIC CHECKS PASSED`

### Run Syntax Error Tests
```bash
./minionparser.exe tests/syntax_error1.minion
./minionparser.exe tests/syntax_error2.minion
./minionparser.exe tests/syntax_error3.minion
```

### Expected
- syntax_error1/2/3 produce `SYNTAX ERROR` blocks.
- syntax_error3 also reports lexical assignment error for `=`.

## 3) Semantic Behavior Checks

The parser now performs basic semantic checks:
- Redeclaration detection
- Use-before-declaration detection
- Assignment type compatibility (basic)
- Uninitialized variable warning (basic)
- Array indexing checks

### Typical semantic error message
```text
SEMANTIC ERROR at line X: Variable used before declaration (name)
```

### Dedicated Semantic Test Files

- `tests/semantic_error1_undeclared.minion`
  - Expected: `Assignment to undeclared variable (y)`
- `tests/semantic_error2_redeclaration.minion`
  - Expected: `Variable redeclared (x)`
- `tests/semantic_error3_type_mismatch.minion`
  - Expected: `Type mismatch in declaration assignment (x)`
- `tests/semantic_error4_non_array_index.minion`
  - Expected: `Indexed access on non-array variable (x)`
- `tests/semantic_error5_scope_out.minion`
  - Expected: `Variable used before declaration (t)`
- `tests/semantic_error6_void_return_value.minion`
  - Expected: `Void function cannot return a value (showValue)`
- `tests/semantic_error7_nonvoid_missing_return.minion`
  - Expected: `Non-void function must return a value (addOne)`
- `tests/semantic_error8_func_arg_count.minion`
  - Expected: `Function argument count mismatch (addTwo)`
- `tests/semantic_error9_func_arg_type.minion`
  - Expected: `Function argument type mismatch (incOne)`
- `tests/semantic_error10_undefined_function.minion`
  - Expected: `Call to undefined function (mystery)`
- `tests/semantic_error11_return_type_mismatch.minion`
  - Expected: `Return type mismatch (giveText)`

### Run Semantic Tests
```bash
./minionparser.exe tests/semantic_error1_undeclared.minion
./minionparser.exe tests/semantic_error2_redeclaration.minion
./minionparser.exe tests/semantic_error3_type_mismatch.minion
./minionparser.exe tests/semantic_error4_non_array_index.minion
./minionparser.exe tests/semantic_error5_scope_out.minion
./minionparser.exe tests/semantic_error6_void_return_value.minion
./minionparser.exe tests/semantic_error7_nonvoid_missing_return.minion
./minionparser.exe tests/semantic_error8_func_arg_count.minion
./minionparser.exe tests/semantic_error9_func_arg_type.minion
./minionparser.exe tests/semantic_error10_undefined_function.minion
./minionparser.exe tests/semantic_error11_return_type_mismatch.minion
```

### Batch Run (Windows)
```bash
run_semantic_tests.bat
```

## 7) One-Command Full Evaluation

```bash
run_full_demo.bat
```

Expected summary:
- Passed checks: 22
- Failed checks: 0
- FULL DEMO STATUS: PASS

## 8) Execution Correctness Proof (Translator + Native Run)

This validates runtime behavior by translating MinionLang to C, compiling with GCC, running binaries, and comparing actual output with expected files.

### Run
```bash
run_execution_proof_tests.bat
```

### Test Set
- `tests/exec1_if_else.minion` -> `tests/exec1_if_else.expected.txt`
- `tests/exec2_loops.minion` -> `tests/exec2_loops.expected.txt`
- `tests/exec3_functions.minion` -> `tests/exec3_functions.expected.txt`

### Expected summary
- Passed: 3
- Failed: 0
- STATUS: PASS

## 9) Optional IR + Constant Folding Demo

### Build
```bash
gcc src/minionlang_ir.c -o minionir.exe
```

### Run
```bash
./minionir.exe tests/ir_demo.minion tests/ir_demo.ir
```

### Expected
- Generates `tests/ir_demo.ir`
- Includes three-address temporaries like `t1 = a + 10`
- Folds constants, e.g.:
  - `2 add 3 mul 4` -> `a = 14`
  - `(8 add 2) mul (3 add 1)` -> `d = 40`

## 4) Optional High-Marks Feature: MinionLang -> C Translation

### Build
```bash
flex src/minionlang_to_c.l
gcc lex.yy.c -o minion2c.exe
```

### Run
```bash
./minion2c.exe tests/sample.minion tests/sample_translated.c
```

### Expected
- `tests/sample_translated.c` generated with Minion keywords translated to C-like forms.
- Examples:
  - `banana` -> `int`
  - `kaba` -> `if`
  - `againa(x : y : z)` -> `for(x ; y ; z)`

## 5) Windows Batch Shortcuts

- Lexer build: `build.bat`
- Parser build: `build_parser.bat`
- Translator build: `build_translator.bat`

## 6) Debugging Checklist

1. If `flex` or `bison` not found:
   - Ensure `GnuWin32/bin` is in PATH.
2. If parser build fails:
   - Rebuild in order: bison -> flex -> gcc.
3. If many syntax errors appear from one typo:
   - Fix the first reported syntax error first.
4. If semantic errors appear on valid code:
   - Check declaration order and assignment operators (`:=`).
