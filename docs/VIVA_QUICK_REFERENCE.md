# MinionLang Lexer - Quick Reference Guide for Viva

## Quick Command Reference

```bash
# Build lexer
.\build.bat

# Run lexer
.\minionlexer.exe <input.minion> [output.tokens]

# Example
.\minionlexer.exe tests\sample.minion tests\sample.tokens

# View output
type tests\sample.tokens
```

---

## Key Concepts (30-Second Answers)

### What is lexical analysis?
First phase of compilation that converts character stream into token stream. Removes whitespace, handles comments, identifies keywords/operators/literals.

### What is Flex?
Tool that generates lexical analyzers from pattern specifications. Takes `.l` file → generates C code → compiles to executable lexer.

### Why MinionLang?
Educational C-based language with Minion-themed keywords. Demonstrates compiler design principles while maintaining formal language structure.

### How does pattern matching work?
Flex converts regex patterns to DFA (Deterministic Finite Automaton). Uses longest-match rule: always chooses longest possible token.

### What is a token?
Smallest meaningful unit in a program. Format: `<TOKEN_TYPE, lexeme, line_number>`

---

## Critical Technical Points

### Pattern Ordering Rules:
1. Multi-word keywords FIRST (`loooong banana`, `bababa kaba`)
2. Single keywords (`banana`, `kaba`)
3. Multi-char operators (`:=`, `:->`, `<=`, `>=`)
4. Single-char operators (`<`, `>`, `:`)
5. Floats BEFORE integers (contains decimal point)
6. Identifiers LAST (most general)

### Important Flex Variables:
- `yytext` - Matched text (string)
- `yyleng` - Length of matched text
- `yylineno` - Current line number
- `yyin` - Input file pointer
- `yyout` - Output file pointer

### Token Categories (71 total):
- Preprocessor (2)
- Data types (7)
- Control flow (12)
- Functions (4)
- Math functions (13)
- Operators (20)
- Punctuation (9)
- Literals (4)

---

## Common Viva Questions

### Q: Difference between lexer and parser?
**A:** Lexer reads characters → produces tokens. Parser reads tokens → checks syntax → builds parse tree.

### Q: Why Flex over manual lexer?
**A:** Faster development, proven correctness, optimized DFA, maintainable patterns, integrates with Bison.

### Q: How handle keywords vs identifiers?
**A:** Keywords listed BEFORE identifier pattern. Flex tries patterns in order.

### Q: What is `%option noyywrap`?
**A:** Tells Flex we're processing single file (no need to wrap to next file). Simplifies code.

### Q: Longest match rule example?
**A:** Input: `loooong banana` → Matches entire string as `TYPE_LONG_INT` (not `IDENTIFIER` + `TYPE_INT`)

### Q: How track line numbers?
**A:** Global variable `yylineno` incremented on each `\n`. Multi-line constructs count embedded newlines.

### Q: Error handling strategy?
**A:** Catch-all pattern `.` matches any unrecognized char. Reports error with line number, continues scanning.

### Q: Comment handling?
**A:** Single: `\$\$.*` matches until newline. Multi: `\$\*...\*\$` with nested `*` handling.

---

## Demonstration Checklist

✅ Show `.l` file structure (3 sections)  
✅ Explain pattern matching (regex → DFA)  
✅ Run build script  
✅ Show generated `lex.yy.c`  
✅ Run lexer on sample file  
✅ Display token output  
✅ Test error handling (invalid char)  
✅ Explain token format  
✅ Discuss Bison integration (future)  

---

## Full Compiler Viva Demo Order

1. Build parser: `.\build_parser.bat`
2. Run valid syntax case: `.\minionparser.exe tests\valid_syntax.minion`
3. Show syntax failure behavior: `.\minionparser.exe tests\syntax_error1.minion`
4. Show semantic failure behavior: `.\minionparser.exe tests\semantic_error10_undefined_function.minion`
5. Run semantic suite: `.\run_semantic_tests.bat`
6. Run execution proof suite: `.\run_execution_proof_tests.bat`
7. Run one-command final demo: `.\run_full_demo.bat`

---

## Key Statistics

- **71 token types** recognized
- **14 categories** of tokens
- **~200 lines** of Flex specification
- **~61KB** generated C code
- **O(n)** time complexity (linear)
- **Version:** Flex 2.5.4

---

## Sample Token Examples

```
Input: banana x := 10;

Tokens:
<TYPE_INT, banana, 1>
<IDENTIFIER, x, 1>
<OP_ASSIGN, :=, 1>
<INTEGER_LITERAL, 10, 1>
<SEMICOLON, ;, 1>
```

---

## Project Files

| File | Description |
|------|-------------|
| `src/minionlang.l` | Flex specification |
| `lex.yy.c` | Generated C code |
| `minionlexer.exe` | Compiled lexer |
| `tests/sample.minion` | Test input |
| `tests/sample.tokens` | Token output |
| `docs/PRD.md` | Requirements |
| `docs/LEXER_DOCUMENTATION.md` | Full documentation |

---

## Compiler Pipeline Position

```
[Source Code]
     ↓
[LEXER] ← Our project (Flex)
     ↓
[Tokens]
     ↓
[Parser] ← Future: Bison
     ↓
[Parse Tree]
     ↓
[Semantic Analysis]
     ↓
[Code Generation]
```

---

**Pro Tip:** Read the full documentation at `docs/LEXER_DOCUMENTATION.md` for detailed explanations!
