# MinionLang Compiler

**A Flex & Bison-based compiler for the MinionLang programming language**

![Status](https://img.shields.io/badge/Status-Complete-success)
![Flex](https://img.shields.io/badge/Flex-2.5.4-blue)
![Bison](https://img.shields.io/badge/Bison-2.4.1-blue)
![Language](https://img.shields.io/badge/Language-C-orange)
![Tokens](https://img.shields.io/badge/Tokens-71_Types-green)

---

## 📋 Overview

This project implements a **complete educational compiler workflow** for **MinionLang**, a C-inspired programming language with Minion-themed keywords. It includes lexer (Flex), parser (Bison), semantic checks, execution-proof translation, and a browser-based MinionLang playground for quick experimentation.

### What is MinionLang?

MinionLang is an educational programming language that:
- Uses **Minion-themed keywords** instead of C keywords
- Maintains **C-like syntax and semantics**
- Demonstrates **compiler design principles**
- Is **formally defined** (not a joke language)

**Example:**
```minionlang
$$ MinionLang program
banana starta() {
    banana x := 10;
    kaba (x > 5) {
        papoy("Hello MinionLang!");
    }
    backa 0;
}
```

---

## 🚀 Quick Start

If `make` is not available in your Windows shell, use `mingw32-make` with the same targets.

### Where to Run Commands

If you are inside project root (`MinionLang_Compiler`):

```bash
mingw32-make all
```

If you are in parent folder (`compiler`), use `-C`:

```bash
mingw32-make -C .\MinionLang_Compiler all
```

### Option 1: Build Everything (Recommended)

```bash
make all
```

This builds:
- lexer (`minionlexer.exe`)
- parser (`minionparser.exe`)
- translator (`minion2c.exe`)
- IR generator (`minionir.exe`)

### Option 2: Run Individual Phase Commands

```bash
make lexer
make parser
make translator
make ir
```

### Option 3: Run Core Tests

```bash
make lexer-test
make syntax-tests
make semantic-tests
```

### Show Output in Terminal or File

Lexer output to terminal:

```bash
mingw32-make lexer
.\minionlexer.exe tests\sample.minion
```

Lexer output to file:

```bash
mingw32-make lexer
.\minionlexer.exe tests\sample.minion tests\sample.tokens
type tests\sample.tokens
```

Parser output to terminal:

```bash
mingw32-make parser
.\minionparser.exe tests\valid_syntax.minion
```

Parser output saved to file:

```bash
mingw32-make parser
.\minionparser.exe tests\valid_syntax.minion > tests\valid_syntax.parse.log 2>&1
type tests\valid_syntax.parse.log
```

### Option 4: Run Full Lab Demo (All Checks)

```bash
make full-demo
```

This runs:
- build parser/lexer/translator
- build IR generator
- valid syntax parse (expected pass)
- syntax error tests (expected fail)
- semantic error tests including scope/return/function-call/undefined-call checks (expected fail)
- sample translation generation
- IR demo generation

Expected summary:
- Passed checks: 22
- Failed checks: 0
- FULL DEMO STATUS: PASS

### Option 5: Execution-Proof Runtime Tests

```bash
make exec-proof
```

This runs 3 executable MinionLang test programs through:
- translation to C
- gcc compilation
- binary execution
- expected-output comparison

### Option 6: IR + Constant Folding Demo (Optional)

```bash
make ir-demo
```

This generates a simple 3-address IR and folds constant-only expressions.

### Option 7: Web Playground (Minion Theme)

Open in browser after deployment or local static hosting:

```bash
web/index.html
```

Features:
- Write MinionLang in a code editor
- Run code in-browser (transpiled JS runtime subset)
- View output, diagnostics, tokens, and transpiled JavaScript
- Minion-based UI theme

### Final Lab Command Sequence

```bash
make parser
make semantic-tests
make exec-proof
make ir-demo
make full-demo
```

## Vercel Deployment (GitHub)

This repository is already Vercel-ready via `vercel.json`.

1. Import repository in Vercel.
2. Keep Framework Preset as `Other`.
3. Keep build command empty.
4. Deploy.

Vercel serves the playground from:
- `/` -> `web/index.html`

### Manual Build

```bash
# === LEXER ONLY ===
.\GnuWin32\bin\flex.exe src\minionlang.l
gcc -o minionlexer.exe lex.yy.c

# === PARSER (Flex + Bison) ===
set PATH=%CD%\GnuWin32\bin;%PATH%
cd src
bison -d minionlang.y
flex minionlang_parser.l
cd ..
gcc -o minionparser.exe src\minionlang.tab.c src\lex.yy.c -I src
```

---

## 📁 Project Structure

```
flexing/
├── src/
│   ├── minionlang.l          # Flex lexer (standalone tokenizer)
│   ├── minionlang_parser.l   # Flex lexer (Bison integration)
│   ├── minionlang.y          # Bison parser grammar
│   ├── minionlang.tab.c      # Generated parser (after build)
│   ├── minionlang.tab.h      # Generated token definitions
│   └── lex.yy.c              # Generated lexer
├── tests/
│   ├── sample.minion         # Comprehensive test file
│   ├── valid_syntax.minion   # Valid syntax test
│   ├── syntax_error1.minion  # Error test: missing semicolon
│   ├── syntax_error2.minion  # Error test: missing braces
│   └── syntax_error3.minion  # Error test: invalid expressions
│   └── sample.tokens         # Expected output
├── docs/
│   ├── PRD.md                # Product Requirements Document
│   ├── LEXER_DOCUMENTATION.md # Complete technical documentation
│   ├── VIVA_QUICK_REFERENCE.md # Quick reference for presentations
│   └── README.md             # This file
├── contexts/
│   └── context.md            # Project context and language design
├── GnuWin32/
│   └── bin/
│       └── flex.exe          # Flex tool (2.5.4)
├── Makefile                  # Unified build/test runner
├── run_execution_proof_tests.bat # End-to-end runtime proof tests
├── run_full_demo.bat         # Full demonstration workflow
├── lex.yy.c                  # Generated C code (after build)
└── minionlexer.exe           # Compiled lexer (after build)
```

---

## 🎯 Features

### Token Recognition
- ✅ **71 token types** across 14 categories
- ✅ **Keywords**: `banana`, `kaba`, `papoy`, `starta`, etc.
- ✅ **Operators**: `add`, `minus`, `AND`, `OR`, `:=`, `==`, etc.
- ✅ **Literals**: integers, floats, strings, characters
- ✅ **Comments**: single-line (`$$`) and multi-line (`$* *$`)
- ✅ **Identifiers**: variable and function names
- ✅ **Preprocessor**: `#bringy`, `#setty`

### Capabilities
- 🔹 **Line number tracking** for all tokens
- 🔹 **Error reporting** with line numbers
- 🔹 **File I/O** via `yyin` and `yyout`
- 🔹 **Longest match** algorithm
- 🔹 **Whitespace handling**
- 🔹 **Ready for Bison integration**

---

## 📖 Documentation

| Document | Description | Use Case |
|----------|-------------|----------|
| [PRD.md](docs/PRD.md) | Complete requirements specification | Understanding project scope |
| [LEXER_DOCUMENTATION.md](docs/LEXER_DOCUMENTATION.md) | Full technical documentation | Deep dive into implementation |
| [PARSER_DOCUMENTATION.md](docs/PARSER_DOCUMENTATION.md) | Parser grammar and integration | Syntax analysis explanation |
| [TESTING_AND_DEBUGGING.md](docs/TESTING_AND_DEBUGGING.md) | Build, run, expected outputs | Lab demonstration and debugging |
| [COMPLETE_VIVA_PREP_GUIDE.md](docs/COMPLETE_VIVA_PREP_GUIDE.md) | End-to-end viva preparation | Answering teacher questions confidently |
| [VIVA_QUICK_REFERENCE.md](docs/VIVA_QUICK_REFERENCE.md) | Quick reference guide | Viva/presentation preparation |
| [COMPLETE_PROJECT_EXPLANATION.md](docs/COMPLETE_PROJECT_EXPLANATION.md) | Beginner-friendly full project explanation | Learning and explaining the full project easily |
| [understand.md](docs/understand.md) | End-to-end lexer/parser learning guide | Understanding `.l` and `.y` with run/output workflow |
| [context.md](contexts/context.md) | Language design and mappings | Understanding MinionLang |

---

## 🔤 MinionLang Keyword Mappings

### Common Keywords

| MinionLang | C Equivalent | Category |
|------------|--------------|----------|
| `banana` | `int` | Data type |
| `smoothie` | `float` | Data type |
| `chibi` | `char` | Data type |
| `starta` | `main` | Function |
| `papoy` | `printf` | I/O |
| `takey` | `scanf` | I/O |
| `kaba` | `if` | Control flow |
| `bababa` | `else` | Control flow |
| `bababa kaba` | `else if` | Control flow |
| `againa` | `for` | Loop |
| `loopa` | `while` | Loop |
| `backa` | `return` | Control flow |

### Operators

| MinionLang | C Equivalent | Type |
|------------|--------------|------|
| `:=` | `=` | Assignment |
| `add` | `+` | Arithmetic |
| `minus` | `-` | Arithmetic |
| `mul` | `*` | Arithmetic |
| `AND` | `&&` | Logical |
| `OR` | `\|\|` | Logical |
| `NOT` | `!` | Logical |

### Comments

| MinionLang | C Equivalent |
|------------|--------------|
| `$$ comment` | `// comment` |
| `$* comment *$` | `/* comment */` |

**[See complete mappings in context.md](contexts/context.md)**

---

## 🧪 Example Usage

### Input: `example.minion`
```minionlang
$$ Calculate sum
banana starta() {
    banana a := 5;
    banana b := 10;
    banana sum := a add b;
    papoy("Sum is: %d", sum);
    backa 0;
}
```

### Command:
```bash
.\minionlexer.exe example.minion output.tokens
```

### Output: `output.tokens`
```
=====================================
MinionLang Lexical Analyzer Output
Input file: example.minion
=====================================

<COMMENT_SINGLE, $$ Calculate sum, 1>
<TYPE_INT, banana, 2>
<MAIN, starta, 2>
<LPAREN, (, 2>
<RPAREN, ), 2>
<LBRACE, {, 2>
<TYPE_INT, banana, 3>
<IDENTIFIER, a, 3>
<OP_ASSIGN, :=, 3>
<INTEGER_LITERAL, 5, 3>
<SEMICOLON, ;, 3>
<TYPE_INT, banana, 4>
<IDENTIFIER, b, 4>
<OP_ASSIGN, :=, 4>
<INTEGER_LITERAL, 10, 4>
<SEMICOLON, ;, 4>
<TYPE_INT, banana, 5>
<IDENTIFIER, sum, 5>
<OP_ASSIGN, :=, 5>
<IDENTIFIER, a, 5>
<OP_ADD, add, 5>
<IDENTIFIER, b, 5>
<SEMICOLON, ;, 5>
<PRINT, papoy, 6>
<LPAREN, (, 6>
<STRING_LITERAL, "Sum is: %d", 6>
<COMMA, ,, 6>
<IDENTIFIER, sum, 6>
<RPAREN, ), 6>
<SEMICOLON, ;, 6>
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

## 🔧 Technical Details

### Token Format
```
<TOKEN_TYPE, lexeme, line_number>
```

### Flex File Structure
```
┌─────────────────────────────┐
│  %{ Declarations %}         │  ← Headers, variables, functions
├─────────────────────────────┤
│  Definitions                │  ← Regex macros, options
├─────────────────────────────┤
│  %%                         │
│  Pattern-Action Rules       │  ← Core lexer logic
│  %%                         │
├─────────────────────────────┤
│  User Code                  │  ← main(), helpers
└─────────────────────────────┘
```

### Key Flex Variables
- `yytext` - Matched text string
- `yyleng` - Length of matched text
- `yylineno` - Current line number
- `yyin` - Input file pointer
- `yyout` - Output file pointer

### Pattern Matching Strategy
1. Multi-word keywords first (`loooong banana`, `bababa kaba`)
2. Single keywords (`banana`, `kaba`)
3. Multi-char operators (`:=`, `:->`, `<=`, `>=`)
4. Single-char operators (`<`, `>`)
5. Floats before integers
6. Identifiers last (most general)
7. Error catch-all (`.`)

---

## 📊 Statistics

- **Token Types**: 71
- **Token Categories**: 14
- **Flex Specification**: ~200 lines
- **Generated C Code**: ~61 KB
- **Time Complexity**: O(n) linear
- **Keywords**: 45+
- **Operators**: 20+
- **Math Functions**: 13

---

## 🎓 For Academic Use

### Lab Objectives Met
✅ Understand lexical analysis  
✅ Learn regular expressions  
✅ Use Flex tool  
✅ Design custom language  
✅ Implement token recognition  
✅ Handle comments and errors  
✅ Prepare for parser integration  

### Suitable For
- Compiler Design lab projects
- Programming Language courses
- Theory of Computation demonstrations
- Automata theory applications

### Viva Preparation
Refer to [VIVA_QUICK_REFERENCE.md](docs/VIVA_QUICK_REFERENCE.md) for:
- Quick answers to common questions
- Key technical concepts
- Demonstration checklist
- Important statistics

---

## 🔮 Future Enhancements

### Phase 2: Parser (Planned)
- [ ] Bison grammar specification
- [ ] Syntax analysis
- [ ] Parse tree generation
- [ ] AST construction

### Phase 3: Code Generation (Planned)
- [ ] MinionLang → C translation
- [ ] Semantic analysis
- [ ] Symbol table management
- [ ] Code optimization

---

## 🐛 Error Handling

The lexer detects and reports:
- **Unrecognized characters** with line numbers
- **Unterminated strings**
- **Invalid tokens**

**Example:**
```
Input: banana x @= 10;
Error: LEXICAL ERROR at line 1: Unrecognized character '@'
```

---

## 🛠️ Requirements

- **Flex**: 2.5.4+ (included in `GnuWin32/`)
- **GCC**: MinGW or similar C compiler
- **OS**: Windows (scripts use batch files)

For Linux/Mac, modify scripts to use bash:
```bash
flex src/minionlang.l
gcc -o minionlexer lex.yy.c
./minionlexer tests/sample.minion tests/sample.tokens
```

---

## 📝 Usage

```bash
minionlexer.exe <input_file> [output_file]

Arguments:
  input_file   : MinionLang source file (.minion)
  output_file  : Token output file (optional, defaults to stdout)

Examples:
  .\minionlexer.exe code.minion tokens.txt    # Output to file
  .\minionlexer.exe code.minion               # Output to console
```

---

## 🤝 Acknowledgments

- **Course**: Compiler Design Lab (CSE)
- **Tool**: Flex by Vern Paxson
- **Inspiration**: Dragon Book (Compilers: Principles, Techniques, and Tools)
- **Theme**: Despicable Me / Minions franchise

---

## 📚 References

1. **Flex Manual**: https://github.com/westes/flex
2. **Compilers: Principles, Techniques, and Tools** - Aho, Sethi, Ullman
3. **Modern Compiler Implementation in C** - Andrew Appel
4. **Flex & Bison** - John Levine (O'Reilly)

---

## 📄 License

This is an academic project for educational purposes.

---

## 📧 Contact

For questions about this project, refer to the documentation or consult your course instructor.

---

**Made with 🍌 for Compiler Design Lab**

*"Bello! Welcome to MinionLang Compilation!"*
