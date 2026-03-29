# Short Project Report: MinionLang Compiler

## Project Title
MinionLang Compiler: A Flex-Bison Based Educational Compiler with Web Playground and IR Support

## 1. Introduction
This project implements a complete educational compiler workflow for a custom language named MinionLang. The language uses Minion-themed keywords while preserving core C-like programming constructs (types, expressions, conditions, loops, functions, and return statements).

The system is implemented using Flex for lexical analysis and Bison for syntax plus semantic analysis, with additional support for source translation and intermediate representation.

## 2. Main Objective
The main objective was to design and implement a practical compiler pipeline that demonstrates:
- Lexical analysis
- Syntax analysis
- Semantic checking
- Execution-oriented translation
- Intermediate code generation

## 3. Core Implementation Summary
- Lexer (`src/minionlang.l`) recognizes keywords, literals, operators, delimiters, comments, and invalid lexemes with line-aware diagnostics.
- Parser (`src/minionlang.y`) validates grammar and performs semantic checks such as redeclaration, undeclared usage, type mismatch, function argument mismatch, and return-type validation.
- Translator converts MinionLang programs to C (`minion2c.exe`), then GCC compiles and executes the generated code.
- IR module (`src/minionlang_ir.c`) produces simple 3-address-style IR and applies constant folding for constant-only arithmetic expressions.

## 4. Unique Features (Main Focus)

### 4.1 Dual Execution Paths (Very Unique for a Lab Project)
The project supports two practical execution experiences:
1. Browser Playground Path: MinionLang -> C transpilation display + in-browser runtime execution with diagnostics.
2. Native Execution-Proof Path: MinionLang -> C translation -> GCC compilation -> binary execution with expected-output comparison.

This dual-mode design makes the project both demo-friendly and technically rigorous.

### 4.2 Compiler + Web Integration
Unlike many compiler lab projects that stay CLI-only, this project includes a deployable web playground (`web/playground.html`) where users can:
- Write MinionLang code
- View tokens and diagnostics
- Run code instantly
- Inspect transpiled output

This significantly improves usability, presentation value, and accessibility.

### 4.3 Semantic Validation Beyond Basic Parsing
The parser is not only grammar-driven; it performs meaningful semantic checks using symbol/function tables and scoped declarations. This gives the project stronger correctness guarantees than syntax-only implementations.

### 4.4 Intermediate Representation + Optimization
The IR module includes:
- Infix-to-postfix conversion logic
- Three-address style emission
- Constant folding for arithmetic expressions

This introduces backend-like compiler thinking and optimization awareness.

### 4.5 Structured Automated Demonstration
The project includes scripted full-demo and execution-proof workflows that verify valid cases and expected-failure scenarios, improving reproducibility and confidence in behavior.

## 5. Results and Outcomes
- Lexer and parser pipeline works successfully on valid and invalid test programs.
- Semantic checks correctly identify multiple real error classes.
- Execution-proof tests validate runtime correctness through actual compiled binaries.
- IR generation and constant folding demonstrate advanced compiler concepts.
- Web playground provides fast interactive demonstrations for viva and presentations.

## 6. Conclusion
MinionLang Compiler is a complete and presentation-ready educational compiler project that goes beyond standard lexer-parser assignments. Its strongest contribution is the combination of formal compiler stages with practical, user-facing execution modes and advanced extensions (IR + optimization). This makes the project both academically strong and uniquely demonstrable.
