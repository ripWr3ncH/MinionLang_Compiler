# MinionLang Lexical Analyzer Documentation

**Course:** Compiler Design Lab (CSE)  
**Project:** MinionLang Compiler - Lexer Phase  
**Date:** February 2026  
**Tool:** Flex (Fast Lexical Analyzer Generator)

---

## Table of Contents
1. [Introduction](#1-introduction)
2. [What is Lexical Analysis?](#2-what-is-lexical-analysis)
3. [What is Flex?](#3-what-is-flex)
4. [MinionLang Overview](#4-minionlang-overview)
5. [Flex File Structure (.l)](#5-flex-file-structure-l)
6. [Token Recognition Strategy](#6-token-recognition-strategy)
7. [How the Lexer Works](#7-how-the-lexer-works)
8. [Building and Running](#8-building-and-running)
9. [Example Walkthrough](#9-example-walkthrough)
10. [Technical Details for Viva](#10-technical-details-for-viva)
11. [Common Questions & Answers](#11-common-questions--answers)

---

## 1. Introduction

This document explains the **MinionLang Lexical Analyzer**, a Flex-based tool that performs the first phase of compilation: **lexical analysis** (tokenization). The lexer reads MinionLang source code and breaks it down into meaningful tokens that will later be used by a parser (Bison) for syntax analysis.

### Key Features
- Recognizes **71 different token types**
- Handles all MinionLang keywords, operators, literals, and identifiers
- Supports single-line (`$$`) and multi-line (`$* *$`) comments
- Tracks line numbers for error reporting
- File-based input/output via `yyin` and `yyout`

---

## 2. What is Lexical Analysis?

**Lexical analysis** is the first phase of compilation that converts a stream of characters into a stream of tokens.

### Example:
```c
banana x := 10;
```

**Lexical Analysis Output:**
```
<TYPE_INT, banana, 1>
<IDENTIFIER, x, 1>
<OP_ASSIGN, :=, 1>
<INTEGER_LITERAL, 10, 1>
<SEMICOLON, ;, 1>
```

### Why is it needed?
1. **Simplifies parsing** - Parser works with tokens, not raw characters
2. **Removes whitespace** - Unnecessary spaces/tabs are discarded
3. **Handles comments** - Comments are recognized and skipped
4. **Error detection** - Identifies invalid characters early
5. **Tokenization** - Groups characters into meaningful units

---

## 3. What is Flex?

**Flex** (Fast Lexical Analyzer Generator) is a tool for generating **lexers** (scanners). It takes a specification file (`.l`) containing pattern-action rules and generates C code that recognizes those patterns.

### How Flex Works:
```
┌─────────────┐       ┌──────────┐       ┌─────────────┐
│ .l file     │──────>│  Flex    │──────>│  lex.yy.c   │
│ (patterns)  │       │  Tool    │       │  (C code)   │
└─────────────┘       └──────────┘       └─────────────┘
                                                 │
                                                 ▼
                                          ┌─────────────┐
                                          │   gcc       │
                                          │  (compile)  │
                                          └─────────────┘
                                                 │
                                                 ▼
                                          ┌─────────────┐
                                          │  lexer.exe  │
                                          │ (executable)│
                                          └─────────────┘
```

### Key Concepts:
- **Regular Expressions**: Patterns that describe tokens
- **Pattern Matching**: Flex uses DFA (Deterministic Finite Automaton)
- **Longest Match Rule**: Always matches the longest possible token
- **Input Buffering**: Efficient scanning of large files

---

## 4. MinionLang Overview

MinionLang is a **C-like programming language** with Minion-themed keywords. It maintains C semantics while using fun, thematic keywords.

### Design Philosophy:
- **Based on C** - Same structure and grammar as C
- **Keyword Replacement** - C keywords → Minion keywords
- **Lexically Valid** - Not a joke language, formally defined
- **Educational** - Demonstrates compiler design concepts

### Example Code Comparison:

**C Code:**
```c
int main() {
    int x = 10;
    if (x > 5) {
        printf("Hello");
    }
    return 0;
}
```

**MinionLang Code:**
```minionlang
banana starta() {
    banana x := 10;
    kaba (x > 5) {
        papoy("Hello");
    }
    backa 0;
}
```

### Key Mappings:
| MinionLang | C Equivalent | Category |
|------------|--------------|----------|
| `banana` | `int` | Data type |
| `starta` | `main` | Function |
| `kaba` | `if` | Control flow |
| `papoy` | `printf` | I/O |
| `backa` | `return` | Control flow |
| `:=` | `=` | Assignment |
| `add` | `+` | Operator |
| `$$` | `//` | Comment |

---

## 5. Flex File Structure (.l)

A Flex file has **three main sections** separated by `%%`:

```
%{
    /* Declarations Section */
    /* C/C++ code, headers, variables */
%}

/* Definitions Section */
/* Regex macros and options */

%%
/* Rules Section */
/* Pattern-action pairs */
%%

/* User Code Section */
/* main() and helper functions */
```

### 5.1 Declarations Section

```c
%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int yylineno = 1;  // Track line numbers

void print_token(const char* token_type, const char* lexeme) {
    fprintf(yyout, "<%s, %s, %d>\n", token_type, lexeme, yylineno);
}
%}
```

**Purpose:**
- Include necessary headers
- Declare global variables
- Define helper functions
- Set up line number tracking

### 5.2 Definitions Section

```c
%option noyywrap

DIGIT       [0-9]
LETTER      [A-Za-z]
ALPHANUM    [A-Za-z0-9_]
WHITESPACE  [ \t]+
NEWLINE     \n
```

**Purpose:**
- Set Flex options (`%option noyywrap` - don't wrap to another file)
- Define regex macros for reusability
- Makes patterns more readable

### 5.3 Rules Section

This is the **core** of the lexer. Each rule has:
- **Pattern** (left side): Regular expression to match
- **Action** (right side): C code to execute when matched

```c
%%
"banana"                    { print_token("TYPE_INT", yytext); }
"kaba"                      { print_token("IF", yytext); }
[0-9]+                      { print_token("INTEGER_LITERAL", yytext); }
[A-Za-z_][A-Za-z0-9_]*      { print_token("IDENTIFIER", yytext); }
[ \t]+                      { /* ignore whitespace */ }
\n                          { yylineno++; }
.                           { print_error("Unrecognized", yytext); }
%%
```

**Special Variables:**
- `yytext` - Matched text (string)
- `yyleng` - Length of matched text
- `yylineno` - Current line number
- `yyin` - Input file pointer
- `yyout` - Output file pointer

### 5.4 User Code Section

```c
int main(int argc, char* argv[]) {
    // Open input file
    yyin = fopen(argv[1], "r");
    
    // Open output file
    yyout = fopen(argv[2], "w");
    
    // Run lexer
    yylex();
    
    // Cleanup
    fclose(yyin);
    fclose(yyout);
    
    return 0;
}
```

**Purpose:**
- Contains `main()` function
- Handles file I/O
- Calls `yylex()` to start scanning
- Additional helper functions

---

## 6. Token Recognition Strategy

### 6.1 Pattern Ordering (Critical!)

Flex uses **first-match** principle, but applies **longest match** rule. The order of patterns matters!

**Correct Order:**
1. **Multi-word keywords** first: `loooong banana`, `bababa kaba`
2. **Single keywords**: `banana`, `bababa`, `kaba`
3. **Multi-character operators**: `:=`, `:->`, `<=`, `>=`, `==`, `!=`
4. **Single-character operators**: `<`, `>`, `:`
5. **Literals**: floats before integers (float contains dot)
6. **Identifiers**: last (most general)
7. **Error catch-all**: `.` matches any single character

### 6.2 Why Order Matters?

**Wrong Order:**
```c
"bababa"                    { print_token("ELSE", yytext); }
"bababa"[ \t]+"kaba"        { print_token("ELSE_IF", yytext); }
```
Problem: `bababa kaba` would be tokenized as `ELSE` + `IDENTIFIER(kaba)`

**Correct Order:**
```c
"bababa"[ \t]+"kaba"        { print_token("ELSE_IF", yytext); }
"bababa"                    { print_token("ELSE", yytext); }
```
Result: `bababa kaba` correctly recognized as `ELSE_IF`

### 6.3 Longest Match Rule

Flex always matches the **longest possible** string.

**Example:**
Input: `loooong banana`

Patterns:
- `banana` → matches "banana"
- `loooong[ \t]+banana` → matches "loooong banana"

Result: Flex chooses the longer match → `TYPE_LONG_INT`

---

## 7. How the Lexer Works

### 7.1 Finite Automaton

Flex compiles patterns into a **DFA (Deterministic Finite Automaton)**:

```
Input: "banana"

State transitions:
Start → 'b' → 'a' → 'n' → 'a' → 'n' → 'a' → ACCEPT (TYPE_INT)
```

### 7.2 Scanning Process

1. **Read character** from input
2. **Transition** to next state in DFA
3. **Continue** until no valid transition
4. **Accept** longest match
5. **Execute action** for matched pattern
6. **Repeat** until EOF

### 7.3 Comment Handling

**Single-line comment:**
```c
"\$\$".*                    { print_token("COMMENT_SINGLE", yytext); }
```
- `\$\$` - Matches literal `$$`
- `.*` - Matches any characters until newline

**Multi-line comment:**
```c
"\$\*"([^*]|\*+[^\$])*"\*\$"  { 
    count_newlines(yytext);
    print_token("COMMENT_MULTI", "(multiline comment)"); 
}
```
- `\$\*` - Matches `$*`
- `([^*]|\*+[^\$])*` - Matches content (handles nested `*` correctly)
- `\*\$` - Matches `*$`

### 7.4 String Literal Handling

```c
\"([^\"\n\\]|\\.)*\"        { print_token("STRING_LITERAL", yytext); }
```
- `\"` - Opening quote
- `([^\"\n\\]|\\.)*` - Any char except quote/newline/backslash, OR escape sequence
- `\"` - Closing quote

This handles: `"Hello"`, `"Line 1\nLine 2"`, `"Quote: \""`

### 7.5 Number Recognition

```c
{DIGIT}+"."{DIGIT}+         { print_token("FLOAT_LITERAL", yytext); }
{DIGIT}+                    { print_token("INTEGER_LITERAL", yytext); }
```

**Note:** Float pattern **must come before** integer!
- Input: `3.14`
- Without proper ordering: would match as `INTEGER(3)` + `.` + `INTEGER(14)`
- With correct ordering: matches as `FLOAT_LITERAL(3.14)`

---

## 8. Building and Running

### 8.1 Project Structure

```
flexing/
├── src/
│   └── minionlang.l          # Flex specification
├── tests/
│   ├── sample.minion         # Test input
│   └── sample.tokens         # Test output
├── docs/
│   ├── PRD.md                # Requirements
│   └── LEXER_DOCUMENTATION.md # This file
├── GnuWin32/
│   └── bin/
│       └── flex.exe          # Flex tool
├── build.bat                 # Build script
├── run_test.bat              # Test script
├── lex.yy.c                  # Generated C code
└── minionlexer.exe           # Compiled lexer
```

### 8.2 Build Steps

**Step 1: Generate C Code**
```bash
.\GnuWin32\bin\flex.exe src\minionlang.l
```
Output: `lex.yy.c` (61KB C source file)

**Step 2: Compile**
```bash
gcc -o minionlexer.exe lex.yy.c
```
Output: `minionlexer.exe` (executable)

**Step 3: Run**
```bash
.\minionlexer.exe tests\sample.minion tests\sample.tokens
```

### 8.3 Automated Build

**Using build.bat:**
```batch
.\build.bat
```

This script:
1. Checks for Flex installation
2. Generates `lex.yy.c`
3. Compiles with GCC
4. Creates `minionlexer.exe`

### 8.4 Usage

```bash
minionlexer.exe <input_file> [output_file]
```

**Arguments:**
- `input_file`: MinionLang source file (`.minion`)
- `output_file`: Token output file (optional, defaults to stdout)

**Example:**
```bash
# Output to file
.\minionlexer.exe code.minion tokens.txt

# Output to console
.\minionlexer.exe code.minion
```

---

## 9. Example Walkthrough

### 9.1 Input Code

**File: `simple.minion`**
```minionlang
$$ Simple MinionLang program
banana starta() {
    banana x := 10;
    kaba (x > 5) {
        papoy("Greater");
    }
    backa 0;
}
```

### 9.2 Tokenization Process

**Line 1:** `$$ Simple MinionLang program`
```
Pattern matched: "\$\$".*
Token: <COMMENT_SINGLE, $$ Simple MinionLang program, 1>
```

**Line 2:** `banana starta() {`

Character by character:
1. `b-a-n-a-n-a` + space → Matches `"banana"` → `<TYPE_INT, banana, 2>`
2. Space → Ignored
3. `s-t-a-r-t-a` + `(` → Matches `"starta"` → `<MAIN, starta, 2>`
4. `(` → Matches `\(` → `<LPAREN, (, 2>`
5. `)` → Matches `\)` → `<RPAREN, ), 2>`
6. Space → Ignored
7. `{` → Matches `\{` → `<LBRACE, {, 2>`

**Line 3:** `    banana x := 10;`

1. Spaces → Ignored
2. `banana` → `<TYPE_INT, banana, 3>`
3. Space → Ignored
4. `x` → Matches identifier pattern → `<IDENTIFIER, x, 3>`
5. Space → Ignored
6. `:=` → Matches multi-char operator → `<OP_ASSIGN, :=, 3>`
7. Space → Ignored
8. `10` → Matches integer pattern → `<INTEGER_LITERAL, 10, 3>`
9. `;` → `<SEMICOLON, ;, 3>`

### 9.3 Complete Output

**File: `tokens.txt`**
```
=====================================
MinionLang Lexical Analyzer Output
Input file: simple.minion
=====================================

<COMMENT_SINGLE, $$ Simple MinionLang program, 1>
<TYPE_INT, banana, 2>
<MAIN, starta, 2>
<LPAREN, (, 2>
<RPAREN, ), 2>
<LBRACE, {, 2>
<TYPE_INT, banana, 3>
<IDENTIFIER, x, 3>
<OP_ASSIGN, :=, 3>
<INTEGER_LITERAL, 10, 3>
<SEMICOLON, ;, 3>
<IF, kaba, 4>
<LPAREN, (, 4>
<IDENTIFIER, x, 4>
<OP_GT, >, 4>
<INTEGER_LITERAL, 5, 4>
<RPAREN, ), 4>
<LBRACE, {, 4>
<PRINT, papoy, 5>
<LPAREN, (, 5>
<STRING_LITERAL, "Greater", 5>
<RPAREN, ), 5>
<SEMICOLON, ;, 5>
<RBRACE, }, 6>
<RETURN, backa, 7>
<INTEGER_LITERAL, 0, 7>
<SEMICOLON, ;, 7>
<RBRACE, }, 8>

=====================================
Lexical analysis complete.
Total lines processed: 8
=====================================
```

---

## 10. Technical Details for Viva

### 10.1 Compilation Phases

```
┌──────────────────────────────────────────────────────────┐
│                 Compiler Pipeline                         │
├──────────────────────────────────────────────────────────┤
│                                                           │
│  Source Code                                              │
│       │                                                   │
│       ▼                                                   │
│  ┌─────────────────┐                                     │
│  │ Lexical Analysis│ ◄── OUR PROJECT (FLEX)              │
│  │    (Lexer)      │                                     │
│  └────────┬────────┘                                     │
│           │ Tokens                                        │
│           ▼                                               │
│  ┌─────────────────┐                                     │
│  │ Syntax Analysis │ ◄── Future: Bison                   │
│  │    (Parser)     │                                     │
│  └────────┬────────┘                                     │
│           │ Parse Tree                                    │
│           ▼                                               │
│  ┌─────────────────┐                                     │
│  │ Semantic        │                                     │
│  │   Analysis      │                                     │
│  └────────┬────────┘                                     │
│           │ Annotated Tree                                │
│           ▼                                               │
│  ┌─────────────────┐                                     │
│  │ Intermediate    │                                     │
│  │ Code Generation │                                     │
│  └────────┬────────┘                                     │
│           ▼                                               │
│  ┌─────────────────┐                                     │
│  │  Optimization   │                                     │
│  └────────┬────────┘                                     │
│           ▼                                               │
│  ┌─────────────────┐                                     │
│  │ Code Generation │                                     │
│  └────────┬────────┘                                     │
│           ▼                                               │
│     Machine Code                                          │
│                                                           │
└──────────────────────────────────────────────────────────┘
```

### 10.2 Regular Expression Theory

**What are Regular Expressions?**
- Formal notation for describing patterns in strings
- Used to specify tokens in lexical analysis

**Examples in our lexer:**

| Pattern | Regex | Meaning |
|---------|-------|---------|
| Identifier | `[A-Za-z_][A-Za-z0-9_]*` | Letter/underscore + alphanumeric* |
| Integer | `[0-9]+` | One or more digits |
| Float | `[0-9]+\.[0-9]+` | Digits + dot + digits |
| Keyword | `banana` | Literal string |

**Regex Operators:**
- `*` - Zero or more
- `+` - One or more
- `?` - Zero or one
- `|` - OR (alternation)
- `[]` - Character class
- `.` - Any character
- `\` - Escape special char

### 10.3 DFA (Deterministic Finite Automaton)

Flex converts regex patterns to DFA for efficient scanning.

**Example: Recognizing "banana"**

```
State Machine:
    b       a       n       a       n       a
(S0)→(S1)→(S2)→(S3)→(S4)→(S5)→(S6)
                                    ↓
                              [ACCEPT: TYPE_INT]
```

**Properties:**
- Deterministic: One unique transition per input
- Linear time: O(n) where n = input length
- Memory efficient: Compiled into C code

### 10.4 Token Categories

Our lexer recognizes 71 token types in 14 categories:

1. **Preprocessor** (2): `#bringy`, `#setty`
2. **Data Types** (7): `banana`, `smoothie`, `megasmoot`, etc.
3. **Control Flow** (12): `kaba`, `bababa`, `againa`, `loopa`, etc.
4. **Functions** (4): `starta`, `papoy`, `takey`, `:->` 
5. **Math Functions** (13): `power`, `root`, `sine`, etc.
6. **Arithmetic Operators** (5): `add`, `minus`, `mul`, etc.
7. **Logical Operators** (4): `AND`, `OR`, `NOT`, `XOR`
8. **Relational Operators** (6): `<`, `>`, `<=`, `>=`, `==`, `!=`
9. **Assignment** (1): `:=`
10. **Punctuation** (9): `;`, `:`, `,`, `()`, `{}`, `[]`
11. **Literals** (4): integer, float, string, char
12. **Identifiers** (1): variable/function names
13. **Comments** (2): single-line, multi-line
14. **Special** (1): header files

### 10.5 Error Handling

**Types of errors detected:**

1. **Unrecognized character:**
   ```c
   .   { print_error("Unrecognized character", yytext); }
   ```
   Example: `@` → `LEXICAL ERROR at line 5: Unrecognized character '@'`

2. **Unterminated string:**
   ```c
   \"([^\"\n\\]|\\.)*{NEWLINE}  { print_error("Unterminated string", ""); }
   ```
   Example: `"Hello` (missing closing quote)

3. **Unterminated comment:**
   - Detected if `$*` found but no matching `*$`
   - Flex will reach EOF without matching

### 10.6 Line Number Tracking

**Method 1: Manual tracking**
```c
int yylineno = 1;

{NEWLINE}   { yylineno++; }
```

**Method 2: Flex option**
```c
%option yylineno
```

In multi-line constructs (comments, strings), count embedded newlines:
```c
void count_newlines(const char* text) {
    for (int i = 0; text[i] != '\0'; i++) {
        if (text[i] == '\n') yylineno++;
    }
}
```

---

## 11. Common Questions & Answers

### Q1: Why use Flex instead of writing a lexer manually?

**Answer:**
- **Faster development**: Flex generates optimized C code automatically
- **Proven correctness**: Well-tested tool used in industry
- **Efficient**: Generated DFA is optimized for speed
- **Maintainable**: Pattern rules are easier to read than manual state machines
- **Integration**: Works seamlessly with Bison parser generator

Manual lexer: 500+ lines of complex state machine code  
Flex lexer: 200 lines of readable pattern rules

### Q2: What is the difference between lexer and parser?

**Answer:**

| Aspect | Lexer | Parser |
|--------|-------|--------|
| **Input** | Character stream | Token stream |
| **Output** | Token stream | Parse tree |
| **Phase** | 1st (Lexical analysis) | 2nd (Syntax analysis) |
| **Grammar** | Regular grammar | Context-free grammar |
| **Tool** | Flex | Bison |
| **Checks** | Token validity | Syntax validity |

**Example:**
```
Input: "banana x := 10;"

Lexer → <TYPE_INT>, <IDENTIFIER>, <OP_ASSIGN>, <INTEGER_LITERAL>, <SEMICOLON>

Parser → Checks: "Is this a valid declaration statement?"
```

### Q3: How does Flex handle ambiguity?

**Answer:**

Flex uses two rules:

1. **Longest Match**: Always prefer longer match
   - Input: `loooong banana`
   - Options: `banana` (6 chars) vs `loooong banana` (14 chars)
   - Result: Choose `loooong banana`

2. **First Match**: If same length, choose pattern listed first
   ```c
   "bababa kaba"   { /* Rule 1 */ }
   "bababa"        { /* Rule 2 */ }
   ```
   For input `bababa kaba`, Rule 1 is tried first and matches.

### Q4: What is the purpose of `yytext` and `yyleng`?

**Answer:**

- **`yytext`**: Pointer to the matched text (string)
- **`yyleng`**: Length of matched text (integer)

**Example:**
```c
Input: "banana"

When pattern "banana" matches:
  yytext = "banana"
  yyleng = 6
```

**Usage:**
```c
"banana"    { 
    printf("Found keyword: %s (length: %d)\n", yytext, yyleng);
    print_token("TYPE_INT", yytext); 
}
```

### Q5: How do you handle keywords vs identifiers?

**Answer:**

**Problem:** Keywords look like identifiers (e.g., `banana` could be a variable name)

**Solution:** List keywords **before** the identifier pattern

```c
/* Keywords first */
"banana"                    { return TYPE_INT; }
"kaba"                      { return IF; }
"backa"                     { return RETURN; }

/* Identifiers last (most general) */
[A-Za-z_][A-Za-z0-9_]*      { return IDENTIFIER; }
```

**Why this works:**
- Flex tries patterns in order
- `banana` matches keyword pattern first
- `myVariable` doesn't match any keyword, falls to identifier pattern

### Q6: Why is `%option noyywrap` needed?

**Answer:**

By default, when Flex reaches EOF, it calls `yywrap()` to check if there's another input file.

**Without noyywrap:**
```c
int yywrap() {
    return 1;  // 1 = no more files
}
```
You must define `yywrap()` manually.

**With noyywrap:**
```c
%option noyywrap
```
Flex generates code assuming single-file input. Simpler!

### Q7: How are multi-line comments handled?

**Answer:**

**Pattern:**
```c
"\$\*"([^*]|\*+[^\$])*"\*\$"
```

**Breakdown:**
- `\$\*` - Match `$*` (opening)
- `([^*]|\*+[^\$])*` - Match any char that's not `*`, OR `*` not followed by `$`
- `\*\$` - Match `*$` (closing)

**This handles:**
```minionlang
$* Simple comment *$
$* Comment with * inside *$
$* Multi
   line
   comment *$
```

**Line tracking:**
```c
count_newlines(yytext);  // Count \n characters in matched text
```

### Q8: What happens if an invalid character is found?

**Answer:**

The catch-all pattern at the end:
```c
.   { print_error("Unrecognized character", yytext); }
```

**Example:**
```minionlang
banana x := @10;
            ^
```

**Output:**
```
LEXICAL ERROR at line 5: Unrecognized character '@'
```

The lexer continues scanning after the error (error recovery).

### Q9: How does the lexer differentiate between `<` (less-than) and `<stdio.h>` (header)?

**Answer:**

**Order matters!**

```c
/* Header file pattern (more specific) */
"<"[A-Za-z0-9_\.]+">"{WHITESPACE}?  { return HEADER_FILE; }

/* Less-than operator (more general) */
"<"                                  { return OP_LT; }
```

For `<stdio.h>`:
- First pattern tries to match: `<` + alphanumeric/dot + `>`
- Success! → `HEADER_FILE`

For standalone `<`:
- First pattern tries but fails (no matching `>`)
- Second pattern matches → `OP_LT`

### Q10: Can we extend this lexer to support more features?

**Answer:**

**Yes! Easy additions:**

1. **Hex numbers:**
```c
"0x"[0-9A-Fa-f]+    { print_token("HEX_LITERAL", yytext); }
```

2. **Scientific notation:**
```c
[0-9]+"."?[0-9]*[eE][+-]?[0-9]+  { print_token("SCIENTIFIC", yytext); }
```

3. **New operators:**
```c
"power_assign"      { print_token("OP_POWER_ASSIGN", yytext); }
```

4. **Nested comments** (requires state management):
```c
%x COMMENT
%%
"$*"                { BEGIN(COMMENT); comment_depth = 1; }
<COMMENT>"$*"       { comment_depth++; }
<COMMENT>"*$"       { if (--comment_depth == 0) BEGIN(INITIAL); }
<COMMENT>.          { /* consume */ }
```

### Q11: How is this integrated with Bison?

**Answer:**

**Current (Flex only):**
```c
"banana"    { print_token("TYPE_INT", yytext); }
```

**Future (Flex + Bison):**
```c
"banana"    { return TYPE_INT; }  // Return token type to parser
```

**Bison parser will:**
1. Call `yylex()` to get next token
2. Use grammar rules to check syntax
3. Build parse tree

**Example grammar (Bison):**
```yacc
declaration: type_specifier identifier ASSIGN expression SEMICOLON
           ;

type_specifier: TYPE_INT
              | TYPE_FLOAT
              | TYPE_CHAR
              ;
```

### Q12: What optimizations does Flex make?

**Answer:**

1. **DFA minimization**: Reduces number of states
2. **Table compression**: Stores transition tables efficiently
3. **Input buffering**: Reads chunks, not single characters
4. **Inline actions**: Small actions embedded directly in state machine
5. **Fast table lookup**: Uses arrays for O(1) transitions

**Performance:**
- Generated lexer can scan **millions of tokens per second**
- Linear time complexity: O(n) for input of length n

---

## 12. Demonstration Script

### For Teacher/Viva:

**1. Show the source code:**
```bash
type src\minionlang.l
```
Explain structure: declarations, definitions, rules, user code

**2. Build the lexer:**
```bash
.\build.bat
```
Show generated `lex.yy.c` file size (~61KB)

**3. Show a test file:**
```bash
type tests\sample.minion
```
Point out keywords, operators, comments

**4. Run the lexer:**
```bash
.\minionlexer.exe tests\sample.minion tests\sample.tokens
```

**5. Show output:**
```bash
type tests\sample.tokens | more
```
Explain token format: `<TYPE, lexeme, line_number>`

**6. Test error handling:**
Create `error.minion`:
```minionlang
banana x @= 10;
```

Run:
```bash
.\minionlexer.exe error.minion output.txt
```

Show: `LEXICAL ERROR at line 1: Unrecognized character '@'`

**7. Explain token flow:**
```
Source Code → Lexer → Tokens → (Future: Parser → Parse Tree)
```

---

## 13. Summary

### What We Achieved:

✅ **Complete lexical analyzer** for MinionLang  
✅ **71 token types** recognized  
✅ **Comments** (single & multi-line)  
✅ **Error handling** with line numbers  
✅ **File I/O** via yyin/yyout  
✅ **Ready for Bison integration**  

### Key Takeaways:

1. **Lexical analysis** is the first compiler phase
2. **Flex** automates lexer generation from patterns
3. **Pattern ordering** is critical for correctness
4. **Regular expressions** define token patterns
5. **DFA** provides efficient scanning
6. **Token stream** feeds into parser (next phase)

### Future Work:

- ✨ Bison parser for syntax analysis
- ✨ MinionLang → C translation
- ✨ Semantic analysis
- ✨ Code optimization
- ✨ Full compiler pipeline

---

## 14. References

1. **Flex Manual**: https://github.com/westes/flex
2. **Compilers: Principles, Techniques, and Tools** (Dragon Book) - Aho, Sethi, Ullman
3. **Modern Compiler Implementation in C** - Andrew Appel
4. **Flex & Bison** - John Levine (O'Reilly)

---

**End of Documentation**

*This document should provide comprehensive understanding for explaining the project to teachers and answering viva questions.*
