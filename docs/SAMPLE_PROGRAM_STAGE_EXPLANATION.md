# Sample Program Stage-by-Stage Explanation

This document explains your exact MinionLang sample from the first character read to the final output.

## 1) Source Program

```minionlang
$$ MinionLang playground sample
#bringy <stdio.h>

banana square(banana n) {
    backa n mul n;
}

banana starta() {
    banana a := 5;
    banana b := 2 add 3 mul 4;

    kaba (a > 3) {
        papoy("a is %d", a);
    } bababa {
        papoy("small a");
    }

    banana s := square(a);
    papoy("square is %d", s);
    papoy("b = %d", b);

    backa 0;
}
```

Expected final output:

```text
a is 5
square is 25
b = 14
```

## 2) Stage 0: Input Read

- Your compiler/program receives the source file as text.
- Comments are recognized as comments and do not affect computation.
- The parser/translator sees the real code lines after comment handling.

## 3) Stage 1: Lexical Analysis (Tokenization)

Lexer file: `src/minionlang.l`

The lexer scans left-to-right and converts source text into tokens.

Examples from your code:
- `banana` -> type keyword token
- `square` -> identifier token
- `(` `)` `{` `}` `;` `,` -> punctuation tokens
- `:=` -> assignment operator token
- `add`, `mul` -> arithmetic operator tokens
- `kaba`, `bababa` -> if/else tokens
- `papoy` -> print token
- `"a is %d"` -> string literal token
- `5`, `2`, `3`, `4` -> integer literal tokens

Important detail:
- `2 add 3 mul 4` is tokenized as values/operators; precedence is handled in parsing/expression grammar stage.

## 4) Stage 2: Syntax Analysis (Parsing)

Parser grammar file: `src/minionlang.y`

The parser checks whether token order matches MinionLang grammar rules.

### Function-level parse
- Parses function definition: `banana square(banana n) { ... }`
- Parses main-like function: `banana starta() { ... }`

### Statement-level parse
Inside `starta()` it parses:
1. Declaration + assignment: `banana a := 5;`
2. Declaration + expression assignment: `banana b := 2 add 3 mul 4;`
3. If-else statement with condition `a > 3`
4. Function call assignment: `banana s := square(a);`
5. Print statements
6. Return statement

If grammar is correct, parse succeeds.

## 5) Stage 3: Semantic Checks

Semantic checks are embedded in parser actions (`src/minionlang.y`).

For this sample, key checks that pass:
- `a`, `b`, `s` are declared before use
- `square` function exists before call
- function argument count/type for `square(a)` is valid
- assignment types are compatible
- return statements are valid for function types

No semantic error means execution/translation can continue.

## 6) Stage 4A: Website Execution Path (Playground)

Website runtime files:
- `web/playground.html`
- `web/app.js`

Flow in browser:
1. Tokenize source for token panel.
2. Static checks for common syntax issues.
3. Transpile MinionLang to JavaScript:
   - `kaba` -> `if`
   - `bababa` -> `else`
   - `add` -> `+`
   - `mul` -> `*`
   - `:=` -> `=`
   - `backa` -> `return`
   - `papoy` -> internal print helper
4. Execute generated JS in sandboxed function.
5. Collect printed lines and show Program Output panel.

So website output is produced by transpiled JS execution.

## 7) Stage 4B: Native Execution-Proof Path (Translator -> C -> EXE)

Native path command family:
- `minion2c.exe input.minion output.c`
- `gcc output.c -o output.exe`
- run `output.exe`

Flow:
1. MinionLang is translated into C source.
2. C source is compiled to native executable.
3. Executable runs and prints final output.

This path is closest to classic compiled language execution.

## 8) Stage 5: Runtime Evaluation (Why output is exactly this)

### Line 1: `a is 5`
- `a := 5`
- condition `a > 3` => `5 > 3` is true
- true branch executes: `papoy("a is %d", a)`
- prints: `a is 5`

### Line 2: `square is 25`
- call `square(a)` with `a = 5`
- in `square`, return `n mul n` => `5 * 5 = 25`
- so `s = 25`
- print `papoy("square is %d", s)` => `square is 25`

### Line 3: `b = 14`
- expression: `2 add 3 mul 4`
- precedence: multiplication before addition
- `3 mul 4 = 12`
- `2 add 12 = 14`
- print `papoy("b = %d", b)` => `b = 14`

## 9) Final End-to-End Summary

Complete conceptual pipeline:

1. Source text read
2. Lexer tokenizes (`.l` rules)
3. Parser validates grammar (`.y` rules)
4. Semantic checks validate meaning
5. Execution path:
   - Website: transpile to JS and execute in browser
   - Native: translate to C, compile, run EXE
6. Print output lines

Final output for this sample:

```text
a is 5
square is 25
b = 14
```

## 10) Wrong-Code Case: Why Website Shows "execution skipped due to syntax issues"

Wrong source:

```minionlang
$$ MinionLang playground sample
#bringy <stdio.h>

banana square(banana n) {
    backa n mul n;
}

banana starta() {
    banan a := 5;
    banana b := 2 add 3 mul 4;

    kaba (a > 3) {
        papoy("a is %d", a);
    } bababa {
        papoy("small a"
    }

    banana s := square(a);
    papoy("square is %d", s);
    papoy("b = %d", b);

    backa 0;
}
```

Website output:
- Program Output: `(execution skipped due to syntax issues)`
- Diagnostics: syntax errors and hint lines
- Tokens: still printed

### 10.1 Why tokens are still shown

Tokenization happens before execution. The website tokenizer can still split most text into valid tokens even when the program is not syntactically complete.

So this is normal:
- Tokens panel is populated
- Execution is blocked later by syntax checks

### 10.2 Exact root causes in this code

Root cause 1 (line 9):
- `banan a := 5;`
- `banan` is not a valid type keyword (`banana` is correct)
- That is why you get:
  - `SYNTAX ERROR line 9: unknown type keyword 'banan' before 'a'.`
  - `HINT line 9: did you mean 'banana'?`

Root cause 2 (line 15):
- `papoy("small a"`
- Missing `)` and `;`
- This breaks parenthesis/bracket balance analysis.

### 10.3 Why multiple bracket errors appear

After one missing `)` in a nested block, later braces no longer align perfectly in a simple stack-based checker.

So one small syntax issue can trigger several follow-up messages like:
- unmatched `}`
- missing closing pair for `(`
- missing closing pair for `{`

These are cascading diagnostics, not separate independent root bugs.

### 10.4 Why execution is skipped

Your website logic does this intentionally:
1. tokenize
2. static syntax analysis
3. if any `SYNTAX ERROR` exists, stop before runtime execution

That prevents confusing JavaScript runtime crashes and gives cleaner compiler-style feedback.

### 10.5 Corrected version of the wrong lines

```minionlang
banana a := 5;
papoy("small a");
```

After fixing those lines, website execution proceeds and the expected output returns.
