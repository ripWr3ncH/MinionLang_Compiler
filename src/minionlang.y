%{
/*
 * MinionLang Parser
 * A Bison-based parser for the MinionLang programming language
 *
 * Course: Compiler Design Lab (CSE)
 * Date: February 2026
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* External declarations */
extern int yylex();
extern int yylineno;
extern char* yytext;
extern FILE* yyin;

/* Error handling */
void yyerror(const char* s);

/* Basic type system */
enum {
    T_ERR = 0,
    T_INT,
    T_FLOAT,
    T_DOUBLE,
    T_LONG_INT,
    T_CHAR,
    T_BOOL,
    T_VOID,
    T_STRING
};

/* Symbol table */
#define MAX_SYMBOLS 1000
typedef struct {
    char* name;
    int type;
    int initialized;
    int is_array;
} Symbol;

Symbol symbols[MAX_SYMBOLS];
int symbol_count = 0;
int current_decl_type = T_ERR;

#define MAX_SCOPE_DEPTH 256
int scope_markers[MAX_SCOPE_DEPTH];
int scope_top = -1;

int inside_function = 0;
int current_function_return_type = T_VOID;
int current_function_has_return = 0;
char current_function_name[128] = {0};

#define MAX_FUNCTIONS 256
#define MAX_PARAMS 16
typedef struct {
    char name[128];
    int return_type;
    int param_count;
    int param_types[MAX_PARAMS];
} FunctionSymbol;

FunctionSymbol functions[MAX_FUNCTIONS];
int function_count = 0;

int current_function_param_count = 0;
int current_function_param_types[MAX_PARAMS];

int current_call_arg_count = 0;
int current_call_arg_types[MAX_PARAMS];

/* Error counters */
int syntax_errors = 0;
int semantic_errors = 0;

static const char* type_name(int type) {
    switch (type) {
        case T_INT: return "int";
        case T_FLOAT: return "float";
        case T_DOUBLE: return "double";
        case T_LONG_INT: return "long int";
        case T_CHAR: return "char";
        case T_BOOL: return "bool";
        case T_VOID: return "void";
        case T_STRING: return "string";
        default: return "invalid";
    }
}

static int is_numeric(int type) {
    return type == T_INT || type == T_FLOAT || type == T_DOUBLE ||
           type == T_LONG_INT || type == T_CHAR || type == T_BOOL;
}

static int merge_numeric_type(int left, int right) {
    if (left == T_ERR || right == T_ERR) return T_ERR;
    if (!is_numeric(left) || !is_numeric(right)) return T_ERR;

    if (left == T_DOUBLE || right == T_DOUBLE) return T_DOUBLE;
    if (left == T_FLOAT || right == T_FLOAT) return T_FLOAT;
    if (left == T_LONG_INT || right == T_LONG_INT) return T_LONG_INT;
    return T_INT;
}

static int find_symbol(const char* name) {
    int i;
    for (i = symbol_count - 1; i >= 0; i--) {
        if (strcmp(symbols[i].name, name) == 0) {
            return i;
        }
    }
    return -1;
}

static void enter_scope(void) {
    if (scope_top + 1 >= MAX_SCOPE_DEPTH) {
        fprintf(stderr, "Internal error: scope stack overflow\n");
        return;
    }
    scope_top++;
    scope_markers[scope_top] = symbol_count;
}

static void exit_scope(void) {
    int i;
    int marker;

    if (scope_top < 0) {
        return;
    }

    marker = scope_markers[scope_top];
    for (i = marker; i < symbol_count; i++) {
        free(symbols[i].name);
        symbols[i].name = NULL;
    }
    symbol_count = marker;
    scope_top--;
}

static int current_scope_start(void) {
    if (scope_top < 0) {
        return 0;
    }
    return scope_markers[scope_top];
}

static int declare_symbol(const char* name, int type, int initialized, int is_array) {
    int i;
    int start = current_scope_start();

    for (i = symbol_count - 1; i >= start; i--) {
        if (strcmp(symbols[i].name, name) == 0) {
            return -1;
        }
    }

    if (symbol_count >= MAX_SYMBOLS) {
        return -2;
    }

    symbols[symbol_count].name = strdup(name);
    symbols[symbol_count].type = type;
    symbols[symbol_count].initialized = initialized;
    symbols[symbol_count].is_array = is_array;
    symbol_count++;
    return symbol_count - 1;
}

static int is_assignment_compatible(int lhs, int rhs) {
    if (lhs == T_ERR || rhs == T_ERR) return 0;
    if (lhs == rhs) return 1;

    if (is_numeric(lhs) && is_numeric(rhs)) return 1;

    return 0;
}

static void semantic_error(const char* msg, const char* detail) {
    if (syntax_errors > 0) {
        return;
    }
    semantic_errors++;
    if (detail) {
        fprintf(stderr, "SEMANTIC ERROR at line %d: %s (%s)\n", yylineno, msg, detail);
    } else {
        fprintf(stderr, "SEMANTIC ERROR at line %d: %s\n", yylineno, msg);
    }
}

static void begin_function_context(int return_type, const char* name) {
    inside_function = 1;
    current_function_return_type = return_type;
    current_function_has_return = 0;
    strncpy(current_function_name, name, sizeof(current_function_name) - 1);
    current_function_name[sizeof(current_function_name) - 1] = '\0';
    enter_scope();
}

static void end_function_context(void) {
    if (inside_function && current_function_return_type != T_VOID && !current_function_has_return) {
        semantic_error("Non-void function must return a value", current_function_name);
    }
    exit_scope();
    inside_function = 0;
    current_function_return_type = T_VOID;
    current_function_has_return = 0;
    current_function_name[0] = '\0';
}

static int find_function(const char* name) {
    int i;
    for (i = 0; i < function_count; i++) {
        if (strcmp(functions[i].name, name) == 0) {
            return i;
        }
    }
    return -1;
}

static void register_current_function_signature(void) {
    int i;
    int idx = find_function(current_function_name);

    if (idx >= 0) {
        if (functions[idx].param_count != current_function_param_count ||
            functions[idx].return_type != current_function_return_type) {
            semantic_error("Conflicting function signature", current_function_name);
            return;
        }
        for (i = 0; i < current_function_param_count; i++) {
            if (functions[idx].param_types[i] != current_function_param_types[i]) {
                semantic_error("Conflicting function parameter types", current_function_name);
                return;
            }
        }
        return;
    }

    if (function_count >= MAX_FUNCTIONS) {
        semantic_error("Function table overflow", current_function_name);
        return;
    }

    strncpy(functions[function_count].name, current_function_name, sizeof(functions[function_count].name) - 1);
    functions[function_count].name[sizeof(functions[function_count].name) - 1] = '\0';
    functions[function_count].return_type = current_function_return_type;
    functions[function_count].param_count = current_function_param_count;
    for (i = 0; i < current_function_param_count; i++) {
        functions[function_count].param_types[i] = current_function_param_types[i];
    }
    function_count++;
}

static void reset_call_args(void) {
    current_call_arg_count = 0;
}

static void push_call_arg(int t) {
    if (current_call_arg_count < MAX_PARAMS) {
        current_call_arg_types[current_call_arg_count++] = t;
    }
}

static int validate_function_call(const char* name) {
    int i;
    int idx = find_function(name);

    if (idx < 0) {
        semantic_error("Call to undefined function", name);
        return T_ERR;
    }

    if (functions[idx].param_count != current_call_arg_count) {
        semantic_error("Function argument count mismatch", name);
        return functions[idx].return_type;
    }

    for (i = 0; i < current_call_arg_count; i++) {
        if (!is_assignment_compatible(functions[idx].param_types[i], current_call_arg_types[i])) {
            semantic_error("Function argument type mismatch", name);
            return functions[idx].return_type;
        }
    }

    return functions[idx].return_type;
}

%}

