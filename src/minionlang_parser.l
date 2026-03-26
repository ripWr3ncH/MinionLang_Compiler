%{
/*
 * MinionLang Lexical Analyzer (Bison Integration Version)
 * Returns tokens to the Bison parser
 * 
 * Course: Compiler Design Lab (CSE)
 * Date: February 2026
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "minionlang.tab.h"

/* Line number tracking */
int yylineno = 1;

/* Function to print error messages */
void lexical_error(const char* message, const char* lexeme) {
    fprintf(stderr, "LEXICAL ERROR at line %d: %s '%s'\n", yylineno, message, lexeme);
}

/* Count newlines in multi-line constructs */
void count_newlines(const char* text) {
    for (int i = 0; text[i] != '\0'; i++) {
        if (text[i] == '\n') {
            yylineno++;
        }
    }
}

static char decode_char_literal(const char* text) {
    if (text[1] != '\\') {
        return text[1];
    }

    switch (text[2]) {
        case 'n': return '\n';
        case 't': return '\t';
        case 'r': return '\r';
        case '\\': return '\\';
        case '\'': return '\'';
        case '"': return '"';
        case '0': return '\0';
        default: return text[2];
    }
}

%}

/* Flex options */
%option noyywrap

/* Definitions */
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

    /* ============================================= */
    /* COMMENTS - Ignored by parser                 */
    /* ============================================= */

"\$\$".*                        { /* Single-line comment - ignore */ }

"\$\*"([^*]|\*+[^\$])*"\*\$"    { 
                                    count_newlines(yytext);
                                    /* Multi-line comment - ignore */
                                }

    /* ============================================= */
    /* PREPROCESSOR DIRECTIVES                      */
    /* ============================================= */

"#bringy"                       { return INCLUDE; }
"#setty"                        { return DEFINE; }

"<"[A-Za-z0-9_\.]+">"{WHITESPACE}?  { return HEADER_FILE; }

    /* ============================================= */
    /* MULTI-WORD KEYWORDS (Must come before single)*/
    /* ============================================= */

"loooong"[ \t]+"banana"         { return TYPE_LONG_INT; }
"bababa"[ \t]+"kaba"            { return ELSE_IF; }

    /* ============================================= */
    /* DATA TYPE KEYWORDS                           */
    /* ============================================= */

"banana"                        { return TYPE_INT; }
"smoothie"                      { return TYPE_FLOAT; }
"megasmoot"                     { return TYPE_DOUBLE; }
"chibi"                         { return TYPE_CHAR; }
"yesno"                         { return TYPE_BOOL; }
"nada"                          { return TYPE_VOID; }

    /* ============================================= */
    /* CONTROL FLOW KEYWORDS                        */
    /* ============================================= */

"kaba"                          { return IF; }
"bababa"                        { return ELSE; }
"againa"                        { return FOR; }
"loopa"                         { return WHILE; }
"dodo"                          { return DO; }
"choosey"                       { return SWITCH; }
"optiona"                       { return CASE; }
"otherwise"                     { return DEFAULT; }
"stoppo"                        { return BREAK; }
"gooo"                          { return CONTINUE; }
"backa"                         { return RETURN; }

    /* ============================================= */
    /* FUNCTION KEYWORDS                            */
    /* ============================================= */

"starta"                        { return MAIN; }
"papoy"                         { return PRINT; }
"takey"                         { return SCAN; }

    /* ============================================= */
    /* MATH FUNCTION KEYWORDS                       */
    /* ============================================= */

"power"                         { return FUNC_POWER; }
"root"                          { return FUNC_SQRT; }
"flr"                           { return FUNC_FLOOR; }
"ceil"                          { return FUNC_CEIL; }
"abs"                           { return FUNC_ABS; }
"loggy"                         { return FUNC_LOG; }
"sine"                          { return FUNC_SIN; }
"cosine"                        { return FUNC_COS; }
"tan"                           { return FUNC_TAN; }
"asine"                         { return FUNC_ASIN; }
"acosine"                       { return FUNC_ACOS; }
"atan"                          { return FUNC_ATAN; }
"primey"                        { return FUNC_PRIME; }

    /* ============================================= */
    /* ARITHMETIC OPERATORS (word-based)            */
    /* ============================================= */

"add"                           { return OP_ADD; }
"minus"                         { return OP_SUB; }
"mul"                           { return OP_MUL; }
"divide"                        { return OP_DIV; }
"mod"                           { return OP_MOD; }
"+"                             { return OP_ADD; }
"-"                             { return OP_SUB; }
"*"                             { return OP_MUL; }
"/"                             { return OP_DIV; }
"%"                             { return OP_MOD; }

    /* ============================================= */
    /* LOGICAL OPERATORS                            */
    /* ============================================= */

"AND"                           { return OP_AND; }
"OR"                            { return OP_OR; }
"NOT"                           { return OP_NOT; }
"XOR"                           { return OP_XOR; }
"&&"                            { return OP_AND; }
"||"                            { return OP_OR; }
"!"                             { return OP_NOT; }
"^"                             { return OP_XOR; }

    /* ============================================= */
    /* MULTI-CHARACTER OPERATORS                    */
    /* ============================================= */

":->"                           { return RETURN_TYPE_IND; }
":="                            { return OP_ASSIGN; }
"<="                            { return OP_LE; }
">="                            { return OP_GE; }
"=="                            { return OP_EQ; }
"!="                            { return OP_NE; }

    /* ============================================= */
    /* SINGLE-CHARACTER RELATIONAL OPERATORS        */
    /* ============================================= */

"<"                             { return OP_LT; }
">"                             { return OP_GT; }

    /* ============================================= */
    /* PUNCTUATION & DELIMITERS                     */
    /* ============================================= */

";"                             { return SEMICOLON; }
":"                             { return COLON; }
","                             { return COMMA; }
"("                             { return LPAREN; }
")"                             { return RPAREN; }
"{"                             { return LBRACE; }
"}"                             { return RBRACE; }
"["                             { return LBRACKET; }
"]"                             { return RBRACKET; }

    /* ============================================= */
    /* LITERALS                                     */
    /* ============================================= */

{FLOAT_A}|{FLOAT_B}|{FLOAT_C}    { yylval.fval = strtod(yytext, NULL); return FLOAT_LITERAL; }
{DIGIT}+                        { yylval.ival = (int)strtol(yytext, NULL, 10); return INTEGER_LITERAL; }
\"([^\"\n\\]|\\.)*\"            { yylval.sval = strdup(yytext); return STRING_LITERAL; }
\'([^\'\n\\]|\\.)\'             { yylval.cval = decode_char_literal(yytext); return CHAR_LITERAL; }

    /* ============================================= */
    /* IDENTIFIERS                                  */
    /* ============================================= */

({LETTER}|"_"){ALPHANUM}*       { yylval.sval = strdup(yytext); return IDENTIFIER; }

    /* ============================================= */
    /* WHITESPACE & NEWLINES                        */
    /* ============================================= */

{WHITESPACE}                    { /* Ignore whitespace */ }
{NEWLINE}                       { yylineno++; }

    /* ============================================= */
    /* ERROR HANDLING                               */
    /* ============================================= */

\"([^\"\n\\]|\\.)*{NEWLINE}     { 
                                    lexical_error("Unterminated string literal", ""); 
                                    yylineno++;
                                }

"="                             { lexical_error("Use ':=' for assignment, not", yytext); }

.                               { lexical_error("Unrecognized character", yytext); }

%%
