# MinionLang Parser Documentation

**Course:** Compiler Design Lab (CSE)  
**Phase:** Syntax Analysis (Parsing)  
**Tools:** Flex (Lexer) + Bison (Parser)  
**Date:** February 2026

---

## Table of Contents
1. [Introduction](#1-introduction)
2. [What is Parsing/Syntax Analysis?](#2-what-is-parsingsyntax-analysis)
3. [What is Bison?](#3-what-is-bison)
4. [Grammar Specification](#4-grammar-specification)
5. [Parser File Structure (.y)](#5-parser-file-structure-y)
6. [How the Parser Works](#6-how-the-parser-works)
7. [Building and Running](#7-building-and-running)
8. [Error Detection Examples](#8-error-detection-examples)
9. [Technical Details for Viva](#9-technical-details-for-viva)
10. [Common Questions & Answers](#10-common-questions--answers)

---

## 1. Introduction

This document explains the **MinionLang Parser**, the second phase of our compiler that performs **syntax analysis**. It uses **Bison** (GNU Parser Generator) integrated with **Flex** (Lexer) to:

- Verify the grammatical structure of MinionLang programs
- Detect and report **syntax errors** with line numbers
- Build the foundation for semantic analysis (future phase)

### What the Parser Does

```
Source Code → [Lexer] → Token Stream → [Parser] → Syntax Valid/Error
```

**Input:** Stream of tokens from the lexer  
**Output:** Success message OR syntax error with location

---

## 2. What is Parsing/Syntax Analysis?

**Parsing** (or Syntax Analysis) is the second phase of compilation that:

1. Takes **tokens** from the lexer
2. Checks if they follow the **grammar rules**
3. Reports **syntax errors** if rules are violated

### Example: Valid vs Invalid

**Valid MinionLang:**
```minionlang
banana x := 10;    ✓ Correct: type identifier := value ;
```

**Invalid MinionLang:**
```minionlang
banana x := 10     ✗ Error: Missing semicolon
banana := 10;      ✗ Error: Missing identifier
banana x 10;       ✗ Error: Missing assignment operator
```

### Lexer vs Parser

| Aspect | Lexer (Flex) | Parser (Bison) |
|--------|--------------|----------------|
| **Phase** | 1st (Lexical) | 2nd (Syntax) |
| **Input** | Character stream | Token stream |
| **Output** | Token stream | Parse tree / Error |
| **Checks** | Token validity | Grammar rules |
| **Grammar** | Regular grammar | Context-free grammar |
| **Detects** | Invalid characters | Invalid structure |

---

## 3. What is Bison?

**Bison** is a parser generator that:
- Takes a **grammar specification** (`.y` file)
- Generates **C code** for a parser
- Uses **LALR(1)** parsing algorithm

### How Bison Works

```
┌─────────────┐       ┌──────────┐       ┌─────────────────┐
│ .y file     │──────>│  Bison   │──────>│ minionlang.tab.c│
│ (grammar)   │       │  Tool    │       │ (parser code)   │
└─────────────┘       └──────────┘       └─────────────────┘
                                                 │
                           Also creates:         │
                      ┌─────────────────┐        │
                      │ minionlang.tab.h│        │
                      │ (token defs)    │        │
                      └─────────────────┘        │
                                                 │
                      Together with:             │
                      ┌─────────────────┐        │
                      │ lex.yy.c        │        │
                      │ (from Flex)     │        │
                      └─────────────────┘        │
                                                 │
                                    gcc          │
                                    ▼            │
                              ┌─────────────┐    │
                              │ parser.exe  │◄───┘
                              │ (executable)│
                              └─────────────┘
```

### LALR(1) Parsing
- **L**ook **A**head **L**eft-to-right, **R**ightmost derivation
- **1** token lookahead
- Efficient bottom-up parsing
- Can handle most programming language grammars

---

## 4. Grammar Specification

### 4.1 Context-Free Grammar (CFG)

MinionLang grammar is specified as a **Context-Free Grammar**:

```
program → global_declarations

global_declarations → global_declaration
                    | global_declarations global_declaration

global_declaration → preprocessor_directive
                   | function_definition
                   | declaration SEMICOLON

type_specifier → TYPE_INT | TYPE_FLOAT | TYPE_DOUBLE 
               | TYPE_LONG_INT | TYPE_CHAR | TYPE_BOOL | TYPE_VOID

declaration → type_specifier declarator_list

declarator → IDENTIFIER
           | IDENTIFIER OP_ASSIGN expression
           | IDENTIFIER LBRACKET INTEGER_LITERAL RBRACKET
```

### 4.2 Key Grammar Rules

#### Function Definition
```
function_definition:
    type_specifier MAIN LPAREN RPAREN compound_statement
    | type_specifier IDENTIFIER LPAREN parameter_list RPAREN compound_statement
```

Matches:
```minionlang
banana starta() { ... }
banana addTwo(banana a, banana b) { ... }
```

#### If-Else Statement
```
selection_statement:
    IF LPAREN expression RPAREN statement
    | IF LPAREN expression RPAREN statement ELSE statement
    | IF LPAREN expression RPAREN statement ELSE_IF LPAREN expression RPAREN statement
```

Matches:
```minionlang
kaba (x > 5) { ... }
kaba (x > 5) { ... } bababa { ... }
kaba (x > 5) { ... } bababa kaba (x < 0) { ... }
```

#### For Loop
```
iteration_statement:
    FOR LPAREN for_init COLON expression COLON for_update RPAREN statement
```

Matches:
```minionlang
againa(banana i := 0 : i < 10 : i := i add 1) { ... }
```

#### Variable Declaration
```
declaration:
    type_specifier declarator_list SEMICOLON

declarator:
    IDENTIFIER OP_ASSIGN expression
```

Matches:
```minionlang
banana x := 10;
smoothie pi := 3.14;
```

---

## 5. Parser File Structure (.y)

### 5.1 Bison File Sections

A `.y` file has **four sections**:

```yacc
%{
/* Prologue: C declarations */
#include <stdio.h>
extern int yylex();
void yyerror(const char* s);
%}

/* Bison declarations */
%token TOKEN_NAME1 TOKEN_NAME2
%left OP_ADD OP_SUB
%right OP_ASSIGN

%%
/* Grammar rules */
program: declaration_list { printf("Success!"); }
       ;
%%

/* Epilogue: C code */
int main() {
    return yyparse();
}
```

### 5.2 Our Parser Structure

#### Token Declarations
```yacc
/* Data Types */
%token TYPE_INT TYPE_FLOAT TYPE_DOUBLE TYPE_LONG_INT TYPE_CHAR TYPE_BOOL TYPE_VOID

/* Control Flow */
%token IF ELSE_IF ELSE FOR WHILE DO SWITCH CASE DEFAULT BREAK CONTINUE RETURN

/* Operators */
%token OP_ADD OP_SUB OP_MUL OP_DIV OP_MOD
%token OP_AND OP_OR OP_NOT OP_XOR
%token OP_LT OP_GT OP_LE OP_GE OP_EQ OP_NE
%token OP_ASSIGN
```

#### Operator Precedence
```yacc
%left OP_OR
%left OP_AND
%left OP_XOR
%left OP_EQ OP_NE
%left OP_LT OP_GT OP_LE OP_GE
%left OP_ADD OP_SUB
%left OP_MUL OP_DIV OP_MOD
%right OP_NOT
%right UMINUS
```

**Why precedence matters:**
- `x add y mul z` → `x add (y mul z)` (mul before add)
- `NOT x AND y` → `(NOT x) AND y` (NOT before AND)

### 5.3 Integration with Flex

**Bison generates:** `minionlang.tab.h` with token definitions

**Flex includes:** `#include "minionlang.tab.h"`

**Flex returns tokens:** `return TYPE_INT;` instead of `print_token(...)`

---

## 6. How the Parser Works

### 6.1 Parsing Process

1. **Parser calls `yylex()`** to get next token
2. **Token is matched** against grammar rules
3. **Shift/Reduce** operations build parse tree
4. **Accept** if input matches start symbol
5. **Error** if no valid production

### 6.2 Shift-Reduce Parsing

**Shift:** Push token onto stack
**Reduce:** Apply grammar rule, replace RHS with LHS

**Example:** Parsing `banana x := 10;`

```
Stack                    Input                Action
-----                    -----                ------
$                        TYPE_INT IDENTIFIER OP_ASSIGN INTEGER_LITERAL SEMICOLON $    Shift
$ TYPE_INT               IDENTIFIER OP_ASSIGN INTEGER_LITERAL SEMICOLON $            Shift
$ TYPE_INT IDENTIFIER    OP_ASSIGN INTEGER_LITERAL SEMICOLON $                       Shift
$ TYPE_INT IDENTIFIER OP_ASSIGN    INTEGER_LITERAL SEMICOLON $                       Shift
$ TYPE_INT IDENTIFIER OP_ASSIGN INTEGER_LITERAL    SEMICOLON $                       Reduce: expr→INTEGER_LITERAL
$ TYPE_INT IDENTIFIER OP_ASSIGN expression    SEMICOLON $                            Reduce: declarator→IDENTIFIER OP_ASSIGN expression
$ TYPE_INT declarator    SEMICOLON $                                                 Reduce: declaration→TYPE_INT declarator
$ declaration    SEMICOLON $                                                         Shift
$ declaration SEMICOLON    $                                                         Reduce: statement→declaration SEMICOLON
$ statement    $                                                                     Accept!
```

### 6.3 Error Recovery

When syntax error occurs:
1. Parser reports error with line number
2. Shows the token that caused the error
3. Continues parsing to find more errors (optional)

```
============================================================
  SYNTAX ERROR
============================================================
  Line 13: syntax error
  Near token: 'kaba'
============================================================
```

---

## 7. Building and Running

### 7.1 Build Steps

```bash
# Step 1: Generate parser from grammar
bison -d minionlang.y
# Output: minionlang.tab.c, minionlang.tab.h

# Step 2: Generate lexer
flex minionlang_parser.l
# Output: lex.yy.c

# Step 3: Compile everything
gcc -o minionparser.exe minionlang.tab.c lex.yy.c -I.
```

### 7.2 Using build_parser.bat

```bash
.\build_parser.bat
```

This automates all build steps.

### 7.3 Running the Parser

**Syntax check a file:**
```bash
.\minionparser.exe <filename.minion>
```

**Examples:**
```bash
# Check valid file
.\minionparser.exe tests\valid_syntax.minion

# Check file with errors
.\minionparser.exe tests\syntax_error1.minion
```

### 7.4 Understanding Output

**Successful parse:**
```
============================================================
         MinionLang Parser - Syntax Analyzer
============================================================

Parsing file: tests\valid_syntax.minion
============================================================

  [Line 4] Include directive parsed
  [Line 13] Variable declared and initialized
  [Line 27] If-else statement parsed
  ...
✓ Program parsed successfully!

============================================================
  [OK] PARSING SUCCESSFUL - No syntax errors found!
============================================================
```

**Failed parse:**
```
============================================================
         MinionLang Parser - Syntax Analyzer
============================================================

Parsing file: tests\syntax_error1.minion
============================================================

  [Line 5] Include directive parsed
  [Line 13] Variable declared and initialized

============================================================
  SYNTAX ERROR
============================================================
  Line 13: syntax error
  Near token: 'kaba'
============================================================

============================================================
  [FAIL] PARSING FAILED - 1 syntax error(s) found
============================================================
```

---

## 8. Error Detection Examples

### 8.1 Missing Semicolon

**Code:**
```minionlang
banana x := 10    $$ Missing semicolon
kaba (x > 5) {
```

**Error:**
```
Line 2: syntax error
Near token: 'kaba'
```
*Parser expected `;` but got `kaba`*

### 8.2 Missing Parenthesis

**Code:**
```minionlang
kaba (x > 5 {    $$ Missing )
    papoy("Test");
}
```

**Error:**
```
Line 1: syntax error
Near token: '{'
```
*Parser expected `)` but got `{`*

### 8.3 Missing Closing Brace

**Code:**
```minionlang
kaba (x > 5) {
    papoy("Test");
$$ Missing closing brace
backa 0;
```

**Error:**
```
Line 4: syntax error
Near token: 'backa'
```
*Parser expected `}` but got `backa`*

### 8.4 Invalid Assignment

**Code:**
```minionlang
banana = 10;    $$ Missing identifier
```

**Error:**
```
Line 1: syntax error
Near token: '='
```
*`=` is not a valid MinionLang operator (should be `:=`)*

### 8.5 Invalid Expression

**Code:**
```minionlang
banana x := 5 add;    $$ Missing operand
```

**Error:**
```
Line 1: syntax error  
Near token: ';'
```
*Parser expected expression after `add` but got `;`*

---

## 9. Technical Details for Viva

### 9.1 Compilation Phases

```
┌─────────────────────────────────────────────────────────┐
│                  Compiler Pipeline                       │
├─────────────────────────────────────────────────────────┤
│  Source Code                                             │
│       │                                                  │
│       ▼                                                  │
│  ┌─────────────────┐                                    │
│  │ Lexical Analysis│  ← Flex (minionlang.l)             │
│  │    (Lexer)      │                                    │
│  └────────┬────────┘                                    │
│           │ Tokens                                       │
│           ▼                                              │
│  ┌─────────────────┐                                    │
│  │ Syntax Analysis │  ← Bison (minionlang.y) ★ WE ARE   │
│  │    (Parser)     │                          HERE      │
│  └────────┬────────┘                                    │
│           │ Parse Tree                                   │
│           ▼                                              │
│  ┌─────────────────┐                                    │
│  │ Semantic        │  ← Future phase                    │
│  │   Analysis      │                                    │
│  └────────┬────────┘                                    │
│           ▼                                              │
│  ┌─────────────────┐                                    │
│  │ Code Generation │  ← Future phase                    │
│  └────────┬────────┘                                    │
│           ▼                                              │
│     Target Code                                          │
└─────────────────────────────────────────────────────────┘
```

### 9.2 Types of Grammars

| Grammar Type | Power | Used For | Tool |
|--------------|-------|----------|------|
| Regular | Weakest | Tokens (lexer) | Flex |
| Context-Free | Medium | Syntax (parser) | Bison |
| Context-Sensitive | Strong | Semantics | Manual code |

### 9.3 Parsing Algorithms

| Algorithm | Type | Used By |
|-----------|------|---------|
| Recursive Descent | Top-down | Hand-written parsers |
| LL(1) | Top-down | ANTLR |
| **LALR(1)** | **Bottom-up** | **Bison** ✓ |
| LR(1) | Bottom-up | Theoretical |

### 9.4 Shift-Reduce Conflicts

**Conflict:** Parser doesn't know whether to shift or reduce

**Example:** Dangling else problem
```
if (a) if (b) x; else y;
```
Does `else` belong to outer or inner `if`?

**Resolution:** Bison uses precedence rules or explicit grammar restructuring.

### 9.5 Our Parser Statistics

- **Token Types:** 71
- **Grammar Rules:** ~80
- **Precedence Levels:** 9
- **Conflicts:** 10 shift/reduce, 1 reduce/reduce (acceptable)

---

## 10. Common Questions & Answers

### Q1: What is the difference between CFG and Regular Grammar?

**Answer:**
- **Regular Grammar:** Can only express patterns (identifiers, numbers)
- **Context-Free Grammar:** Can express nested structures (if-else, loops)
- Example: Matching parentheses `()` needs CFG, not possible with regex

### Q2: Why Bison over hand-written parser?

**Answer:**
- **Automatic generation** from grammar specification
- **Proven algorithms** (LALR parsing)
- **Conflict detection** during generation
- **Easier maintenance** - just update grammar
- **Integration** with Flex

### Q3: What are shift-reduce conflicts?

**Answer:**
- Parser can't decide: push token (shift) or apply rule (reduce)
- Usually resolved by precedence declarations
- `%left`, `%right`, `%nonassoc` keywords

### Q4: How does precedence work?

**Answer:**
```yacc
%left OP_ADD OP_SUB      // + - same level, left associative
%left OP_MUL OP_DIV      // * / higher level
```
- `a add b mul c` → `a add (b mul c)`
- `a minus b add c` → `(a minus b) add c`

### Q5: What is yyparse()?

**Answer:**
- Main parsing function generated by Bison
- Calls `yylex()` to get tokens
- Returns 0 on success, 1 on error
- Uses grammar rules to validate structure

### Q6: What is yyerror()?

**Answer:**
- Error handling function (we implement)
- Called when parser finds syntax error
- Receives error message from Bison
- We add line number and token information

### Q7: How to add new language features?

**Answer:**
1. Add token to lexer (`.l` file)
2. Declare token in parser (`%token`)
3. Add grammar rule in parser (`.y` file)
4. Rebuild both lexer and parser

### Q8: What errors can the parser NOT detect?

**Answer:**
- **Type errors:** `banana x := "hello";` (string to int)
- **Undeclared variables:** Using variable before declaration
- **Logic errors:** `x := x divide 0;`
- These require **semantic analysis** (next phase)

### Q9: How does the parser track line numbers?

**Answer:**
```c
extern int yylineno;  // Declared in lexer
```
- Lexer increments `yylineno` on newline
- Parser reads it when error occurs
- Reports accurate error location

### Q10: What is the token header file (.tab.h)?

**Answer:**
- Generated by `bison -d`
- Contains `#define` for each token
- Example: `#define TYPE_INT 258`
- Shared between lexer and parser
- Ensures token values match

---

## 11. Quick Reference

### Build Commands
```bash
# Build parser
.\build_parser.bat

# Manual build
bison -d src\minionlang.y
flex src\minionlang_parser.l  
gcc -o minionparser.exe src\minionlang.tab.c src\lex.yy.c -I src
```

### Run Commands
```bash
# Check syntax
.\minionparser.exe tests\valid_syntax.minion
.\minionparser.exe tests\syntax_error1.minion
```

### Key Files
| File | Description |
|------|-------------|
| `minionlang.y` | Bison grammar specification |
| `minionlang_parser.l` | Flex lexer for parser |
| `minionlang.tab.c` | Generated parser |
| `minionlang.tab.h` | Generated token definitions |
| `lex.yy.c` | Generated lexer |

### Error Message Format
```
============================================================
  SYNTAX ERROR
============================================================
  Line N: syntax error
  Near token: 'TOKEN'
============================================================
```

---

## 12. Summary

### What We Achieved
✅ **Bison parser** for MinionLang  
✅ **Syntax error detection** with line numbers  
✅ **Grammar coverage** for all language constructs  
✅ **Integration** with Flex lexer  
✅ **Production-ready** error messages  

### Key Concepts
1. **Parsing** checks grammatical structure
2. **Bison** generates LALR(1) parsers
3. **CFG** expresses programming language syntax
4. **Shift-reduce** is bottom-up parsing
5. **Precedence** resolves ambiguity

### Future Work
- ✨ Semantic analysis (type checking)
- ✨ Symbol table management
- ✨ Abstract Syntax Tree (AST)
- ✨ Code generation

---

**End of Parser Documentation**

*"Bello! Your MinionLang syntax is now verified!"*