%union {
    int ival;
    double fval;
    char cval;
    char* sval;
    int type;
}

/* ============================================= */
/* TOKEN DECLARATIONS                           */
/* ============================================= */

/* Preprocessor */
%token INCLUDE DEFINE HEADER_FILE

/* Data Types */
%token TYPE_INT TYPE_FLOAT TYPE_DOUBLE TYPE_LONG_INT TYPE_CHAR TYPE_BOOL TYPE_VOID

/* Control Flow */
%token IF ELSE_IF ELSE FOR WHILE DO SWITCH CASE DEFAULT BREAK CONTINUE RETURN

/* Functions */
%token MAIN PRINT SCAN RETURN_TYPE_IND

/* Math Functions */
%token FUNC_POWER FUNC_SQRT FUNC_FLOOR FUNC_CEIL FUNC_ABS FUNC_LOG
%token FUNC_SIN FUNC_COS FUNC_TAN FUNC_ASIN FUNC_ACOS FUNC_ATAN FUNC_PRIME

/* Arithmetic Operators */
%token OP_ADD OP_SUB OP_MUL OP_DIV OP_MOD

/* Logical Operators */
%token OP_AND OP_OR OP_NOT OP_XOR

/* Relational Operators */
%token OP_LT OP_GT OP_LE OP_GE OP_EQ OP_NE

