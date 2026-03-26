# Product Requirements Document (PRD)
## MinionLang Lexical Analyzer

**Project:** MinionLang Compiler - Lexer Phase  
**Version:** 1.0  
**Date:** February 16, 2026  
**Course:** Compiler Design Lab (CSE)

---

## 1. Executive Summary

This document specifies the requirements for building a **Flex-based lexical analyzer** for **MinionLang**, a toy programming language with Minion-themed keywords that maps to C semantics. The lexer will tokenize MinionLang source code and produce output suitable for later parsing with Bison.

---

## 2. Project Scope

### 2.1 In Scope
- Flex (.l) file implementation for MinionLang lexical analysis
- Token recognition for all MinionLang keywords, operators, identifiers, literals
- File-based input via `yyin`
- File-based output via `yyout`
- Generation of `lex.yy.cpp` (C++ lexer output)
- Comprehensive token output format for Bison integration

### 2.2 Out of Scope (Future Work)
- Bison parser implementation
- MinionLang to C translation
- Semantic analysis
- Code generation

---

## 3. Technical Requirements

### 3.1 Input/Output Specification

| Aspect | Requirement |
|--------|-------------|
| Input Source | File via `yyin` containing MinionLang source code |
| Output Target | File via `yyout` containing tokenized output |
| Output Format | C++ (`lex.yy.cpp` via `flex -o` or `flex --c++`) |
| Encoding | UTF-8 / ASCII |

### 3.2 Build Requirements

```
Flex version: 2.6+ recommended
Output: lex.yy.cpp (C++ mode)
Compilation: g++ lex.yy.cpp -o lexer -lfl
```

---

## 4. Token Specification

### 4.1 Preprocessor Directives

| Token Name | MinionLang Pattern | C Equivalent | Regex Pattern |
|------------|-------------------|--------------|---------------|
| `INCLUDE` | `#bringy` | `#include` | `#bringy` |
| `DEFINE` | `#setty` | `#define` | `#setty` |

### 4.2 Data Type Keywords

| Token Name | MinionLang Keyword | C Equivalent | Regex Pattern |
|------------|-------------------|--------------|---------------|
| `TYPE_INT` | `banana` | `int` | `banana` |
| `TYPE_FLOAT` | `smoothie` | `float` | `smoothie` |
| `TYPE_DOUBLE` | `megasmoot` | `double` | `megasmoot` |
| `TYPE_LONG_INT` | `loooong banana` | `long int` | `loooong[ \t]+banana` |
| `TYPE_CHAR` | `chibi` | `char` | `chibi` |
| `TYPE_BOOL` | `yesno` | `bool` | `yesno` |
| `TYPE_VOID` | `nada` | `void` | `nada` |

### 4.3 Control Flow Keywords

| Token Name | MinionLang Keyword | C Equivalent | Regex Pattern |
|------------|-------------------|--------------|---------------|
| `IF` | `kaba` | `if` | `kaba` |
| `ELSE_IF` | `bababa kaba` | `else if` | `bababa[ \t]+kaba` |
| `ELSE` | `bababa` | `else` | `bababa` |
| `FOR` | `againa` | `for` | `againa` |
| `WHILE` | `loopa` | `while` | `loopa` |
| `DO` | `dodo` | `do` | `dodo` |
| `SWITCH` | `choosey` | `switch` | `choosey` |
| `CASE` | `optiona` | `case` | `optiona` |
| `DEFAULT` | `otherwise` | `default` | `otherwise` |
| `BREAK` | `stoppo` | `break` | `stoppo` |
| `CONTINUE` | `gooo` | `continue` | `gooo` |
| `RETURN` | `backa` | `return` | `backa` |

### 4.4 Function Keywords

| Token Name | MinionLang Keyword | C Equivalent | Regex Pattern |
|------------|-------------------|--------------|---------------|
| `MAIN` | `starta` | `main` | `starta` |
| `PRINT` | `papoy` | `printf` | `papoy` |
| `SCAN` | `takey` | `scanf` | `takey` |
| `RETURN_TYPE_INDICATOR` | `:->` | (return type syntax) | `:->` |

### 4.5 Math Functions

