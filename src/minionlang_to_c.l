%{
/*
 * MinionLang to C Translator (Lexical Translation)
 * Optional high-marks feature: source-to-source translation.
 */

#include <stdio.h>
#include <stdlib.h>

int yylineno = 1;
int for_header_pending = 0;
int for_paren_depth = 0;
int skip_return_type_once = 0;

static void out(const char* s) {
    fputs(s, yyout);
}

%}

%option noyywrap
%x FORHDR

DIGIT       [0-9]
LETTER      [A-Za-z]
ALPHANUM    [A-Za-z0-9_]
WHITESPACE  [ \t]+
NEWLINE     \n
EXP         [eE][+-]?{DIGIT}+
FLOAT_A     {DIGIT}+\.{DIGIT}+({EXP})?
FLOAT_B     {DIGIT}+\.({EXP})?
FLOAT_C     \.{DIGIT}+({EXP})?

%%

"$$"[^\n]*                      {
                                    out("//");
                                    out(yytext + 2);
                                  }

"$*"([^*]|\*+[^$])*"*$"         {
                                    int i;
                                    for (i = 0; yytext[i] != '\0'; i++) {
                                        if (yytext[i] == '\n') yylineno++;
                                    }
                                    putc('/', yyout);
                                    putc('*', yyout);
                                    fwrite(yytext + 2, 1, yyleng - 4, yyout);
                                    putc('*', yyout);
                                    putc('/', yyout);
                                  }

"#bringy"                        { out("#include"); }
"#setty"                         { out("#define"); }

"loooong"[ \t]+"banana"          {
                                    if (skip_return_type_once) {
                                        skip_return_type_once = 0;
                                    } else {
                                        out("long int");
                                    }
                                  }
"bababa"[ \t]+"kaba"             { out("else if"); }

"banana"                         { if (skip_return_type_once) skip_return_type_once = 0; else out("int"); }
"smoothie"                       { if (skip_return_type_once) skip_return_type_once = 0; else out("float"); }
"megasmoot"                      { if (skip_return_type_once) skip_return_type_once = 0; else out("double"); }
"chibi"                          { if (skip_return_type_once) skip_return_type_once = 0; else out("char"); }
"yesno"                          { if (skip_return_type_once) skip_return_type_once = 0; else out("bool"); }
"nada"                           { if (skip_return_type_once) skip_return_type_once = 0; else out("void"); }

"kaba"                           { out("if"); }
"bababa"                         { out("else"); }
"againa"                         { out("for"); for_header_pending = 1; }
"loopa"                          { out("while"); }
"dodo"                           { out("do"); }
"choosey"                        { out("switch"); }
"optiona"                        { out("case"); }
"otherwise"                      { out("default"); }
"stoppo"                         { out("break"); }
"gooo"                           { out("continue"); }
"backa"                          { out("return"); }

"starta"                         { out("main"); }
"papoy"                          { out("printf"); }
"takey"                          { out("scanf"); }

"power"                          { out("pow"); }
"root"                           { out("sqrt"); }
"flr"                            { out("floor"); }
"ceil"                           { out("ceil"); }
"abs"                            { out("abs"); }
"loggy"                          { out("log"); }
"sine"                           { out("sin"); }
"cosine"                         { out("cos"); }
"tan"                            { out("tan"); }
"asine"                          { out("asin"); }
"acosine"                        { out("acos"); }
"atan"                           { out("atan"); }
"primey"                         { out("isPrime"); }

"add"                            { out("+"); }
"minus"                          { out("-"); }
"mul"                            { out("*"); }
"divide"                         { out("/"); }
"mod"                            { out("%"); }

"AND"                            { out("&&"); }
"OR"                             { out("||"); }
"NOT"                            { out("!"); }
"XOR"                            { out("^"); }

":->"                            { skip_return_type_once = 1; }
":="                             { out("="); }

"<="                             { out("<="); }
">="                             { out(">="); }
"=="                             { out("=="); }
"!="                             { out("!="); }
"<"                              { out("<"); }
">"                              { out(">"); }