/* Assignment */
%token OP_ASSIGN

/* Punctuation */
%token SEMICOLON COLON COMMA LPAREN RPAREN LBRACE RBRACE LBRACKET RBRACKET

/* Literals */
%token <ival> INTEGER_LITERAL
%token <fval> FLOAT_LITERAL
%token <sval> STRING_LITERAL
%token <cval> CHAR_LITERAL

/* Identifier */
%token <sval> IDENTIFIER

/* Comments (usually ignored, but declared for completeness) */
%token COMMENT_SINGLE COMMENT_MULTI

%type <type> type_specifier expression assignment_expression logical_or_expression
%type <type> logical_and_expression logical_xor_expression equality_expression
%type <type> relational_expression additive_expression multiplicative_expression
%type <type> unary_expression postfix_expression primary_expression function_call
%type <type> math_function_call call_arguments_opt call_argument_list
%type <type> function_prologue main_prologue
%type <sval> assignment_target

/* ============================================= */
/* OPERATOR PRECEDENCE (lowest to highest)      */
/* ============================================= */

%left OP_OR
%left OP_AND
%left OP_XOR
%left OP_EQ OP_NE
%left OP_LT OP_GT OP_LE OP_GE
%left OP_ADD OP_SUB
%left OP_MUL OP_DIV OP_MOD
%right OP_NOT
%right UMINUS
%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE_IF
%nonassoc ELSE

%%

/* ============================================= */
/* GRAMMAR RULES                                */
/* ============================================= */

program
    : global_declarations
        {
            if (syntax_errors == 0) {
                printf("Program parsed successfully.\n");
            }
        }
    ;

global_declarations
    : global_declaration
    | global_declarations global_declaration
    ;

global_declaration
    : preprocessor_directive
    | function_definition
    | declaration SEMICOLON
    | SEMICOLON
    | error SEMICOLON
        {
            yyerror("Invalid global declaration");
            yyerrok;
        }
    ;

/* ============================================= */
/* PREPROCESSOR DIRECTIVES                      */
/* ============================================= */

preprocessor_directive
    : INCLUDE HEADER_FILE
        { printf("  [Line %d] Include directive parsed\n", yylineno); }
    | DEFINE IDENTIFIER INTEGER_LITERAL
        {
            printf("  [Line %d] Macro definition parsed\n", yylineno);
            free($2);
        }
    | DEFINE IDENTIFIER FLOAT_LITERAL
        {
            printf("  [Line %d] Macro definition parsed\n", yylineno);
            free($2);
        }
    | DEFINE IDENTIFIER
        {
            printf("  [Line %d] Macro definition parsed\n", yylineno);
            free($2);
        }
    ;

/* ============================================= */
/* FUNCTION DEFINITIONS                         */
/* ============================================= */

function_prologue
    : type_specifier IDENTIFIER
        {
            begin_function_context($1, $2);
                current_function_param_count = 0;
            $$ = $1;
            free($2);
        }
    ;

main_prologue
    : type_specifier MAIN
        {
            begin_function_context($1, "starta");
                current_function_param_count = 0;
            $$ = $1;
        }
    ;

