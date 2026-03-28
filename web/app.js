const sample = `$$ MinionLang playground sample
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
}`;

const editor = document.getElementById("editor");
const outputEl = document.getElementById("output");
const diagnosticsEl = document.getElementById("diagnostics");
const tokensEl = document.getElementById("tokens");
const transpiledEl = document.getElementById("transpiled");

const keywords = new Set([
  "banana", "smoothie", "chibi", "bool", "void",
  "kaba", "bababa", "againa", "loopa", "do", "backa",
  "papoy", "takey", "starta", "AND", "OR", "NOT",
  "add", "minus", "mul", "div", "mod"
]);

const typeWords = new Set(["banana", "smoothie", "chibi", "bool", "void"]);

function setPanel(el, value) {
  el.textContent = value || "";
}

function stripComments(src) {
  const noBlock = src.replace(/\$\*[\s\S]*?\*\$/g, "");
  return noBlock
    .split("\n")
    .map((line) => {
      const i = line.indexOf("$$");
      return i >= 0 ? line.slice(0, i) : line;
    })
    .join("\n");
}

function tokenize(src) {
  const clean = stripComments(src);
  const rx = /<[^>\n]+>|"(?:\\.|[^"\\])*"|'(?:\\.|[^'\\])*'|:=|:->|<=|>=|==|!=|&&|\|\||#bringy|#setty|[(){}\[\],;:]|[+\-*/%<>]|[A-Za-z_][A-Za-z0-9_]*|\d+\.\d+|\d+|\S/g;
  const tokens = [];
  const diagnostics = [];

  let m;
  while ((m = rx.exec(clean)) !== null) {
    const text = m[0];
    const line = clean.slice(0, m.index).split("\n").length;

    let type = "UNKNOWN";
    if (/^<[^>\n]+>$/.test(text)) type = "HEADER_FILE";
    if (/^"/.test(text)) type = "STRING_LITERAL";
    else if (/^'/.test(text)) type = "CHAR_LITERAL";
    else if (/^\d+\.\d+$/.test(text)) type = "FLOAT_LITERAL";
    else if (/^\d+$/.test(text)) type = "INTEGER_LITERAL";
    else if (text === "#bringy") type = "INCLUDE";
    else if (text === "#setty") type = "DEFINE";
    else if ([":=",":->","<=",">=","==","!=","&&","||","+","-","*","/","%","<",">",":",";",",","(",")","{","}","[","]"].includes(text)) type = "SYMBOL";
    else if (/^[A-Za-z_][A-Za-z0-9_]*$/.test(text) && keywords.has(text)) type = "KEYWORD";
    else if (/^[A-Za-z_][A-Za-z0-9_]*$/.test(text)) type = "IDENTIFIER";

    if (type === "UNKNOWN") {
      diagnostics.push(`LEXICAL WARNING line ${line}: unrecognized token '${text}'`);
    }

    tokens.push({ type, text, line });
  }

  return { tokens, diagnostics };
}

function convertParams(raw) {
  if (!raw.trim()) return "";
  return raw
    .split(",")
    .map((p) => p.trim().replace(/\b(?:banana|smoothie|chibi|bool|void)\b\s+/g, ""))
    .join(", ");
}

function transpileToJs(src) {
  let code = stripComments(src);

  code = code.replace(/^\s*#bringy.*$/gm, "");
  code = code.replace(/^\s*#setty\s+([A-Za-z_][A-Za-z0-9_]*)\s+(.+)$/gm, "const $1 = $2;");

  code = code.replace(/\bbababa\s+kaba\b/g, "else if");
  code = code.replace(/\bkaba\b/g, "if");
  code = code.replace(/\bbababa\b/g, "else");
  code = code.replace(/\bloopa\b/g, "while");
  code = code.replace(/\bbacka\b/g, "return");
  code = code.replace(/\bpapoy\b/g, "__print");
  code = code.replace(/\btakey\b/g, "__scan");

  code = code.replace(/\bagaina\s*\(([^)]*)\)/g, (_, inner) => `for(${inner.replace(/:/g, ";")})`);

  code = code.replace(/\badd\b/g, "+");
  code = code.replace(/\bminus\b/g, "-");
  code = code.replace(/\bmul\b/g, "*");
  code = code.replace(/\bdiv\b/g, "/");
  code = code.replace(/\bmod\b/g, "%");
  code = code.replace(/\bAND\b/g, "&&");
  code = code.replace(/\bOR\b/g, "||");
  code = code.replace(/\bNOT\b/g, "!");
  code = code.replace(/:=/g, "=");

  code = code.replace(/\b(?:banana|smoothie|chibi|bool|void)\s+([A-Za-z_][A-Za-z0-9_]*)\s*\(([^)]*)\)\s*\{/g,
    (_, name, params) => `function ${name}(${convertParams(params)}) {`
  );

  code = code.replace(/\b(?:banana|smoothie|chibi|bool)\s+([A-Za-z_][A-Za-z0-9_]*)(?!\s*\()/g, "let $1");

  code = code.replace(/\blet\s+starta\s*\(/g, "function starta(");

  return code;
}

function runProgram(src) {
  const diagnostics = [];
  const { tokens, diagnostics: lexicalDiag } = tokenize(src);
  diagnostics.push(...lexicalDiag);

  const js = transpileToJs(src);
  setPanel(transpiledEl, js);

  const lines = tokens.map((t) => `<${t.type}, ${t.text}, ${t.line}>`);
  setPanel(tokensEl, lines.join("\n"));

  try {
    const runtime = `
      const __out = [];
      function __fmt(fmt, vals) {
        let i = 0;
        return String(fmt).replace(/%[dfs]/g, () => String(vals[i++] ?? ""));
      }
      function __print(...args) {
        if (args.length === 0) {
          __out.push("");
          return;
        }
        if (typeof args[0] === "string" && /%[dfs]/.test(args[0])) {
          __out.push(__fmt(args[0], args.slice(1)));
        } else {
          __out.push(args.map((x) => String(x)).join(" "));
        }
      }
      function __scan() {
        throw new Error("takey/scan is not supported in browser playground");
      }
      ${js}
      if (typeof starta === "function") {
        starta();
      } else {
        __out.push("No starta() found. Define main as banana starta() { ... }");
      }
      return __out.join("\\n");
    `;

    const out = new Function(runtime)();
    setPanel(outputEl, out || "(no output)");

    if (diagnostics.length === 0) {
      diagnostics.push("OK: tokenization and execution completed.");
    }
  } catch (err) {
    diagnostics.push(`RUNTIME ERROR: ${err.message}`);
    setPanel(outputEl, "(execution failed)");
  }

  setPanel(diagnosticsEl, diagnostics.join("\n"));
}

function showTokensOnly(src) {
  const { tokens, diagnostics } = tokenize(src);
  setPanel(tokensEl, tokens.map((t) => `<${t.type}, ${t.text}, ${t.line}>`).join("\n"));
  setPanel(diagnosticsEl, diagnostics.length ? diagnostics.join("\n") : "OK: tokenization completed.");
}

document.getElementById("btn-example").addEventListener("click", () => {
  editor.value = sample;
  runProgram(editor.value);
});

document.getElementById("btn-tokenize").addEventListener("click", () => {
  showTokensOnly(editor.value);
});

document.getElementById("btn-run").addEventListener("click", () => {
  runProgram(editor.value);
});

document.getElementById("btn-clear").addEventListener("click", () => {
  editor.value = "";
  setPanel(outputEl, "");
  setPanel(diagnosticsEl, "");
  setPanel(tokensEl, "");
  setPanel(transpiledEl, "");
});

editor.value = sample;
runProgram(sample);