| Token Name | MinionLang Keyword | C Equivalent | Regex Pattern |
|------------|-------------------|--------------|---------------|
| `FUNC_POWER` | `power` | `pow` | `power` |
| `FUNC_SQRT` | `root` | `sqrt` | `root` |
| `FUNC_FLOOR` | `flr` | `floor` | `flr` |
| `FUNC_CEIL` | `ceil` | `ceil` | `ceil` |
| `FUNC_ABS` | `abs` | `abs` | `abs` |
| `FUNC_LOG` | `loggy` | `log` | `loggy` |
| `FUNC_SIN` | `sine` | `sin` | `sine` |
| `FUNC_COS` | `cosine` | `cos` | `cosine` |
| `FUNC_TAN` | `tan` | `tan` | `tan` |
| `FUNC_ASIN` | `asine` | `asin` | `asine` |
| `FUNC_ACOS` | `acosine` | `acos` | `acosine` |
| `FUNC_ATAN` | `atan` | `atan` | `atan` |
| `FUNC_PRIME` | `primey` | (custom) | `primey` |

### 4.6 Arithmetic Operators

| Token Name | MinionLang Symbol | C Equivalent | Regex Pattern |
|------------|------------------|--------------|---------------|
| `OP_ADD` | `add` | `+` | `add` |
| `OP_SUB` | `minus` | `-` | `minus` |
| `OP_MUL` | `mul` | `*` | `mul` |
| `OP_DIV` | `divide` | `/` | `divide` |
| `OP_MOD` | `mod` | `%` | `mod` |

### 4.7 Logical Operators

| Token Name | MinionLang Symbol | C Equivalent | Regex Pattern |
|------------|------------------|--------------|---------------|
| `OP_AND` | `AND` | `&&` | `AND` |
| `OP_OR` | `OR` | `\|\|` | `OR` |
| `OP_NOT` | `NOT` | `!` | `NOT` |
| `OP_XOR` | `XOR` | `^` | `XOR` |

### 4.8 Relational Operators

| Token Name | MinionLang Symbol | C Equivalent | Regex Pattern |
|------------|------------------|--------------|---------------|
| `OP_LT` | `<` | `<` | `<` |
| `OP_GT` | `>` | `>` | `>` |
| `OP_LE` | `<=` | `<=` | `<=` |
| `OP_GE` | `>=` | `>=` | `>=` |
| `OP_EQ` | `==` | `==` | `==` |
| `OP_NE` | `!=` | `!=` | `!=` |

### 4.9 Assignment Operator

| Token Name | MinionLang Symbol | C Equivalent | Regex Pattern |
|------------|------------------|--------------|---------------|
| `OP_ASSIGN` | `:=` | `=` | `:=` |

### 4.10 Punctuation & Delimiters

| Token Name | Symbol | Description | Regex Pattern |
|------------|--------|-------------|---------------|
| `SEMICOLON` | `;` | Statement terminator | `;` |
| `COLON` | `:` | Colon (for loop separator) | `:` |
| `COMMA` | `,` | Comma separator | `,` |
| `LPAREN` | `(` | Left parenthesis | `\(` |
| `RPAREN` | `)` | Right parenthesis | `\)` |
| `LBRACE` | `{` | Left curly brace | `\{` |
| `RBRACE` | `}` | Right curly brace | `\}` |
| `LBRACKET` | `[` | Left square bracket | `\[` |
| `RBRACKET` | `]` | Right square bracket | `\]` |

### 4.11 Comments

| Token Name | MinionLang Pattern | C Equivalent | Regex Pattern |
|------------|-------------------|--------------|---------------|
| `COMMENT_SINGLE` | `$$...` | `//...` | `\$\$.*` |
| `COMMENT_MULTI` | `$* ... *$` | `/* ... */` | `\$\*([^*]\|\*+[^$])*\*\$` |

### 4.12 Literals

| Token Name | Description | Regex Pattern |
|------------|-------------|---------------|
| `INTEGER_LITERAL` | Integer numbers | `[0-9]+` |
| `FLOAT_LITERAL` | Floating point numbers | `[0-9]+\.[0-9]+` |
| `STRING_LITERAL` | String in double quotes | `\"([^\"\n])*\"` |
| `CHAR_LITERAL` | Character in single quotes | `\'([^\'\n])\'` |

