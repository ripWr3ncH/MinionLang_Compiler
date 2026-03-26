# MinionLang Complete Viva Preparation Guide

This guide is designed so you can explain your project from zero to advanced depth and answer common teacher questions confidently.

## 1) 30-Second Project Pitch

MinionLang is a C-inspired educational language with Minion-themed keywords.
This project implements:
- Lexical analysis using Flex
- Syntax analysis using Bison
- Basic semantic analysis (symbol table, scope, type checks, function checks)
- Optional translation to C for execution proof
- Optional IR generation with tiny optimization (constant folding)

The full automated demo passes all checks through a single command.

## 2) Problem Statement and Goal

Goal:
Build a compiler lab project that demonstrates core compilation phases clearly and practically.

Why this project is valid for compiler lab:
- Shows tokenization using regular-expression rules
- Shows context-free grammar parsing
- Shows semantic checks beyond syntax
- Shows executable evidence through translation and runtime tests
- Shows intermediate representation and optimization basics

## 3) Language Design Summary

MinionLang keeps C-like semantics, but keyword surface is themed.

Examples:
- banana -> int
- kaba -> if
- bababa -> else
- papoy -> printf
- takey -> scanf
- backa -> return
- againa -> for
- loopa -> while
- := -> assignment

Important point to explain:
The language is not random; it is systematically mapped to familiar C concepts so grammar and semantics remain teachable.

## 4) Project Architecture

Main source files:
- src/minionlang.l: standalone lexer
- src/minionlang_parser.l: parser-integrated lexer (returns Bison tokens)
- src/minionlang.y: grammar + semantic actions
- src/minionlang_to_c.l: MinionLang to C translator
- src/minionlang_ir.c: simple TAC generator with constant folding

Automation and testing:
- build.bat, build_parser.bat, build_translator.bat, build_ir.bat
- run_semantic_tests.bat
- run_execution_proof_tests.bat
- run_full_demo.bat

## 5) Compilation Pipeline (How to Explain)

Source code
-> Lexer (Flex)
-> Token stream
-> Parser (Bison)
-> Semantic checks
-> Optional C translation and execution proof
-> Optional IR generation and tiny optimization

## 6) Lexical Analysis (What to Say)

Core points:
- Flex patterns match keywords, operators, identifiers, literals, comments
- Longest-match and rule-order behavior are essential
- Line numbers tracked for diagnostics
- Lexical error example: using = instead of := gives a targeted message

Typical viva answer:
Lexer validates character-level patterns and produces tokens; parser then validates structural correctness.

## 7) Syntax Analysis (What to Say)

Core points:
- Grammar implemented in Bison with precedence and associativity
- Handles declarations, expressions, conditionals, loops, functions, arrays, switch-case
- Error recovery exists, but tuned to avoid noisy cascades

Strong practical point:
The parser now reports first meaningful syntax failure more cleanly for easier debugging.

## 8) Semantic Analysis (What to Say)

Implemented semantic checks include:
- Redeclaration detection
- Use-before-declaration detection
- Type mismatch checks in declaration and assignment
- Array misuse checks (indexing non-array)
- Scope-aware symbol behavior for block boundaries
- Function return consistency
- Function call argument count and type checks
- Undefined function call detection

This is important:
Syntax can be valid while semantics are wrong. Your project demonstrates that distinction clearly.

## 9) Optional Feature 1: Translation to C

Purpose:
Show practical execution correctness, not just parser acceptance.

Flow:
MinionLang source -> translated C -> gcc compile -> run binary -> compare output with expected.

Why teachers like this:
It proves behavior-level correctness, not only grammar recognition.

## 10) Optional Feature 2: IR + Constant Folding

Implemented in src/minionlang_ir.c.

What it does:
- Extracts assignment expressions
- Emits simple three-address code with temporary variables
- Folds constant-only subexpressions

Example idea:
2 add 3 mul 4 becomes folded result 14.

How to defend this feature:
- It is intentionally small but conceptually accurate
- Demonstrates optimization at intermediate stage
- Good extension beyond minimum lab requirement

## 11) Test Strategy (How to Explain)

