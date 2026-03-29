# MinionLang End-to-End Understanding Guide

This file is a practical, beginner-friendly guide to understand your compiler from start to finish, especially the lexer (.l) and parser (.y) parts.

## 1. Project Goal

Your project builds a mini compiler pipeline for MinionLang:
- Lexer (Flex): converts characters into tokens
- Parser (Bison): validates grammar and performs basic semantic checks
- Optional later stages: translator and IR demo

For lab and viva, the most important core is:
- `src/minionlang.l`
- `src/minionlang_parser.l`
- `src/minionlang.y`

## 2. End-to-End Flow (Lexer -> Parser)

1. Input file (example: `tests/valid_syntax.minion`) is read.
2. Lexer matches patterns and produces tokens.
3. Parser consumes those tokens according to grammar rules.
4. Parser prints parse actions and syntax/semantic diagnostics.
5. Program exits with success/failure code.

## 3. Main Part of `.l` File (`src/minionlang.l`)

Your `.l` file has 3 standard sections:

1. Declarations section (`%{ ... %}`)
- Includes headers and helper functions
- Tracks line numbers (`yylineno`)
- Prints tokens and lexical errors

2. Rules section (between first and second `%%`)
- Regex patterns and actions
- Pattern order matters a lot
- Examples in your file:
  - comments (`$$ ...`, `$* ... *$`)
  - directives (`#bringy`, `#setty`)
  - keywords (`banana`, `kaba`, `backa`)
  - operators (`:=`, `==`, `+`, `-`, etc.)
  - literals (int, float, string, char)
  - identifiers
  - fallback lexical error (`.`)

3. User code section (after second `%%`)
- `main()` opens input file
- optional output file if 2nd argument is provided
- runs `yylex()`
- prints summary

### Why pattern order matters

Flex uses longest-match and then rule order for ties. That is why your lexer keeps:
- multi-word keywords before single-word keywords
- multi-character operators before single-character ones
- float literal before integer literal
- identifier rule near the end

## 4. Main Part of `.y` File (`src/minionlang.y`)

Your `.y` file also has 3 major areas:

1. C declarations and helpers (`%{ ... %}`)
- symbol table (`symbols[]`)
- scope stack (`enter_scope()`, `exit_scope()`)
- function table (`functions[]`)
- semantic helper checks (type compatibility, call validation)
- error counters (`syntax_errors`, `semantic_errors`)

2. Bison declarations
- `%union` for semantic values
- `%token` declarations for all token types
- `%type` mappings for non-terminals
- precedence/associativity rules (`%left`, `%right`, `%nonassoc`)

3. Grammar rules (`%% ... %%`)
- high-level start symbol: `program`
- declarations and statements
- control flow (`if`, `else if`, `switch`, loops)
- function definitions and calls
- expressions with precedence
- semantic checks inside actions (`{ ... }`)

After grammar, it has:
- `yyerror()` for formatted syntax error output
- `main()` that opens input file, calls `yyparse()`, and prints final pass/fail status

## 5. Lexer and Parser Integration

- `src/minionlang_parser.l` returns tokens expected by `src/minionlang.y`
- Bison generates parser files:
  - `src/minionlang.tab.c`
  - `src/minionlang.tab.h`
- Flex scanner includes/uses token definitions from generated header in parser mode

## 6. How to Build and Run (Recommended)

Use `mingw32-make` on Windows.

If you are in project root (`MinionLang_Compiler`):

```bash
mingw32-make all
```

If you are in parent folder (`compiler`):

```bash
mingw32-make -C .\MinionLang_Compiler all
```

## 7. Run Only Lexer

Build lexer:

```bash
mingw32-make lexer
```

Run lexer (output in terminal):

```bash
.\minionlexer.exe tests\sample.minion
```

Run lexer (output in file):

```bash
.\minionlexer.exe tests\sample.minion tests\sample.tokens
```

See saved file:

```bash
type tests\sample.tokens
```

## 8. Run Only Parser

Build parser:

```bash
mingw32-make parser
```

Run parser (output in terminal):

```bash
.\minionparser.exe tests\valid_syntax.minion
```

Run parser and save all output (stdout + stderr) to file:

```bash
.\minionparser.exe tests\valid_syntax.minion > tests\valid_syntax.parse.log 2>&1
```

See saved parser log:

```bash
type tests\valid_syntax.parse.log
```

## 9. Quick Test Targets

From repo root:

```bash
mingw32-make syntax-tests
mingw32-make semantic-tests
mingw32-make full-demo
```

From parent folder:

```bash
mingw32-make -C .\MinionLang_Compiler syntax-tests
mingw32-make -C .\MinionLang_Compiler semantic-tests
mingw32-make -C .\MinionLang_Compiler full-demo
```

## 10. Output Behavior Summary

Lexer executable (`minionlexer.exe`):
- With one argument: writes token stream to terminal
- With two arguments: writes token stream to specified output file

Parser executable (`minionparser.exe`):
- Writes parse logs and final status to terminal
- To save parser logs, use shell redirection (`> file 2>&1`)

## 11. Most Important Things to Explain in Viva

1. Why lexer rule order is important
2. How tokens from `.l` match `%token` in `.y`
3. How precedence in `.y` resolves expression ambiguity
4. Where semantic checks are implemented in parser actions
5. Difference between syntax errors and semantic errors
6. How to run and capture outputs in terminal/file

## 12. One-Minute Explanation Script

"My project implements a MinionLang compiler front-end using Flex and Bison. The lexer in `minionlang.l` recognizes keywords, identifiers, literals, operators, and comments, then emits tokens with line numbers. The parser in `minionlang.y` consumes those tokens with grammar rules for declarations, expressions, control flow, and functions. I also added semantic checks such as redeclaration, undeclared usage, type mismatch, and function argument mismatch. I can run lexer and parser separately, and I can show outputs either directly in terminal or save them to files for reporting."