function_definition
    : main_prologue LPAREN RPAREN function_body
        {
                register_current_function_signature();
            end_function_context();
            printf("  [Line %d] Main function parsed\n", yylineno);
        }
    | main_prologue LPAREN parameter_list RPAREN function_body
        {
                register_current_function_signature();
            end_function_context();
            printf("  [Line %d] Main function with parameters parsed\n", yylineno);
        }
    | function_prologue LPAREN RPAREN function_body
        {
                register_current_function_signature();
            end_function_context();
            printf("  [Line %d] Function definition parsed\n", yylineno);
        }
    | function_prologue LPAREN parameter_list RPAREN function_body
        {
                register_current_function_signature();
            end_function_context();
            printf("  [Line %d] Function with parameters parsed\n", yylineno);
        }
    | function_prologue LPAREN RPAREN RETURN_TYPE_IND type_specifier function_body
        {
            if (!is_assignment_compatible($1, $5)) {
                semantic_error("Function header type and return-indicator type mismatch", current_function_name);
            }
                register_current_function_signature();
            end_function_context();
            printf("  [Line %d] Function with return type indicator parsed\n", yylineno);
        }
    | function_prologue LPAREN parameter_list RPAREN RETURN_TYPE_IND type_specifier function_body
        {
            if (!is_assignment_compatible($1, $6)) {
                semantic_error("Function header type and return-indicator type mismatch", current_function_name);
            }
                register_current_function_signature();
            end_function_context();
            printf("  [Line %d] Function with parameters and return type parsed\n", yylineno);
        }
    ;

parameter_list
    : parameter
    | parameter_list COMMA parameter
    ;

parameter
    : type_specifier IDENTIFIER
        {
            if (declare_symbol($2, $1, 1, 0) == -1) {
                semantic_error("Parameter redeclared", $2);
            }
            if (inside_function && current_function_param_count < MAX_PARAMS) {
                current_function_param_types[current_function_param_count++] = $1;
            }
            free($2);
        }
    | type_specifier IDENTIFIER LBRACKET RBRACKET
        {
            if (declare_symbol($2, $1, 1, 1) == -1) {
                semantic_error("Parameter redeclared", $2);
            }
            if (inside_function && current_function_param_count < MAX_PARAMS) {
                current_function_param_types[current_function_param_count++] = $1;
            }
            free($2);
        }
    ;

/* ============================================= */
/* TYPE SPECIFIERS                              */
/* ============================================= */

type_specifier
    : TYPE_INT       { $$ = T_INT; current_decl_type = $$; }
    | TYPE_FLOAT     { $$ = T_FLOAT; current_decl_type = $$; }
    | TYPE_DOUBLE    { $$ = T_DOUBLE; current_decl_type = $$; }
    | TYPE_LONG_INT  { $$ = T_LONG_INT; current_decl_type = $$; }
    | TYPE_CHAR      { $$ = T_CHAR; current_decl_type = $$; }
    | TYPE_BOOL      { $$ = T_BOOL; current_decl_type = $$; }
    | TYPE_VOID      { $$ = T_VOID; current_decl_type = $$; }
    ;

/* ============================================= */
/* DECLARATIONS                                 */
/* ============================================= */

declaration
    : type_specifier declarator_list
    ;

declarator_list
    : declarator
    | declarator_list COMMA declarator
    ;

declarator
    : IDENTIFIER
        {
            if (declare_symbol($1, current_decl_type, 0, 0) == -1) {
                semantic_error("Variable redeclared", $1);
            } else {
                printf("  [Line %d] Variable declared: %s\n", yylineno, $1);
            }
            free($1);
        }
    | IDENTIFIER OP_ASSIGN expression
        {
            int idx = declare_symbol($1, current_decl_type, 1, 0);
            if (idx == -1) {
                semantic_error("Variable redeclared", $1);
            } else if (!is_assignment_compatible(current_decl_type, $3)) {
                semantic_error("Type mismatch in declaration assignment", $1);
            } else {
                printf("  [Line %d] Variable declared and initialized\n", yylineno);
            }
            free($1);
        }
    | IDENTIFIER LBRACKET INTEGER_LITERAL RBRACKET
        {
            int idx = declare_symbol($1, current_decl_type, 1, 1);
            if (idx == -1) {
                semantic_error("Array redeclared", $1);
            } else {
                printf("  [Line %d] Array declared\n", yylineno);
            }
            free($1);
        }
    | IDENTIFIER LBRACKET INTEGER_LITERAL RBRACKET OP_ASSIGN LBRACE initializer_list RBRACE
        {
            int idx = declare_symbol($1, current_decl_type, 1, 1);
            if (idx == -1) {
                semantic_error("Array redeclared", $1);
            } else {
                printf("  [Line %d] Array declared and initialized\n", yylineno);
            }
            free($1);
        }
    | IDENTIFIER LBRACKET RBRACKET OP_ASSIGN LBRACE initializer_list RBRACE
        {
            int idx = declare_symbol($1, current_decl_type, 1, 1);
            if (idx == -1) {
                semantic_error("Array redeclared", $1);
            } else {
                printf("  [Line %d] Array declared with inferred size\n", yylineno);
            }
            free($1);
        }
    ;

