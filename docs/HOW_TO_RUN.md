# MinionLang - How to Run Guide

A simple guide to run and demonstrate the MinionLang compiler project.

---

## Quick Overview

This project has **two parts**:

| Part | Tool | What it does | Output |
|------|------|--------------|--------|
| **Part 1: Lexer** | Flex | Breaks code into tokens | List of tokens |
| **Part 2: Parser** | Bison | Checks if syntax is correct | Success or Error |

---

## Part 1: Lexer Only (Flex)

**What it does:** Takes MinionLang code and shows all the tokens (keywords, variables, operators, etc.)

### Step 1: Build the Lexer

Double-click `build.bat` OR run in terminal:
```
.\build.bat
```

This creates `minionlexer.exe`

### Step 2: Run the Lexer

```
.\minionlexer.exe tests\sample.minion tests\output.tokens
```

### Step 3: See the Output

```
type tests\output.tokens
```

### What You'll See

For this input code:
```
banana x := 10;
```

The lexer shows:
```
<TYPE_INT, banana, 1>
<IDENTIFIER, x, 1>
<OP_ASSIGN, :=, 1>
<INTEGER_LITERAL, 10, 1>
<SEMICOLON, ;, 1>
```

**Meaning:**
- `banana` → recognized as `TYPE_INT` (integer type)
- `x` → recognized as `IDENTIFIER` (variable name)
- `:=` → recognized as `OP_ASSIGN` (assignment operator)
- `10` → recognized as `INTEGER_LITERAL` (number)
- `;` → recognized as `SEMICOLON`

---

## Part 2: Parser (Flex + Bison)

**What it does:** Checks if the code follows correct grammar/syntax rules.

### Step 1: Build the Parser

Double-click `build_parser.bat` OR run in terminal:
```
.\build_parser.bat
```

This creates `minionparser.exe`

### Step 2: Run the Parser

**Test with valid code:**
```
.\minionparser.exe tests\valid_syntax.minion
```

**Test with code that has errors:**
```
.\minionparser.exe tests\syntax_error1.minion
```

### What You'll See

**For valid code:**
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
Program parsed successfully!

============================================================
  [OK] PARSING SUCCESSFUL - No syntax errors found!
============================================================
```

**For code with errors:**
```
============================================================
  SYNTAX ERROR
============================================================
  Line 13: syntax error
  Near token: 'kaba'
============================================================

  [FAIL] PARSING FAILED - 1 syntax error(s) found
============================================================
```

---

## Demo for Teacher

### If teacher wants to see ONLY the Lexer:

1. Open terminal in project folder
2. Run:
   ```
   .\build.bat
   .\minionlexer.exe tests\sample.minion tests\demo.tokens
   type tests\demo.tokens
   ```
3. Explain: "This shows how the lexer breaks code into tokens"

### If teacher wants to see ONLY the Parser:

1. Open terminal in project folder
2. Run:
   ```
   .\build_parser.bat
   .\minionparser.exe tests\valid_syntax.minion
   ```
3. Explain: "This checks if the syntax is correct"

4. Show error detection:
   ```
   .\minionparser.exe tests\syntax_error1.minion
   ```
5. Explain: "This detects syntax errors and shows the line number"

---

## Test Files Available

| File | Description | Use For |
|------|-------------|---------|
| `tests\sample.minion` | Complete example | Lexer demo |
| `tests\valid_syntax.minion` | Correct syntax | Parser success demo |
| `tests\syntax_error1.minion` | Missing semicolon | Parser error demo |
| `tests\syntax_error2.minion` | Missing braces | Parser error demo |
| `tests\syntax_error3.minion` | Invalid expression | Parser error demo |

---

## How the Parts Work Together

```
                    YOUR CODE
                        │
                        ▼
        ┌───────────────────────────────┐
        │         LEXER (Flex)          │
        │   - Reads characters           │
        │   - Identifies tokens          │
        │   - Removes comments           │
        │   - Tracks line numbers        │
        └───────────────┬───────────────┘
                        │
                    TOKENS
                        │
                        ▼
        ┌───────────────────────────────┐
        │        PARSER (Bison)         │
        │   - Receives tokens            │
        │   - Checks grammar rules       │
        │   - Reports syntax errors      │
        │   - Validates structure        │
        └───────────────────────────────┘
                        │
                        ▼
              SUCCESS or ERROR
```

---

## Simple Explanation for Viva

### What is the Lexer?
> "The lexer reads the source code character by character and groups them into tokens. For example, it recognizes `banana` as an integer type keyword, `10` as a number, and `:=` as the assignment operator."

### What is the Parser?
> "The parser takes the tokens from the lexer and checks if they follow the grammar rules. For example, a variable declaration must be: `type name := value ;`. If the semicolon is missing, the parser reports an error."

### Why two phases?
> "The lexer handles WHAT the pieces are (tokens). The parser handles HOW they fit together (structure). Splitting them makes the compiler simpler and easier to maintain."

---

## Quick Commands Summary

| What to do | Command |
|------------|---------|
| Build lexer | `.\build.bat` |
| Run lexer | `.\minionlexer.exe input.minion output.tokens` |
| Build parser | `.\build_parser.bat` |
| Run parser | `.\minionparser.exe input.minion` |
| See tokens | `type output.tokens` |

---

## Troubleshooting

**"flex not found"**
- Make sure you're in the project folder
- The `GnuWin32` folder must exist with flex.exe inside

**"gcc not found"**
- Install MinGW or add GCC to your PATH

**"Cannot open file"**
- Check if the file path is correct
- Make sure the .minion file exists

---

## File Structure

```
flexing/
├── build.bat              ← Build lexer
├── build_parser.bat       ← Build parser
├── minionlexer.exe        ← Lexer executable
├── minionparser.exe       ← Parser executable
├── src/
│   ├── minionlang.l       ← Lexer code (Flex)
│   ├── minionlang.y       ← Parser code (Bison)
│   └── minionlang_parser.l← Lexer for parser
├── tests/
│   ├── sample.minion      ← Test file
│   ├── valid_syntax.minion
│   └── syntax_error*.minion
└── docs/
    ├── LEXER_DOCUMENTATION.md
    ├── PARSER_DOCUMENTATION.md
    └── HOW_TO_RUN.md      ← This file
```

---

**That's it! You're ready to demonstrate your compiler project.**