";"                              { out(";"); }
":"                              { out(":"); }
","                              { out(","); }
<INITIAL>"("                     {
                                                                        out("(");
                                                                        if (for_header_pending) {
                                                                                for_header_pending = 0;
                                                                                for_paren_depth = 1;
                                                                                BEGIN(FORHDR);
                                                                        }
                                                                    }
<INITIAL>")"                     { out(")"); }
"{"                              { out("{"); }
"}"                              { out("}"); }
"["                              { out("["); }
"]"                              { out("]"); }

{FLOAT_A}|{FLOAT_B}|{FLOAT_C}     { out(yytext); }
{DIGIT}+                         { out(yytext); }
\"([^\"\n\\]|\\.)*\"             { out(yytext); }
\'([^\'\n\\]|\\.)\'              { out(yytext); }

({LETTER}|"_"){ALPHANUM}*        { out(yytext); }

{WHITESPACE}                     { out(yytext); }
{NEWLINE}                        { yylineno++; out("\n"); }

.                                { out(yytext); }

<FORHDR>"("                      { out("("); for_paren_depth++; }
<FORHDR>")"                      {
                                                                        out(")");
                                                                        for_paren_depth--;
                                                                        if (for_paren_depth <= 0) {
                                                                                BEGIN(INITIAL);
                                                                        }
                                                                    }
<FORHDR>":"                      { out(";"); }
<FORHDR>"loooong"[ \t]+"banana"  { out("long int"); }
<FORHDR>"banana"                  { out("int"); }
<FORHDR>"smoothie"                { out("float"); }
<FORHDR>"megasmoot"               { out("double"); }
<FORHDR>"chibi"                   { out("char"); }
<FORHDR>"yesno"                   { out("bool"); }
<FORHDR>":="                      { out("="); }
<FORHDR>"add"                     { out("+"); }
<FORHDR>"minus"                   { out("-"); }
<FORHDR>"mul"                     { out("*"); }
<FORHDR>"divide"                  { out("/"); }
<FORHDR>"mod"                     { out("%"); }
<FORHDR>"AND"                     { out("&&"); }
<FORHDR>"OR"                      { out("||"); }
<FORHDR>"NOT"                     { out("!"); }
<FORHDR>"XOR"                     { out("^"); }
<FORHDR>"<="                      { out("<="); }
<FORHDR>">="                      { out(">="); }
<FORHDR>"=="                      { out("=="); }
<FORHDR>"!="                      { out("!="); }
<FORHDR>"<"                       { out("<"); }
<FORHDR>">"                       { out(">"); }
<FORHDR>{FLOAT_A}|{FLOAT_B}|{FLOAT_C} { out(yytext); }
<FORHDR>{DIGIT}+                  { out(yytext); }
<FORHDR>({LETTER}|"_"){ALPHANUM}* { out(yytext); }
<FORHDR>{WHITESPACE}              { out(yytext); }
<FORHDR>{NEWLINE}                { yylineno++; out("\n"); }
<FORHDR>.                        { out(yytext); }

%%

int main(int argc, char* argv[]) {
    FILE* in;
    FILE* out_file;

    if (argc < 2) {
        fprintf(stderr, "Usage: %s <input.minion> [output.c]\n", argv[0]);
        return 1;
    }

    in = fopen(argv[1], "r");
    if (!in) {
        fprintf(stderr, "Error: cannot open input file %s\n", argv[1]);
        return 1;
    }
    yyin = in;

    if (argc >= 3) {
        out_file = fopen(argv[2], "w");
        if (!out_file) {
            fprintf(stderr, "Error: cannot open output file %s\n", argv[2]);
            fclose(in);
            return 1;
        }
        yyout = out_file;
    } else {
        yyout = stdout;
    }

    yylex();

    fclose(in);
    if (argc >= 3) {
        fclose(yyout);
    }

    return 0;
}