initializer_list
    : expression
    | initializer_list COMMA expression
    ;

/* ============================================= */
/* STATEMENTS                                   */
/* ============================================= */

function_body
    : LBRACE RBRACE
    | LBRACE statement_list RBRACE
    ;

block_start
    : /* empty */
        { enter_scope(); }
    ;

scoped_compound_statement
    : LBRACE block_start RBRACE
        { exit_scope(); }
    | LBRACE block_start statement_list RBRACE
        { exit_scope(); }
    ;

statement_list
    : statement
    | statement_list statement
    ;

statement
    : declaration SEMICOLON
    | expression_statement
    | scoped_compound_statement
    | selection_statement
    | iteration_statement
    | jump_statement
    | io_statement
    | SEMICOLON
    | error SEMICOLON
        {
            yyerror("Invalid statement");
            yyerrok;
        }
    ;

expression_statement
    : expression SEMICOLON
    ;

/* ============================================= */
/* SELECTION STATEMENTS (if, switch)            */
/* ============================================= */

selection_statement
    : IF LPAREN expression RPAREN statement %prec LOWER_THAN_ELSE
        { printf("  [Line %d] If statement parsed\n", yylineno); }
    | IF LPAREN expression RPAREN statement ELSE statement
        { printf("  [Line %d] If-else statement parsed\n", yylineno); }
    | IF LPAREN expression RPAREN statement else_if_chain %prec LOWER_THAN_ELSE
        { printf("  [Line %d] If-else-if chain parsed\n", yylineno); }
    | IF LPAREN expression RPAREN statement else_if_chain ELSE statement
        { printf("  [Line %d] Complete if-else-if-else parsed\n", yylineno); }
    | SWITCH LPAREN expression RPAREN LBRACE case_list RBRACE
        { printf("  [Line %d] Switch statement parsed\n", yylineno); }
    ;

else_if_chain
    : ELSE_IF LPAREN expression RPAREN statement
    | else_if_chain ELSE_IF LPAREN expression RPAREN statement
    ;

case_list
    : case_statement
    | case_list case_statement
    ;

case_statement
    : CASE expression COLON statement_list
        { printf("  [Line %d] Case clause parsed\n", yylineno); }
    | CASE expression COLON
        { printf("  [Line %d] Empty case clause parsed\n", yylineno); }
    | DEFAULT COLON statement_list
        { printf("  [Line %d] Default clause parsed\n", yylineno); }
    | DEFAULT COLON
        { printf("  [Line %d] Empty default clause parsed\n", yylineno); }
    ;

/* ============================================= */
/* ITERATION STATEMENTS (for, while, do-while)  */
/* ============================================= */

iteration_statement
    : WHILE LPAREN expression RPAREN statement
        { printf("  [Line %d] While loop parsed\n", yylineno); }
    | DO scoped_compound_statement WHILE LPAREN expression RPAREN SEMICOLON
        { printf("  [Line %d] Do-while loop parsed\n", yylineno); }
    | FOR LPAREN for_init COLON expression COLON for_update RPAREN statement
        { printf("  [Line %d] For loop parsed\n", yylineno); }
    | FOR LPAREN for_init COLON COLON for_update RPAREN statement
        { printf("  [Line %d] For loop (infinite condition) parsed\n", yylineno); }
    | FOR LPAREN COLON expression COLON RPAREN statement
        { printf("  [Line %d] For loop (no init/update) parsed\n", yylineno); }
    ;

for_init
    : declaration
    | expression
    ;

for_update
    : expression
    | /* empty */
    ;

/* ============================================= */
/* JUMP STATEMENTS                              */
/* ============================================= */