### 4.13 Identifiers

| Token Name | Description | Regex Pattern |
|------------|-------------|---------------|
| `IDENTIFIER` | Variable/function names | `[A-Za-z_][A-Za-z0-9_]*` |

### 4.14 Special

| Token Name | Description | Regex Pattern |
|------------|-------------|---------------|
| `HEADER_FILE` | Header file inclusion | `<[A-Za-z0-9_\.]+>` |
| `NEWLINE` | Line tracking | `\n` |
| `WHITESPACE` | Spaces/tabs (ignored) | `[ \t]+` |

---

## 5. Token Output Format

Each recognized token should be output in the following format for Bison compatibility:

```
<TOKEN_TYPE, lexeme, line_number>
```

**Example Output:**
```
<TYPE_INT, banana, 3>
<IDENTIFIER, x, 3>
<OP_ASSIGN, :=, 3>
<INTEGER_LITERAL, 10, 3>
<SEMICOLON, ;, 3>
```

---

## 6. Lexer Structure Requirements

### 6.1 Flex File Structure (.l)

```
%{
    /* C/C++ declarations */
    /* Include necessary headers */
    /* Define global variables (line counter, etc.) */
%}

/* Definitions section */
/* Define regex macros like DIGIT, LETTER, etc. */

%%
/* Rules section */
/* Pattern-action pairs for all tokens */
%%

/* User code section */
/* main() function with yyin/yyout handling */
```

### 6.2 Required Features

1. **Line Number Tracking**: Maintain `yylineno` or custom counter
2. **Error Handling**: Report unrecognized characters with line number
3. **Comment Handling**: Properly skip single-line (`$$`) and multi-line (`$* ... *$`) comments
4. **Whitespace Handling**: Ignore spaces and tabs, track newlines
5. **Longest Match**: Flex default behavior (e.g., `bababa kaba` before `bababa`)

### 6.3 Pattern Priority (Order Matters!)

The following patterns MUST be ordered correctly to avoid conflicts:

1. Multi-word keywords first: `loooong banana`, `bababa kaba`
2. Single keywords: `banana`, `bababa`, `kaba`, etc.
3. Multi-character operators: `:=`, `:->`, `<=`, `>=`, `==`, `!=`
4. Single-character operators: `<`, `>`, `:`, etc.
5. Literals: integers, floats, strings, chars
6. Identifiers: last among meaningful tokens
7. Whitespace and newlines
8. Error catch-all: `.` for any unrecognized character

---

## 7. Example MinionLang Input

```minionlang
#bringy <stdio.h>

banana starta() {
    banana x := 10;
    smoothie y := 3.14;

    kaba (x > 5) {
        papoy("Value is greater than 5");
    } bababa {
        papoy("Value is small");
    }

    $$ This is a comment
    $* This is a
       multi-line comment *$

    againa(banana i := 0 : i < 10 : i := i add 1) {
        papoy("Loop iteration");
    }

    backa 0;
}
```

---

## 8. Expected Token Output

For the above input, the lexer should produce:

