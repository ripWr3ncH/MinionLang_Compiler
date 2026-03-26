%{
/*
 * MinionLang Lexical Analyzer
 * A Flex-based lexer for the MinionLang programming language
 * 
 * Course: Compiler Design Lab (CSE)
 * Date: February 2026
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Line number tracking */
int yylineno = 1;

/* Function to print tokens in the required format */
void print_token(const char* token_type, const char* lexeme) {
    fprintf(yyout, "<%s, %s, %d>\n", token_type, lexeme, yylineno);
}

/* Function to print error messages */
void print_error(const char* message, const char* lexeme) {
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

%}

/* Flex options */
%option noyywrap

/* Definitions */
DIGIT       [0-9]
LETTER      [A-Za-z]
ALPHANUM    [A-Za-z0-9_]
WHITESPACE  [ \t]+
NEWLINE     \n

%%

    /* ============================================= */
    /* COMMENTS - Must be handled first             */
    /* ============================================= */

    /* Single-line comment: $$ ... */
"\$\$".*                        { print_token("COMMENT_SINGLE", yytext); }

    /* Multi-line comment: $* ... *$ */
"\$\*"([^*]|\*+[^\$])*"\*\$"    { 
                                    int start_line = yylineno;
                                    count_newlines(yytext);
                                    fprintf(yyout, "<COMMENT_MULTI, (multiline comment), %d>\n", start_line);
                                }

    /* ============================================= */
    /* PREPROCESSOR DIRECTIVES                      */
    /* ============================================= */

"#bringy"                       { print_token("INCLUDE", yytext); }
"#setty"                        { print_token("DEFINE", yytext); }

    /* Header file: <filename.h> */
"<"[A-Za-z0-9_\.]+">"{WHITESPACE}?  { print_token("HEADER_FILE", yytext); }

    /* ============================================= */
    /* MULTI-WORD KEYWORDS (Must come before single)*/
    /* ============================================= */

    /* loooong banana -> long int */
"loooong"[ \t]+"banana"         { print_token("TYPE_LONG_INT", yytext); }

    /* bababa kaba -> else if */
"bababa"[ \t]+"kaba"            { print_token("ELSE_IF", yytext); }

    /* ============================================= */
    /* DATA TYPE KEYWORDS                           */
    /* ============================================= */

"banana"                        { print_token("TYPE_INT", yytext); }
"smoothie"                      { print_token("TYPE_FLOAT", yytext); }
"megasmoot"                     { print_token("TYPE_DOUBLE", yytext); }
"chibi"                         { print_token("TYPE_CHAR", yytext); }
"yesno"                         { print_token("TYPE_BOOL", yytext); }
"nada"                          { print_token("TYPE_VOID", yytext); }

    /* ============================================= */
    /* CONTROL FLOW KEYWORDS                        */
    /* ============================================= */

"kaba"                          { print_token("IF", yytext); }
"bababa"                        { print_token("ELSE", yytext); }
"againa"                        { print_token("FOR", yytext); }
"loopa"                         { print_token("WHILE", yytext); }
"dodo"                          { print_token("DO", yytext); }
"choosey"                       { print_token("SWITCH", yytext); }
"optiona"                       { print_token("CASE", yytext); }
"otherwise"                     { print_token("DEFAULT", yytext); }
"stoppo"                        { print_token("BREAK", yytext); }
"gooo"                          { print_token("CONTINUE", yytext); }
"backa"                         { print_token("RETURN", yytext); }

    /* ============================================= */
    /* FUNCTION KEYWORDS                            */
    /* ============================================= */

"starta"                        { print_token("MAIN", yytext); }
"papoy"                         { print_token("PRINT", yytext); }
"takey"                         { print_token("SCAN", yytext); }

    /* ============================================= */
    /* MATH FUNCTION KEYWORDS                       */
    /* ============================================= */

"power"                         { print_token("FUNC_POWER", yytext); }
"root"                          { print_token("FUNC_SQRT", yytext); }
"flr"                           { print_token("FUNC_FLOOR", yytext); }
"ceil"                          { print_token("FUNC_CEIL", yytext); }
"abs"                           { print_token("FUNC_ABS", yytext); }
"loggy"                         { print_token("FUNC_LOG", yytext); }
"sine"                          { print_token("FUNC_SIN", yytext); }
"cosine"                        { print_token("FUNC_COS", yytext); }
"tan"                           { print_token("FUNC_TAN", yytext); }
"asine"                         { print_token("FUNC_ASIN", yytext); }
"acosine"                       { print_token("FUNC_ACOS", yytext); }
"atan"                          { print_token("FUNC_ATAN", yytext); }
"primey"                        { print_token("FUNC_PRIME", yytext); }

    /* ============================================= */
    /* ARITHMETIC OPERATORS (word-based)            */
    /* ============================================= */

"add"                           { print_token("OP_ADD", yytext); }
"minus"                         { print_token("OP_SUB", yytext); }
"mul"                           { print_token("OP_MUL", yytext); }
"divide"                        { print_token("OP_DIV", yytext); }
"mod"                           { print_token("OP_MOD", yytext); }
"+"                             { print_token("OP_ADD", yytext); }
"-"                             { print_token("OP_SUB", yytext); }
"*"                             { print_token("OP_MUL", yytext); }
"/"                             { print_token("OP_DIV", yytext); }
"%"                             { print_token("OP_MOD", yytext); }

    /* ============================================= */
    /* LOGICAL OPERATORS                            */
    /* ============================================= */

"AND"                           { print_token("OP_AND", yytext); }
"OR"                            { print_token("OP_OR", yytext); }
"NOT"                           { print_token("OP_NOT", yytext); }
"XOR"                           { print_token("OP_XOR", yytext); }
"&&"                            { print_token("OP_AND", yytext); }
"||"                            { print_token("OP_OR", yytext); }
"!"                             { print_token("OP_NOT", yytext); }
"^"                             { print_token("OP_XOR", yytext); }

    /* ============================================= */
    /* MULTI-CHARACTER OPERATORS                    */
    /* ============================================= */

    /* Return type indicator :-> */
":->"                           { print_token("RETURN_TYPE_IND", yytext); }

    /* Assignment operator := */
":="                            { print_token("OP_ASSIGN", yytext); }

    /* Relational operators (multi-char first) */
"<="                            { print_token("OP_LE", yytext); }
">="                            { print_token("OP_GE", yytext); }
"=="                            { print_token("OP_EQ", yytext); }
"!="                            { print_token("OP_NE", yytext); }

    /* ============================================= */
    /* SINGLE-CHARACTER RELATIONAL OPERATORS        */
    /* ============================================= */

"<"                             { print_token("OP_LT", yytext); }
">"                             { print_token("OP_GT", yytext); }

    /* ============================================= */
    /* PUNCTUATION & DELIMITERS                     */
    /* ============================================= */

";"                             { print_token("SEMICOLON", yytext); }
":"                             { print_token("COLON", yytext); }
","                             { print_token("COMMA", yytext); }
"("                             { print_token("LPAREN", yytext); }
")"                             { print_token("RPAREN", yytext); }
"{"                             { print_token("LBRACE", yytext); }
"}"                             { print_token("RBRACE", yytext); }
"["                             { print_token("LBRACKET", yytext); }
"]"                             { print_token("RBRACKET", yytext); }

    /* ============================================= */
    /* LITERALS                                     */
    /* ============================================= */

    /* Float literal (must come before integer) */
{DIGIT}+"."{DIGIT}+             { print_token("FLOAT_LITERAL", yytext); }

    /* Integer literal */
{DIGIT}+                        { print_token("INTEGER_LITERAL", yytext); }

    /* String literal */
\"([^\"\n\\]|\\.)*\"            { print_token("STRING_LITERAL", yytext); }

    /* Character literal */
\'([^\'\n\\]|\\.)?\'            { print_token("CHAR_LITERAL", yytext); }

    /* ============================================= */
    /* IDENTIFIERS                                  */
    /* ============================================= */

    /* Identifier: starts with letter or underscore */
({LETTER}|"_"){ALPHANUM}*       { print_token("IDENTIFIER", yytext); }

    /* ============================================= */
    /* WHITESPACE & NEWLINES                        */
    /* ============================================= */

{WHITESPACE}                    { /* Ignore whitespace */ }

{NEWLINE}                       { yylineno++; }

    /* ============================================= */
    /* ERROR HANDLING                               */
    /* ============================================= */

    /* Unterminated string */
\"([^\"\n\\]|\\.)*{NEWLINE}     { 
                                    print_error("Unterminated string literal", ""); 
                                    yylineno++;
                                }