jump_statement
    : RETURN SEMICOLON
        {
            if (!inside_function) {
                semantic_error("Return used outside a function", NULL);
            } else if (current_function_return_type != T_VOID) {
                semantic_error("Non-void function must return a value", current_function_name);
            }
            current_function_has_return = 1;
            printf("  [Line %d] Return statement (void) parsed\n", yylineno);
        }
    | RETURN expression SEMICOLON
        {
            if (!inside_function) {
                semantic_error("Return used outside a function", NULL);
            } else if (current_function_return_type == T_VOID) {
                semantic_error("Void function cannot return a value", current_function_name);
            } else if (!is_assignment_compatible(current_function_return_type, $2)) {
                semantic_error("Return type mismatch", current_function_name);
            }
            current_function_has_return = 1;
            printf("  [Line %d] Return statement parsed\n", yylineno);
        }
    | BREAK SEMICOLON
        { printf("  [Line %d] Break statement parsed\n", yylineno); }
    | CONTINUE SEMICOLON
        { printf("  [Line %d] Continue statement parsed\n", yylineno); }
    ;

/* ============================================= */
/* I/O STATEMENTS                               */
/* ============================================= */

io_statement
    : PRINT LPAREN argument_list RPAREN SEMICOLON
        { printf("  [Line %d] Print statement parsed\n", yylineno); }
    | SCAN LPAREN argument_list RPAREN SEMICOLON
        { printf("  [Line %d] Scan statement parsed\n", yylineno); }
    ;

/* ============================================= */
/* EXPRESSIONS                                  */
/* ============================================= */

expression
    : assignment_expression
        { $$ = $1; }
    ;

assignment_expression
    : logical_or_expression
        { $$ = $1; }
    | assignment_target OP_ASSIGN assignment_expression
        {
            int idx = find_symbol($1);
            if (idx < 0) {
                semantic_error("Assignment to undeclared variable", $1);
                $$ = T_ERR;
            } else {
                if (!is_assignment_compatible(symbols[idx].type, $3)) {
                    semantic_error("Type mismatch in assignment", $1);
                }
                symbols[idx].initialized = 1;
                $$ = symbols[idx].type;
                printf("  [Line %d] Assignment expression parsed\n", yylineno);
            }
            free($1);
        }
    ;

assignment_target
    : IDENTIFIER
        { $$ = $1; }
    | IDENTIFIER LBRACKET expression RBRACKET
        {
            int idx = find_symbol($1);
            if (idx < 0) {
                semantic_error("Array used before declaration", $1);
            } else if (!symbols[idx].is_array) {
                semantic_error("Indexed access on non-array variable", $1);
            }
            $$ = $1;
        }
    ;

logical_or_expression
    : logical_and_expression
        { $$ = $1; }
    | logical_or_expression OP_OR logical_and_expression
        {
            if (!is_numeric($1) || !is_numeric($3)) {
                semantic_error("Logical OR expects numeric/bool operands", NULL);
                $$ = T_ERR;
            } else {
                $$ = T_BOOL;
            }
        }
    ;

logical_and_expression
    : logical_xor_expression
        { $$ = $1; }
    | logical_and_expression OP_AND logical_xor_expression
        {
            if (!is_numeric($1) || !is_numeric($3)) {
                semantic_error("Logical AND expects numeric/bool operands", NULL);
                $$ = T_ERR;
            } else {
                $$ = T_BOOL;
            }
        }
    ;

logical_xor_expression
    : equality_expression
        { $$ = $1; }
    | logical_xor_expression OP_XOR equality_expression
        {
            if (!is_numeric($1) || !is_numeric($3)) {
                semantic_error("Logical XOR expects numeric/bool operands", NULL);
                $$ = T_ERR;
            } else {
                $$ = T_BOOL;
            }
        }
    ;

equality_expression
    : relational_expression
        { $$ = $1; }
    | equality_expression OP_EQ relational_expression
        { $$ = T_BOOL; }
    | equality_expression OP_NE relational_expression
        { $$ = T_BOOL; }
    ;

relational_expression
    : additive_expression
        { $$ = $1; }
    | relational_expression OP_LT additive_expression
        { $$ = T_BOOL; }
    | relational_expression OP_GT additive_expression
        { $$ = T_BOOL; }
    | relational_expression OP_LE additive_expression
        { $$ = T_BOOL; }
    | relational_expression OP_GE additive_expression
        { $$ = T_BOOL; }
    ;