```
<INCLUDE, #bringy, 1>
<HEADER_FILE, <stdio.h>, 1>
<TYPE_INT, banana, 3>
<MAIN, starta, 3>
<LPAREN, (, 3>
<RPAREN, ), 3>
<LBRACE, {, 3>
<TYPE_INT, banana, 4>
<IDENTIFIER, x, 4>
<OP_ASSIGN, :=, 4>
<INTEGER_LITERAL, 10, 4>
<SEMICOLON, ;, 4>
<TYPE_FLOAT, smoothie, 5>
<IDENTIFIER, y, 5>
<OP_ASSIGN, :=, 5>
<FLOAT_LITERAL, 3.14, 5>
<SEMICOLON, ;, 5>
<IF, kaba, 7>
<LPAREN, (, 7>
<IDENTIFIER, x, 7>
<OP_GT, >, 7>
<INTEGER_LITERAL, 5, 7>
<RPAREN, ), 7>
<LBRACE, {, 7>
<PRINT, papoy, 8>
<LPAREN, (, 8>
<STRING_LITERAL, "Value is greater than 5", 8>
<RPAREN, ), 8>
<SEMICOLON, ;, 8>
<RBRACE, }, 9>
<ELSE, bababa, 9>
<LBRACE, {, 9>
<PRINT, papoy, 10>
<LPAREN, (, 10>
<STRING_LITERAL, "Value is small", 10>
<RPAREN, ), 10>
<SEMICOLON, ;, 10>
<RBRACE, }, 11>
<COMMENT_SINGLE, $$ This is a comment, 13>
<COMMENT_MULTI, $* This is a\n   multi-line comment *$, 14>
<FOR, againa, 17>
<LPAREN, (, 17>
<TYPE_INT, banana, 17>
<IDENTIFIER, i, 17>
<OP_ASSIGN, :=, 17>
<INTEGER_LITERAL, 0, 17>
<COLON, :, 17>
<IDENTIFIER, i, 17>
<OP_LT, <, 17>
<INTEGER_LITERAL, 10, 17>
<COLON, :, 17>
<IDENTIFIER, i, 17>
<OP_ASSIGN, :=, 17>
<IDENTIFIER, i, 17>
<OP_ADD, add, 17>
<INTEGER_LITERAL, 1, 17>
<RPAREN, ), 17>
<LBRACE, {, 17>
<PRINT, papoy, 18>
<LPAREN, (, 18>
<STRING_LITERAL, "Loop iteration", 18>
<RPAREN, ), 18>
<SEMICOLON, ;, 18>
<RBRACE, }, 19>
<RETURN, backa, 21>
<INTEGER_LITERAL, 0, 21>
<SEMICOLON, ;, 21>
<RBRACE, }, 22>
```

---

## 9. Build & Execution Instructions

### 9.1 Generate Lexer
```bash
flex -o lex.yy.cpp minionlang.l
```

Or for C++ class-based output:
```bash
flex --c++ -o lex.yy.cpp minionlang.l
```

### 9.2 Compile Lexer
```bash
g++ lex.yy.cpp -o minionlexer -lfl
```

### 9.3 Run Lexer
```bash
./minionlexer input.minion output.tokens
```

Or with redirection:
```bash
./minionlexer < input.minion > output.tokens
```

---

## 10. Error Handling Requirements

| Error Type | Action |
|------------|--------|
| Unrecognized character | Print error with line number, continue lexing |
| Unterminated string | Print error with line number |
| Unterminated multi-line comment | Print error at EOF |

**Error Output Format:**
```
LEXICAL ERROR at line X: Unrecognized character 'c'
```

---

## 11. File Naming Convention

| File | Purpose |
|------|---------|
| `minionlang.l` | Flex source file |
| `lex.yy.cpp` | Generated C++ lexer |
| `minionlexer` / `minionlexer.exe` | Compiled executable |
| `*.minion` | MinionLang source files |
| `*.tokens` | Token output files |

---

## 12. Success Criteria

1. ✅ All MinionLang keywords are correctly recognized and tokenized
2. ✅ Identifiers are distinguished from keywords
3. ✅ Integer and float literals are correctly identified
4. ✅ String and character literals are handled properly
5. ✅ All operators (arithmetic, logical, relational, assignment) are tokenized
6. ✅ Comments (single and multi-line) are recognized and handled
7. ✅ Line numbers are tracked and reported
8. ✅ Input via `yyin` and output via `yyout` work correctly
9. ✅ Error handling for unrecognized tokens
10. ✅ Generates valid `lex.yy.cpp` that compiles without errors

---

## 13. Deliverables

1. **minionlang.l** - Complete Flex specification file
2. **lex.yy.cpp** - Generated lexer (for verification)
3. **Sample input files** - Test cases in MinionLang
4. **Sample output files** - Expected token outputs

---

## 14. Appendix: Complete Token Reference Table