"="                             { print_error("Use ':=' for assignment, not", yytext); }

    /* Any unrecognized character */
.                               { print_error("Unrecognized character", yytext); }

%%

/* ============================================= */
/* MAIN FUNCTION                                */
/* ============================================= */

int main(int argc, char* argv[]) {
    /* Check command line arguments */
    if (argc < 2) {
        printf("Usage: %s <input_file> [output_file]\n", argv[0]);
        printf("  input_file  : MinionLang source file (.minion)\n");
        printf("  output_file : Token output file (optional, defaults to stdout)\n");
        return 1;
    }

    /* Open input file */
    FILE* input_file = fopen(argv[1], "r");
    if (!input_file) {
        fprintf(stderr, "Error: Cannot open input file '%s'\n", argv[1]);
        return 1;
    }
    yyin = input_file;

    /* Open output file if specified, otherwise use stdout */
    if (argc >= 3) {
        FILE* output_file = fopen(argv[2], "w");
        if (!output_file) {
            fprintf(stderr, "Error: Cannot open output file '%s'\n", argv[2]);
            fclose(input_file);
            return 1;
        }
        yyout = output_file;
    } else {
        yyout = stdout;
    }

    /* Print header */
    fprintf(yyout, "=====================================\n");
    fprintf(yyout, "MinionLang Lexical Analyzer Output\n");
    fprintf(yyout, "Input file: %s\n", argv[1]);
    fprintf(yyout, "=====================================\n\n");

    /* Run the lexer */
    yylex();

    /* Print summary */
    fprintf(yyout, "\n=====================================\n");
    fprintf(yyout, "Lexical analysis complete.\n");
    fprintf(yyout, "Total lines processed: %d\n", yylineno);
    fprintf(yyout, "=====================================\n");

    /* Clean up */
    fclose(input_file);
    if (argc >= 3) {
        fclose(yyout);
    }

    return 0;
}