additive_expression
    : multiplicative_expression
        { $$ = $1; }
    | additive_expression OP_ADD multiplicative_expression
        {
            $$ = merge_numeric_type($1, $3);
            if ($$ == T_ERR) semantic_error("Invalid operands for addition", NULL);
        }
    | additive_expression OP_SUB multiplicative_expression
        {
            $$ = merge_numeric_type($1, $3);
            if ($$ == T_ERR) semantic_error("Invalid operands for subtraction", NULL);
        }
    ;

multiplicative_expression
    : unary_expression
        { $$ = $1; }
    | multiplicative_expression OP_MUL unary_expression
        {
            $$ = merge_numeric_type($1, $3);
            if ($$ == T_ERR) semantic_error("Invalid operands for multiplication", NULL);
        }
    | multiplicative_expression OP_DIV unary_expression
        {
            $$ = merge_numeric_type($1, $3);
            if ($$ == T_ERR) semantic_error("Invalid operands for division", NULL);
        }
    | multiplicative_expression OP_MOD unary_expression
        {
            if (!is_numeric($1) || !is_numeric($3)) {
                semantic_error("Modulus expects numeric operands", NULL);
                $$ = T_ERR;
            } else {
                $$ = T_INT;
            }
        }
    ;

unary_expression
    : postfix_expression
        { $$ = $1; }
    | OP_NOT unary_expression
        {
            if (!is_numeric($2)) {
                semantic_error("Logical NOT expects numeric/bool operand", NULL);
                $$ = T_ERR;
            } else {
                $$ = T_BOOL;
            }
        }
    | OP_SUB unary_expression %prec UMINUS
        {
            if (!is_numeric($2)) {
                semantic_error("Unary minus expects numeric operand", NULL);
                $$ = T_ERR;
            } else {
                $$ = $2;
            }
        }
    ;

postfix_expression
    : primary_expression
        { $$ = $1; }
    | IDENTIFIER LBRACKET expression RBRACKET
        {
            int idx = find_symbol($1);
            if (idx < 0) {
                semantic_error("Array used before declaration", $1);
                $$ = T_ERR;
            } else if (!symbols[idx].is_array) {
                semantic_error("Indexed access on non-array variable", $1);
                $$ = T_ERR;
            } else {
                $$ = symbols[idx].type;
                printf("  [Line %d] Array access parsed\n", yylineno);
            }
            free($1);
        }
    | function_call
        { $$ = $1; }
    ;

primary_expression
    : IDENTIFIER
        {
            int idx = find_symbol($1);
            if (idx < 0) {
                semantic_error("Variable used before declaration", $1);
                $$ = T_ERR;
            } else {
                if (!symbols[idx].initialized) {
                    semantic_error("Variable may be uninitialized", $1);
                }
                $$ = symbols[idx].type;
            }
            free($1);
        }
    | INTEGER_LITERAL
        { $$ = T_INT; }
    | FLOAT_LITERAL
        { $$ = T_FLOAT; }
    | STRING_LITERAL
        {
            $$ = T_STRING;
            free($1);
        }
    | CHAR_LITERAL
        { $$ = T_CHAR; }
    | LPAREN expression RPAREN
        { $$ = $2; }
    ;

/* ============================================= */
/* FUNCTION CALLS                               */
/* ============================================= */

function_call
    : IDENTIFIER LPAREN call_arguments_opt RPAREN
        {
            printf("  [Line %d] Function call parsed\n", yylineno);
            $$ = validate_function_call($1);
            free($1);
        }
    | math_function_call
        { $$ = $1; }
    ;

call_arguments_opt
    : /* empty */
        {
            reset_call_args();
            $$ = T_VOID;
        }
    | call_argument_list
        {
            $$ = T_VOID;
        }
    ;

call_argument_list
    : assignment_expression
        {
            reset_call_args();
            push_call_arg($1);
            $$ = T_VOID;
        }
    | call_argument_list COMMA assignment_expression
        {
            push_call_arg($3);
            $$ = T_VOID;
        }
    ;