You have positive and negative tests.

Positive:
- valid_syntax.minion
- execution proof programs

Negative syntax:
- syntax_error1, syntax_error2, syntax_error3

Negative semantic:
- undeclared use
- redeclaration
- type mismatch
- non-array indexing
- scope violation
- void return value
- non-void missing return
- function arg count mismatch
- function arg type mismatch
- undefined function call
- return type mismatch

Why this is strong:
Coverage includes both structural and meaning-level failures.

## 12) Exact Demo Sequence for Viva

Recommended order in front of teacher:
1. build_parser.bat
2. minionparser.exe tests/valid_syntax.minion
3. minionparser.exe tests/syntax_error1.minion
4. minionparser.exe tests/semantic_error10_undefined_function.minion
5. run_semantic_tests.bat
6. run_execution_proof_tests.bat
7. build_ir.bat
8. minionir.exe tests/ir_demo.minion tests/ir_demo.ir
9. run_full_demo.bat

What to highlight at the end:
- Full demo pass count and zero failures

## 13) High-Probability Teacher Questions and Ready Answers

Q1. Why two lexer files?
A. One lexer is standalone for token-only tasks, and one is parser-integrated to return tokens and semantic values to Bison.

Q2. Why not only syntax checking?
A. Real compilers require semantic validity; syntax-only acceptance misses meaningful errors like undeclared variables or wrong function arguments.

Q3. How did you handle scope?
A. A scope marker stack tracks symbol table boundaries. Entering block pushes marker, leaving block pops symbols to previous count.

Q4. How do you prevent noisy error output?
A. Parser error behavior was tuned to prioritize first failure clarity and suppress misleading semantic noise after syntax errors.

Q5. How do function checks work?
A. Function signatures are registered and validated at call sites for argument count and type compatibility.

Q6. What is constant folding here?
A. Compile-time evaluation of constant-only expressions in IR generation, reducing runtime work.

Q7. Is this a full production compiler?
A. No. It is an academic compiler-lab implementation focused on core phases and demonstrable correctness.

Q8. Why use Flex and Bison?
A. They are standard tools for lexical and syntax phases, reduce manual parser complexity, and make grammar maintenance clearer.

Q9. Can semantic checks produce false positives?
A. The project is tuned for the defined grammar and tests; behavior is validated by dedicated negative and positive test suites.

Q10. What are current limitations?
A. No full AST pipeline, no code generation backend, and optimization is intentionally minimal.

## 14) Honest Limitations (Say This If Asked)

- IR module is intentionally small and assignment-focused
- Optimization only includes basic constant folding
- Translation module is rule-based, not full semantic-preserving backend
- Designed for lab demonstration quality, not industrial compiler completeness

This answer is good because it is truthful and still shows mature engineering scope control.

## 15) What Makes This Submission Strong

- End-to-end tooling from lexical phase to semantic checks
- Automated scripts for reproducible results
- Negative testing across syntax and semantics
- Execution proof via translated C runtime validation
- Optional IR and optimization extension
- Clear docs and viva workflow

## 16) One-Page Memory Sheet (Night Before Viva)

Remember these keywords:
- Lexer: regex to tokens
- Parser: CFG validation
- Semantic: meaning validation
- Scope: marker stack
- Function checks: signature table
- Execution proof: translate and run
- IR: TAC with temp variables
- Optimization: constant folding

Remember these proof lines:
- Full demo passes with zero failures
- Syntax and semantic negatives are intentionally expected-fail
- Optional enhancements are implemented and runnable

## 17) If Teacher Asks For Live Deep Dive

Open and explain in this order:
1. src/minionlang_parser.l
2. src/minionlang.y
3. run_semantic_tests.bat
4. run_execution_proof_tests.bat
5. src/minionlang_ir.c

This order shows progression from token to meaning to optimization.

## 18) Final Confidence Script (Speak Naturally)

This project demonstrates the full educational compiler flow. I implemented lexical, syntax, and semantic phases with automated tests. I also proved runtime behavior through translation to C and added optional IR with constant folding. The full demo passes reproducibly, and I can show both valid runs and controlled failure cases for debugging clarity.