| # | Token Name | MinionLang | C Equivalent | Category |
|---|------------|------------|--------------|----------|
| 1 | INCLUDE | #bringy | #include | Preprocessor |
| 2 | DEFINE | #setty | #define | Preprocessor |
| 3 | TYPE_INT | banana | int | Data Type |
| 4 | TYPE_FLOAT | smoothie | float | Data Type |
| 5 | TYPE_DOUBLE | megasmoot | double | Data Type |
| 6 | TYPE_LONG_INT | loooong banana | long int | Data Type |
| 7 | TYPE_CHAR | chibi | char | Data Type |
| 8 | TYPE_BOOL | yesno | bool | Data Type |
| 9 | TYPE_VOID | nada | void | Data Type |
| 10 | IF | kaba | if | Control Flow |
| 11 | ELSE_IF | bababa kaba | else if | Control Flow |
| 12 | ELSE | bababa | else | Control Flow |
| 13 | FOR | againa | for | Control Flow |
| 14 | WHILE | loopa | while | Control Flow |
| 15 | DO | dodo | do | Control Flow |
| 16 | SWITCH | choosey | switch | Control Flow |
| 17 | CASE | optiona | case | Control Flow |
| 18 | DEFAULT | otherwise | default | Control Flow |
| 19 | BREAK | stoppo | break | Control Flow |
| 20 | CONTINUE | gooo | continue | Control Flow |
| 21 | RETURN | backa | return | Control Flow |
| 22 | MAIN | starta | main | Function |
| 23 | PRINT | papoy | printf | Function |
| 24 | SCAN | takey | scanf | Function |
| 25 | RETURN_TYPE_IND | :-> | (syntax) | Function |
| 26 | FUNC_POWER | power | pow | Math |
| 27 | FUNC_SQRT | root | sqrt | Math |
| 28 | FUNC_FLOOR | flr | floor | Math |
| 29 | FUNC_CEIL | ceil | ceil | Math |
| 30 | FUNC_ABS | abs | abs | Math |
| 31 | FUNC_LOG | loggy | log | Math |
| 32 | FUNC_SIN | sine | sin | Math |
| 33 | FUNC_COS | cosine | cos | Math |
| 34 | FUNC_TAN | tan | tan | Math |
| 35 | FUNC_ASIN | asine | asin | Math |
| 36 | FUNC_ACOS | acosine | acos | Math |
| 37 | FUNC_ATAN | atan | atan | Math |
| 38 | FUNC_PRIME | primey | (custom) | Math |
| 39 | OP_ADD | add | + | Arithmetic |
| 40 | OP_SUB | minus | - | Arithmetic |
| 41 | OP_MUL | mul | * | Arithmetic |
| 42 | OP_DIV | divide | / | Arithmetic |
| 43 | OP_MOD | mod | % | Arithmetic |
| 44 | OP_AND | AND | && | Logical |
| 45 | OP_OR | OR | \|\| | Logical |
| 46 | OP_NOT | NOT | ! | Logical |
| 47 | OP_XOR | XOR | ^ | Logical |
| 48 | OP_LT | < | < | Relational |
| 49 | OP_GT | > | > | Relational |
| 50 | OP_LE | <= | <= | Relational |
| 51 | OP_GE | >= | >= | Relational |
| 52 | OP_EQ | == | == | Relational |
| 53 | OP_NE | != | != | Relational |
| 54 | OP_ASSIGN | := | = | Assignment |
| 55 | SEMICOLON | ; | ; | Punctuation |
| 56 | COLON | : | : | Punctuation |
| 57 | COMMA | , | , | Punctuation |
| 58 | LPAREN | ( | ( | Punctuation |
| 59 | RPAREN | ) | ) | Punctuation |
| 60 | LBRACE | { | { | Punctuation |
| 61 | RBRACE | } | } | Punctuation |
| 62 | LBRACKET | [ | [ | Punctuation |
| 63 | RBRACKET | ] | ] | Punctuation |
| 64 | IDENTIFIER | [A-Za-z_]... | variable | Identifier |
| 65 | INTEGER_LITERAL | [0-9]+ | number | Literal |
| 66 | FLOAT_LITERAL | [0-9]+.[0-9]+ | decimal | Literal |
| 67 | STRING_LITERAL | "..." | string | Literal |
| 68 | CHAR_LITERAL | '.' | char | Literal |
| 69 | COMMENT_SINGLE | $$... | //... | Comment |
| 70 | COMMENT_MULTI | $*...*$ | /*...*/ | Comment |
| 71 | HEADER_FILE | <...> | <...> | Special |

---

**End of PRD Document**