math_function_call
    : FUNC_POWER LPAREN expression COMMA expression RPAREN
        { printf("  [Line %d] Power function call parsed\n", yylineno); $$ = T_DOUBLE; }
    | FUNC_SQRT LPAREN expression RPAREN
        { printf("  [Line %d] Square root function call parsed\n", yylineno); $$ = T_DOUBLE; }
    | FUNC_FLOOR LPAREN expression RPAREN
        { printf("  [Line %d] Floor function call parsed\n", yylineno); $$ = T_DOUBLE; }
    | FUNC_CEIL LPAREN expression RPAREN
        { printf("  [Line %d] Ceil function call parsed\n", yylineno); $$ = T_DOUBLE; }
    | FUNC_ABS LPAREN expression RPAREN
        { printf("  [Line %d] Abs function call parsed\n", yylineno); $$ = T_INT; }
    | FUNC_LOG LPAREN expression RPAREN
        { printf("  [Line %d] Log function call parsed\n", yylineno); $$ = T_DOUBLE; }
    | FUNC_SIN LPAREN expression RPAREN
        { printf("  [Line %d] Sine function call parsed\n", yylineno); $$ = T_DOUBLE; }
    | FUNC_COS LPAREN expression RPAREN
        { printf("  [Line %d] Cosine function call parsed\n", yylineno); $$ = T_DOUBLE; }
    | FUNC_TAN LPAREN expression RPAREN
        { printf("  [Line %d] Tangent function call parsed\n", yylineno); $$ = T_DOUBLE; }
    | FUNC_ASIN LPAREN expression RPAREN
        { printf("  [Line %d] Arc sine function call parsed\n", yylineno); $$ = T_DOUBLE; }
    | FUNC_ACOS LPAREN expression RPAREN
        { printf("  [Line %d] Arc cosine function call parsed\n", yylineno); $$ = T_DOUBLE; }
    | FUNC_ATAN LPAREN expression RPAREN
        { printf("  [Line %d] Arc tangent function call parsed\n", yylineno); $$ = T_DOUBLE; }
    | FUNC_PRIME LPAREN expression RPAREN
        { printf("  [Line %d] Prime check function call parsed\n", yylineno); $$ = T_BOOL; }
    ;

argument_list
    : assignment_expression
    | argument_list COMMA assignment_expression
    ;

%%

/* ============================================= */
/* ERROR HANDLING                               */
/* ============================================= */

void yyerror(const char* s) {
    if (syntax_errors > 0) {
        return;
    }
    syntax_errors++;
    fprintf(stderr, "\n============================================================\n");
    fprintf(stderr, "  SYNTAX ERROR                                              \n");
    fprintf(stderr, "============================================================\n");
    fprintf(stderr, "  Line %d: %s\n", yylineno, s);
    fprintf(stderr, "  Near token: '%s'\n", yytext ? yytext : "(null)");
    fprintf(stderr, "============================================================\n\n");
}

/* ============================================= */
/* MAIN FUNCTION                                */
/* ============================================= */

int main(int argc, char* argv[]) {
    int i;
    printf("\n");
    printf("============================================================\n");
    printf("         MinionLang Parser - Syntax Analyzer                \n");
    printf("         Compiler Design Lab (CSE)                          \n");
    printf("============================================================\n\n");

    if (argc < 2) {
        printf("Usage: %s <input_file.minion>\n", argv[0]);
        return 1;
    }

    yyin = fopen(argv[1], "r");
    if (!yyin) {
        fprintf(stderr, "Error: Cannot open file '%s'\n", argv[1]);
        return 1;
    }

    printf("Parsing file: %s\n", argv[1]);
    printf("============================================================\n\n");

    enter_scope();

    i = yyparse();

    fclose(yyin);

    printf("\n============================================================\n");
    if (i == 0 && syntax_errors == 0 && semantic_errors == 0) {
        printf("  [OK] PARSING + BASIC SEMANTIC CHECKS PASSED               \n");
    } else {
        printf("  [FAIL] Syntax errors: %d, Semantic errors: %d              \n", syntax_errors, semantic_errors);
    }
    printf("============================================================\n");

    while (scope_top >= 0) {
        exit_scope();
    }

    return (syntax_errors || semantic_errors) ? 1 : 0;
}
