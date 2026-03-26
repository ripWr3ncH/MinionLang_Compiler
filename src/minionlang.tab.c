
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.4.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Copy the first part of user declarations.  */

/* Line 189 of yacc.c  */
#line 1 "src\\minionlang.y"

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



/* Line 189 of yacc.c  */
#line 368 "src\\minionlang.tab.c"

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     INCLUDE = 258,
     DEFINE = 259,
     HEADER_FILE = 260,
     TYPE_INT = 261,
     TYPE_FLOAT = 262,
     TYPE_DOUBLE = 263,
     TYPE_LONG_INT = 264,
     TYPE_CHAR = 265,
     TYPE_BOOL = 266,
     TYPE_VOID = 267,
     IF = 268,
     ELSE_IF = 269,
     ELSE = 270,
     FOR = 271,
     WHILE = 272,
     DO = 273,
     SWITCH = 274,
     CASE = 275,
     DEFAULT = 276,
     BREAK = 277,
     CONTINUE = 278,
     RETURN = 279,
     MAIN = 280,
     PRINT = 281,
     SCAN = 282,
     RETURN_TYPE_IND = 283,
     FUNC_POWER = 284,
     FUNC_SQRT = 285,
     FUNC_FLOOR = 286,
     FUNC_CEIL = 287,
     FUNC_ABS = 288,
     FUNC_LOG = 289,
     FUNC_SIN = 290,
     FUNC_COS = 291,
     FUNC_TAN = 292,
     FUNC_ASIN = 293,
     FUNC_ACOS = 294,
     FUNC_ATAN = 295,
     FUNC_PRIME = 296,
     OP_ADD = 297,
     OP_SUB = 298,
     OP_MUL = 299,
     OP_DIV = 300,
     OP_MOD = 301,
     OP_AND = 302,
     OP_OR = 303,
     OP_NOT = 304,
     OP_XOR = 305,
     OP_LT = 306,
     OP_GT = 307,
     OP_LE = 308,
     OP_GE = 309,
     OP_EQ = 310,
     OP_NE = 311,
     OP_ASSIGN = 312,
     SEMICOLON = 313,
     COLON = 314,
     COMMA = 315,
     LPAREN = 316,
     RPAREN = 317,
     LBRACE = 318,
     RBRACE = 319,
     LBRACKET = 320,
     RBRACKET = 321,
     INTEGER_LITERAL = 322,
     FLOAT_LITERAL = 323,
     STRING_LITERAL = 324,
     CHAR_LITERAL = 325,
     IDENTIFIER = 326,
     COMMENT_SINGLE = 327,
     COMMENT_MULTI = 328,
     UMINUS = 329,
     LOWER_THAN_ELSE = 330
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 214 of yacc.c  */
#line 295 "src\\minionlang.y"

    int ival;
    double fval;
    char cval;
    char* sval;
    int type;



/* Line 214 of yacc.c  */
#line 489 "src\\minionlang.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 501 "src\\minionlang.tab.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  24
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   1103

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  76
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  48
/* YYNRULES -- Number of rules.  */
#define YYNRULES  146
/* YYNRULES -- Number of states.  */
#define YYNSTATES  317

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   330

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     5,     7,    10,    12,    14,    17,    19,
      22,    25,    29,    33,    36,    39,    42,    47,    53,    58,
      64,    71,    79,    81,    85,    88,    93,    95,    97,    99,
     101,   103,   105,   107,   110,   112,   116,   118,   122,   127,
     136,   144,   146,   150,   153,   157,   158,   162,   167,   169,
     172,   175,   177,   179,   181,   183,   185,   187,   189,   192,
     195,   201,   209,   216,   225,   233,   239,   246,   248,   251,
     256,   260,   264,   267,   273,   281,   291,   300,   308,   310,
     312,   314,   315,   318,   322,   325,   328,   334,   340,   342,
     344,   348,   350,   355,   357,   361,   363,   367,   369,   373,
     375,   379,   383,   385,   389,   393,   397,   401,   403,   407,
     411,   413,   417,   421,   425,   427,   430,   433,   435,   440,
     442,   444,   446,   448,   450,   452,   456,   461,   463,   464,
     466,   468,   472,   479,   484,   489,   494,   499,   504,   509,
     514,   519,   524,   529,   534,   539,   541
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      77,     0,    -1,    78,    -1,    79,    -1,    78,    79,    -1,
      80,    -1,    83,    -1,    87,    58,    -1,    58,    -1,     1,
      58,    -1,     3,     5,    -1,     4,    71,    67,    -1,     4,
      71,    68,    -1,     4,    71,    -1,    86,    71,    -1,    86,
      25,    -1,    82,    61,    62,    91,    -1,    82,    61,    84,
      62,    91,    -1,    81,    61,    62,    91,    -1,    81,    61,
      84,    62,    91,    -1,    81,    61,    62,    28,    86,    91,
      -1,    81,    61,    84,    62,    28,    86,    91,    -1,    85,
      -1,    84,    60,    85,    -1,    86,    71,    -1,    86,    71,
      65,    66,    -1,     6,    -1,     7,    -1,     8,    -1,     9,
      -1,    10,    -1,    11,    -1,    12,    -1,    86,    88,    -1,
      89,    -1,    88,    60,    89,    -1,    71,    -1,    71,    57,
     106,    -1,    71,    65,    67,    66,    -1,    71,    65,    67,
      66,    57,    63,    90,    64,    -1,    71,    65,    66,    57,
      63,    90,    64,    -1,   106,    -1,    90,    60,   106,    -1,
      63,    64,    -1,    63,    94,    64,    -1,    -1,    63,    92,
      64,    -1,    63,    92,    94,    64,    -1,    95,    -1,    94,
      95,    -1,    87,    58,    -1,    96,    -1,    93,    -1,    97,
      -1,   101,    -1,   104,    -1,   105,    -1,    58,    -1,     1,
      58,    -1,   106,    58,    -1,    13,    61,   106,    62,    95,
      -1,    13,    61,   106,    62,    95,    15,    95,    -1,    13,
      61,   106,    62,    95,    98,    -1,    13,    61,   106,    62,
      95,    98,    15,    95,    -1,    19,    61,   106,    62,    63,
      99,    64,    -1,    14,    61,   106,    62,    95,    -1,    98,
      14,    61,   106,    62,    95,    -1,   100,    -1,    99,   100,
      -1,    20,   106,    59,    94,    -1,    20,   106,    59,    -1,
      21,    59,    94,    -1,    21,    59,    -1,    17,    61,   106,
      62,    95,    -1,    18,    93,    17,    61,   106,    62,    58,
      -1,    16,    61,   102,    59,   106,    59,   103,    62,    95,
      -1,    16,    61,   102,    59,    59,   103,    62,    95,    -1,
      16,    61,    59,   106,    59,    62,    95,    -1,    87,    -1,
     106,    -1,   106,    -1,    -1,    24,    58,    -1,    24,   106,
      58,    -1,    22,    58,    -1,    23,    58,    -1,    26,    61,
     123,    62,    58,    -1,    27,    61,   123,    62,    58,    -1,
     107,    -1,   109,    -1,   108,    57,   107,    -1,    71,    -1,
      71,    65,   106,    66,    -1,   110,    -1,   109,    48,   110,
      -1,   111,    -1,   110,    47,   111,    -1,   112,    -1,   111,
      50,   112,    -1,   113,    -1,   112,    55,   113,    -1,   112,
      56,   113,    -1,   114,    -1,   113,    51,   114,    -1,   113,
      52,   114,    -1,   113,    53,   114,    -1,   113,    54,   114,
      -1,   115,    -1,   114,    42,   115,    -1,   114,    43,   115,
      -1,   116,    -1,   115,    44,   116,    -1,   115,    45,   116,
      -1,   115,    46,   116,    -1,   117,    -1,    49,   116,    -1,
      43,   116,    -1,   118,    -1,    71,    65,   106,    66,    -1,
     119,    -1,    71,    -1,    67,    -1,    68,    -1,    69,    -1,
      70,    -1,    61,   106,    62,    -1,    71,    61,   120,    62,
      -1,   122,    -1,    -1,   121,    -1,   107,    -1,   121,    60,
     107,    -1,    29,    61,   106,    60,   106,    62,    -1,    30,
      61,   106,    62,    -1,    31,    61,   106,    62,    -1,    32,
      61,   106,    62,    -1,    33,    61,   106,    62,    -1,    34,
      61,   106,    62,    -1,    35,    61,   106,    62,    -1,    36,
      61,   106,    62,    -1,    37,    61,   106,    62,    -1,    38,
      61,   106,    62,    -1,    39,    61,   106,    62,    -1,    40,
      61,   106,    62,    -1,    41,    61,   106,    62,    -1,   107,
      -1,   123,    60,   107,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   382,   382,   391,   392,   396,   397,   398,   399,   400,
     412,   414,   419,   424,   436,   446,   455,   461,   467,   473,
     479,   488,   500,   501,   505,   515,   532,   533,   534,   535,
     536,   537,   538,   546,   550,   551,   555,   564,   576,   586,
     596,   609,   610,   618,   619,   624,   628,   630,   635,   636,
     640,   641,   642,   643,   644,   645,   646,   647,   648,   656,
     664,   666,   668,   670,   672,   677,   678,   682,   683,   687,
     689,   691,   693,   702,   704,   706,   708,   710,   715,   716,
     720,   721,   729,   739,   751,   753,   762,   764,   773,   778,
     780,   799,   801,   814,   816,   828,   830,   842,   844,   856,
     858,   860,   865,   867,   869,   871,   873,   878,   880,   885,
     893,   895,   900,   905,   917,   919,   928,   940,   942,   957,
     962,   976,   978,   980,   985,   987,   996,  1002,  1008,  1012,
    1019,  1025,  1033,  1035,  1037,  1039,  1041,  1043,  1045,  1047,
    1049,  1051,  1053,  1055,  1057,  1062,  1063
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "INCLUDE", "DEFINE", "HEADER_FILE",
  "TYPE_INT", "TYPE_FLOAT", "TYPE_DOUBLE", "TYPE_LONG_INT", "TYPE_CHAR",
  "TYPE_BOOL", "TYPE_VOID", "IF", "ELSE_IF", "ELSE", "FOR", "WHILE", "DO",
  "SWITCH", "CASE", "DEFAULT", "BREAK", "CONTINUE", "RETURN", "MAIN",
  "PRINT", "SCAN", "RETURN_TYPE_IND", "FUNC_POWER", "FUNC_SQRT",
  "FUNC_FLOOR", "FUNC_CEIL", "FUNC_ABS", "FUNC_LOG", "FUNC_SIN",
  "FUNC_COS", "FUNC_TAN", "FUNC_ASIN", "FUNC_ACOS", "FUNC_ATAN",
  "FUNC_PRIME", "OP_ADD", "OP_SUB", "OP_MUL", "OP_DIV", "OP_MOD", "OP_AND",
  "OP_OR", "OP_NOT", "OP_XOR", "OP_LT", "OP_GT", "OP_LE", "OP_GE", "OP_EQ",
  "OP_NE", "OP_ASSIGN", "SEMICOLON", "COLON", "COMMA", "LPAREN", "RPAREN",
  "LBRACE", "RBRACE", "LBRACKET", "RBRACKET", "INTEGER_LITERAL",
  "FLOAT_LITERAL", "STRING_LITERAL", "CHAR_LITERAL", "IDENTIFIER",
  "COMMENT_SINGLE", "COMMENT_MULTI", "UMINUS", "LOWER_THAN_ELSE",
  "$accept", "program", "global_declarations", "global_declaration",
  "preprocessor_directive", "function_prologue", "main_prologue",
  "function_definition", "parameter_list", "parameter", "type_specifier",
  "declaration", "declarator_list", "declarator", "initializer_list",
  "function_body", "block_start", "scoped_compound_statement",
  "statement_list", "statement", "expression_statement",
  "selection_statement", "else_if_chain", "case_list", "case_statement",
  "iteration_statement", "for_init", "for_update", "jump_statement",
  "io_statement", "expression", "assignment_expression",
  "assignment_target", "logical_or_expression", "logical_and_expression",
  "logical_xor_expression", "equality_expression", "relational_expression",
  "additive_expression", "multiplicative_expression", "unary_expression",
  "postfix_expression", "primary_expression", "function_call",
  "call_arguments_opt", "call_argument_list", "math_function_call",
  "argument_list", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    76,    77,    78,    78,    79,    79,    79,    79,    79,
      80,    80,    80,    80,    81,    82,    83,    83,    83,    83,
      83,    83,    84,    84,    85,    85,    86,    86,    86,    86,
      86,    86,    86,    87,    88,    88,    89,    89,    89,    89,
      89,    90,    90,    91,    91,    92,    93,    93,    94,    94,
      95,    95,    95,    95,    95,    95,    95,    95,    95,    96,
      97,    97,    97,    97,    97,    98,    98,    99,    99,   100,
     100,   100,   100,   101,   101,   101,   101,   101,   102,   102,
     103,   103,   104,   104,   104,   104,   105,   105,   106,   107,
     107,   108,   108,   109,   109,   110,   110,   111,   111,   112,
     112,   112,   113,   113,   113,   113,   113,   114,   114,   114,
     115,   115,   115,   115,   116,   116,   116,   117,   117,   117,
     118,   118,   118,   118,   118,   118,   119,   119,   120,   120,
     121,   121,   122,   122,   122,   122,   122,   122,   122,   122,
     122,   122,   122,   122,   122,   123,   123
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     1,     2,     1,     1,     2,     1,     2,
       2,     3,     3,     2,     2,     2,     4,     5,     4,     5,
       6,     7,     1,     3,     2,     4,     1,     1,     1,     1,
       1,     1,     1,     2,     1,     3,     1,     3,     4,     8,
       7,     1,     3,     2,     3,     0,     3,     4,     1,     2,
       2,     1,     1,     1,     1,     1,     1,     1,     2,     2,
       5,     7,     6,     8,     7,     5,     6,     1,     2,     4,
       3,     3,     2,     5,     7,     9,     8,     7,     1,     1,
       1,     0,     2,     3,     2,     2,     5,     5,     1,     1,
       3,     1,     4,     1,     3,     1,     3,     1,     3,     1,
       3,     3,     1,     3,     3,     3,     3,     1,     3,     3,
       1,     3,     3,     3,     1,     2,     2,     1,     4,     1,
       1,     1,     1,     1,     1,     3,     4,     1,     0,     1,
       1,     3,     6,     4,     4,     4,     4,     4,     4,     4,
       4,     4,     4,     4,     4,     1,     3
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     0,    26,    27,    28,    29,    30,    31,
      32,     8,     0,     0,     3,     5,     0,     0,     6,     0,
       0,     9,    10,    13,     1,     4,     0,     0,    15,    36,
      33,    34,     7,    11,    12,     0,     0,    22,     0,     0,
       0,     0,     0,     0,     0,     0,    18,     0,     0,    24,
      16,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   121,   122,
     123,   124,   120,    37,    88,     0,    89,    93,    95,    97,
      99,   102,   107,   110,   114,   117,   119,   127,     0,     0,
      36,    35,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    57,    45,    43,     0,     0,    52,
       0,    48,    51,    53,    54,    55,    56,     0,    23,     0,
      19,     0,    17,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   120,   116,   115,     0,
     128,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    38,    20,
      58,     0,     0,     0,     0,     0,    84,    85,    82,     0,
       0,     0,     0,    50,    44,    49,    59,     0,    25,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   125,   130,     0,   129,     0,    90,    94,
      96,    98,   100,   101,   103,   104,   105,   106,   108,   109,
     111,   112,   113,     0,     0,     0,     0,    78,     0,    79,
       0,     0,     0,    83,   145,     0,     0,    46,     0,    21,
       0,   133,   134,   135,   136,   137,   138,   139,   140,   141,
     142,   143,   144,     0,   126,     0,   118,     0,    41,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    47,
       0,   118,   131,     0,    40,     0,    60,     0,    81,     0,
      73,     0,     0,   146,    86,    87,   132,    42,    39,     0,
       0,    62,     0,     0,    80,    81,     0,     0,     0,     0,
      67,     0,    61,     0,     0,    77,     0,     0,    74,     0,
       0,    64,    68,     0,     0,    63,    76,     0,     0,     0,
       0,     0,    75,     0,    65,     0,    66
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    12,    13,    14,    15,    16,    17,    18,    36,    37,
     107,   108,    30,    31,   247,    46,   172,   109,   110,   111,
     112,   113,   281,   289,   290,   114,   218,   283,   115,   116,
     117,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,   195,   196,    87,   225
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -170
static const yytype_int16 yypact[] =
{
      68,   -50,    17,   -44,  -170,  -170,  -170,  -170,  -170,  -170,
    -170,  -170,    46,    55,  -170,  -170,    12,    20,  -170,   -20,
      41,  -170,  -170,   -32,  -170,  -170,    33,   126,  -170,   -37,
      54,  -170,  -170,  -170,  -170,   -16,     8,  -170,    57,    77,
      36,   989,    80,    59,   146,   532,  -170,   146,   -14,    85,
    -170,    77,   102,   113,   118,   119,   120,   121,   122,   123,
     125,   129,   130,   131,   132,  1032,  1032,   989,  -170,  -170,
    -170,  -170,   -36,  -170,  -170,   137,   148,   150,   128,    93,
      51,   117,    65,  -170,  -170,  -170,  -170,  -170,   138,   133,
     -34,  -170,    77,   144,   142,   149,   151,   152,   155,   156,
     159,   903,   157,   158,  -170,  -170,  -170,    59,   162,  -170,
     603,  -170,  -170,  -170,  -170,  -170,  -170,   163,  -170,   146,
    -170,   139,  -170,   989,   989,   989,   989,   989,   989,   989,
     989,   989,   989,   989,   989,   989,   -31,  -170,  -170,   161,
     989,   989,   989,  1032,  1032,  1032,  1032,  1032,  1032,  1032,
    1032,  1032,  1032,  1032,  1032,  1032,  1032,   165,   167,  -170,
    -170,   989,   860,   989,   190,   989,  -170,  -170,  -170,   168,
     989,   989,   674,  -170,  -170,  -170,  -170,    77,  -170,   169,
     170,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   181,   989,  -170,  -170,   183,   187,   143,  -170,   150,
     128,    93,    51,    51,   117,   117,   117,   117,    65,    65,
    -170,  -170,  -170,   989,   185,   189,   989,  -170,   166,  -170,
     191,   201,   211,  -170,  -170,    67,    83,  -170,   745,  -170,
     989,  -170,  -170,  -170,  -170,  -170,  -170,  -170,  -170,  -170,
    -170,  -170,  -170,   164,  -170,   989,   195,   -12,  -170,   989,
     816,   217,   946,   816,   989,   227,   989,   234,   235,  -170,
     232,  -170,  -170,   989,  -170,    37,   147,   233,   989,   237,
    -170,   236,   145,  -170,  -170,  -170,  -170,  -170,  -170,   238,
     816,   153,   816,   239,  -170,   989,   242,   989,   243,   -10,
    -170,   989,  -170,   244,   816,  -170,   816,   241,  -170,   245,
     248,  -170,  -170,   246,   989,  -170,  -170,   816,   319,   390,
     816,   251,  -170,   461,  -170,   816,  -170
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -170,  -170,  -170,   218,  -170,  -170,  -170,  -170,   280,   263,
       6,     2,  -170,   271,    72,   -35,  -170,   225,  -169,  -109,
    -170,  -170,  -170,  -170,    34,  -170,  -170,    39,  -170,  -170,
     -41,  -133,  -170,  -170,   204,   200,   188,    23,   -33,    24,
     -48,  -170,  -170,  -170,  -170,  -170,  -170,   192
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -93
static const yytype_int16 yytable[] =
{
      73,   175,    20,   228,    50,    28,    19,   194,    21,   198,
     287,   288,    44,   120,   119,    20,   122,   137,   138,    19,
      41,   -91,    22,    41,   -14,   140,   139,    23,    42,   141,
     140,    42,    38,    38,   192,    33,    34,   224,   224,     4,
       5,     6,     7,     8,     9,    10,    24,    45,   263,    45,
      92,    29,   264,    38,   301,    -2,     1,   159,     2,     3,
     169,     4,     5,     6,     7,     8,     9,    10,    47,     1,
      48,     2,     3,    26,     4,     5,     6,     7,     8,     9,
      10,    27,   179,   180,   181,   182,   183,   184,   185,   186,
     187,   188,   189,   190,   191,    35,    47,   263,    51,    32,
     197,   278,   148,   149,   150,   151,   210,   211,   212,   154,
     155,   156,   262,    11,    43,   204,   205,   206,   207,   175,
     215,   219,   220,   273,   222,   177,    11,   256,    49,   257,
      90,   309,     4,     5,     6,     7,     8,     9,    10,   313,
      45,   266,   229,   256,   270,   258,    88,    89,   146,   147,
     121,   243,     4,     5,     6,     7,     8,     9,    10,   152,
     153,   279,   280,   123,   217,   287,   288,   293,   294,   202,
     203,   292,   248,   295,   124,   251,   208,   209,   145,   125,
     126,   127,   128,   129,   130,   305,   131,   306,    39,   260,
     132,   133,   134,   135,   142,   157,   143,   144,   312,   158,
     175,   314,   160,   161,   175,   178,   316,   221,   248,   246,
     162,   269,   163,   271,   166,   105,   165,   167,   170,   171,
     173,   176,   277,   193,   214,   252,   223,   284,   213,   230,
     261,    25,   231,   232,   233,   234,   235,   236,   237,   238,
     239,   240,   241,   242,   284,   244,   299,   245,   249,    93,
     303,   250,   -92,   253,     4,     5,     6,     7,     8,     9,
      10,    94,   254,   311,    95,    96,    97,    98,   -72,   -72,
      99,   100,   101,   255,   102,   103,   267,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
     272,    65,   274,   275,   276,   282,   285,    66,   286,   291,
     298,   296,   300,   307,   308,   304,   104,    40,   310,    67,
     118,   105,   -72,   315,    91,    68,    69,    70,    71,    72,
      93,   265,   164,   302,   297,     4,     5,     6,     7,     8,
       9,    10,    94,   201,     0,    95,    96,    97,    98,   -70,
     -70,    99,   100,   101,   200,   102,   103,   199,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    63,
      64,     0,    65,   226,     0,     0,     0,     0,    66,     0,
       0,     0,     0,     0,     0,     0,     0,   104,     0,     0,
      67,     0,   105,   -70,     0,     0,    68,    69,    70,    71,
      72,    93,     0,     0,     0,     0,     4,     5,     6,     7,
       8,     9,    10,    94,     0,     0,    95,    96,    97,    98,
     -71,   -71,    99,   100,   101,     0,   102,   103,     0,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    64,     0,    65,     0,     0,     0,     0,     0,    66,
       0,     0,     0,     0,     0,     0,     0,     0,   104,     0,
       0,    67,     0,   105,   -71,     0,     0,    68,    69,    70,
      71,    72,    93,     0,     0,     0,     0,     4,     5,     6,
       7,     8,     9,    10,    94,     0,     0,    95,    96,    97,
      98,   -69,   -69,    99,   100,   101,     0,   102,   103,     0,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    64,     0,    65,     0,     0,     0,     0,     0,
      66,     0,     0,     0,     0,     0,     0,     0,     0,   104,
       0,     0,    67,     0,   105,   -69,     0,     0,    68,    69,
      70,    71,    72,    93,     0,     0,     0,     0,     4,     5,
       6,     7,     8,     9,    10,    94,     0,     0,    95,    96,
      97,    98,     0,     0,    99,   100,   101,     0,   102,   103,
       0,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,     0,    65,     0,     0,     0,     0,
       0,    66,     0,     0,     0,     0,     0,     0,     0,     0,
     104,     0,     0,    67,     0,   105,   106,     0,     0,    68,
      69,    70,    71,    72,    93,     0,     0,     0,     0,     4,
       5,     6,     7,     8,     9,    10,    94,     0,     0,    95,
      96,    97,    98,     0,     0,    99,   100,   101,     0,   102,
     103,     0,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,     0,    65,     0,     0,     0,
       0,     0,    66,     0,     0,     0,     0,     0,     0,     0,
       0,   104,     0,     0,    67,     0,   105,   174,     0,     0,
      68,    69,    70,    71,    72,    93,     0,     0,     0,     0,
       4,     5,     6,     7,     8,     9,    10,    94,     0,     0,
      95,    96,    97,    98,     0,     0,    99,   100,   101,     0,
     102,   103,     0,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    64,     0,    65,     0,     0,
       0,     0,     0,    66,     0,     0,     0,     0,     0,     0,
       0,     0,   104,     0,     0,    67,     0,   105,   227,     0,
       0,    68,    69,    70,    71,    72,    93,     0,     0,     0,
       0,     4,     5,     6,     7,     8,     9,    10,    94,     0,
       0,    95,    96,    97,    98,     0,     0,    99,   100,   101,
       0,   102,   103,     0,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    64,     0,    65,     0,
       0,     0,     0,     0,    66,     0,     0,     0,     0,     0,
       0,     0,     0,   104,     0,     0,    67,     0,   105,   259,
       0,     0,    68,    69,    70,    71,    72,    93,     0,     0,
       0,     0,     4,     5,     6,     7,     8,     9,    10,    94,
       0,     0,    95,    96,    97,    98,     0,     0,    99,   100,
     101,     0,   102,   103,     0,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,     0,    65,
       0,     0,     0,     0,     0,    66,     4,     5,     6,     7,
       8,     9,    10,     0,   104,     0,     0,    67,     0,   105,
       0,     0,     0,    68,    69,    70,    71,    72,     0,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    64,     0,    65,     0,     0,     0,     0,     0,    66,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   216,
       0,    67,     0,     0,     0,     0,     0,    68,    69,    70,
      71,    72,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,     0,    65,     0,     0,     0,
       0,     0,    66,     0,     0,     0,     0,     0,     0,     0,
       0,   168,     0,     0,    67,     0,     0,     0,     0,     0,
      68,    69,    70,    71,    72,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,     0,    65,
       0,     0,     0,     0,     0,    66,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   268,     0,    67,     0,     0,
       0,     0,     0,    68,    69,    70,    71,    72,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    63,
      64,     0,    65,     0,     0,     0,     0,     0,    66,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      67,     0,     0,     0,     0,     0,    68,    69,    70,    71,
      72,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,     0,    65,     0,     0,     0,     0,
       0,    66,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    67,     0,     0,     0,     0,     0,    68,
      69,    70,    71,   136
};

static const yytype_int16 yycheck[] =
{
      41,   110,     0,   172,    39,    25,     0,   140,    58,   142,
      20,    21,    28,    48,    28,    13,    51,    65,    66,    13,
      57,    57,     5,    57,    61,    61,    67,    71,    65,    65,
      61,    65,    26,    27,    65,    67,    68,   170,   171,     6,
       7,     8,     9,    10,    11,    12,     0,    63,    60,    63,
      44,    71,    64,    47,    64,     0,     1,    92,     3,     4,
     101,     6,     7,     8,     9,    10,    11,    12,    60,     1,
      62,     3,     4,    61,     6,     7,     8,     9,    10,    11,
      12,    61,   123,   124,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,   135,    62,    60,    60,    62,    58,
     141,    64,    51,    52,    53,    54,   154,   155,   156,    44,
      45,    46,   245,    58,    60,   148,   149,   150,   151,   228,
     161,   162,   163,   256,   165,   119,    58,    60,    71,    62,
      71,   300,     6,     7,     8,     9,    10,    11,    12,   308,
      63,   250,   177,    60,   253,    62,    66,    67,    55,    56,
      65,   192,     6,     7,     8,     9,    10,    11,    12,    42,
      43,    14,    15,    61,   162,    20,    21,    14,    15,   146,
     147,   280,   213,   282,    61,   216,   152,   153,    50,    61,
      61,    61,    61,    61,    61,   294,    61,   296,    62,   230,
      61,    61,    61,    61,    57,    57,    48,    47,   307,    66,
     309,   310,    58,    61,   313,    66,   315,    17,   249,    66,
      61,   252,    61,   254,    58,    63,    61,    58,    61,    61,
      58,    58,   263,    62,    57,    59,    58,   268,    63,    60,
      66,    13,    62,    62,    62,    62,    62,    62,    62,    62,
      62,    62,    62,    62,   285,    62,   287,    60,    63,     1,
     291,    62,    57,    62,     6,     7,     8,     9,    10,    11,
      12,    13,    61,   304,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    62,    26,    27,    59,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      63,    43,    58,    58,    62,    62,    59,    49,    62,    61,
      58,    62,    59,    62,    59,    61,    58,    27,    62,    61,
      47,    63,    64,    62,    43,    67,    68,    69,    70,    71,
       1,   249,    97,   289,   285,     6,     7,     8,     9,    10,
      11,    12,    13,   145,    -1,    16,    17,    18,    19,    20,
      21,    22,    23,    24,   144,    26,    27,   143,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    -1,    43,   171,    -1,    -1,    -1,    -1,    49,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    58,    -1,    -1,
      61,    -1,    63,    64,    -1,    -1,    67,    68,    69,    70,
      71,     1,    -1,    -1,    -1,    -1,     6,     7,     8,     9,
      10,    11,    12,    13,    -1,    -1,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    -1,    26,    27,    -1,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    -1,    43,    -1,    -1,    -1,    -1,    -1,    49,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    58,    -1,
      -1,    61,    -1,    63,    64,    -1,    -1,    67,    68,    69,
      70,    71,     1,    -1,    -1,    -1,    -1,     6,     7,     8,
       9,    10,    11,    12,    13,    -1,    -1,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    -1,    26,    27,    -1,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    -1,    43,    -1,    -1,    -1,    -1,    -1,
      49,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    58,
      -1,    -1,    61,    -1,    63,    64,    -1,    -1,    67,    68,
      69,    70,    71,     1,    -1,    -1,    -1,    -1,     6,     7,
       8,     9,    10,    11,    12,    13,    -1,    -1,    16,    17,
      18,    19,    -1,    -1,    22,    23,    24,    -1,    26,    27,
      -1,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    -1,    43,    -1,    -1,    -1,    -1,
      -1,    49,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      58,    -1,    -1,    61,    -1,    63,    64,    -1,    -1,    67,
      68,    69,    70,    71,     1,    -1,    -1,    -1,    -1,     6,
       7,     8,     9,    10,    11,    12,    13,    -1,    -1,    16,
      17,    18,    19,    -1,    -1,    22,    23,    24,    -1,    26,
      27,    -1,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    -1,    43,    -1,    -1,    -1,
      -1,    -1,    49,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    58,    -1,    -1,    61,    -1,    63,    64,    -1,    -1,
      67,    68,    69,    70,    71,     1,    -1,    -1,    -1,    -1,
       6,     7,     8,     9,    10,    11,    12,    13,    -1,    -1,
      16,    17,    18,    19,    -1,    -1,    22,    23,    24,    -1,
      26,    27,    -1,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    -1,    43,    -1,    -1,
      -1,    -1,    -1,    49,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    58,    -1,    -1,    61,    -1,    63,    64,    -1,
      -1,    67,    68,    69,    70,    71,     1,    -1,    -1,    -1,
      -1,     6,     7,     8,     9,    10,    11,    12,    13,    -1,
      -1,    16,    17,    18,    19,    -1,    -1,    22,    23,    24,
      -1,    26,    27,    -1,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    -1,    43,    -1,
      -1,    -1,    -1,    -1,    49,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    58,    -1,    -1,    61,    -1,    63,    64,
      -1,    -1,    67,    68,    69,    70,    71,     1,    -1,    -1,
      -1,    -1,     6,     7,     8,     9,    10,    11,    12,    13,
      -1,    -1,    16,    17,    18,    19,    -1,    -1,    22,    23,
      24,    -1,    26,    27,    -1,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    -1,    43,
      -1,    -1,    -1,    -1,    -1,    49,     6,     7,     8,     9,
      10,    11,    12,    -1,    58,    -1,    -1,    61,    -1,    63,
      -1,    -1,    -1,    67,    68,    69,    70,    71,    -1,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    -1,    43,    -1,    -1,    -1,    -1,    -1,    49,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    59,
      -1,    61,    -1,    -1,    -1,    -1,    -1,    67,    68,    69,
      70,    71,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    -1,    43,    -1,    -1,    -1,
      -1,    -1,    49,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    58,    -1,    -1,    61,    -1,    -1,    -1,    -1,    -1,
      67,    68,    69,    70,    71,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    -1,    43,
      -1,    -1,    -1,    -1,    -1,    49,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    59,    -1,    61,    -1,    -1,
      -1,    -1,    -1,    67,    68,    69,    70,    71,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    -1,    43,    -1,    -1,    -1,    -1,    -1,    49,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      61,    -1,    -1,    -1,    -1,    -1,    67,    68,    69,    70,
      71,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    -1,    43,    -1,    -1,    -1,    -1,
      -1,    49,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    61,    -1,    -1,    -1,    -1,    -1,    67,
      68,    69,    70,    71
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     1,     3,     4,     6,     7,     8,     9,    10,    11,
      12,    58,    77,    78,    79,    80,    81,    82,    83,    86,
      87,    58,     5,    71,     0,    79,    61,    61,    25,    71,
      88,    89,    58,    67,    68,    62,    84,    85,    86,    62,
      84,    57,    65,    60,    28,    63,    91,    60,    62,    71,
      91,    62,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    43,    49,    61,    67,    68,
      69,    70,    71,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   122,    66,    67,
      71,    89,    86,     1,    13,    16,    17,    18,    19,    22,
      23,    24,    26,    27,    58,    63,    64,    86,    87,    93,
      94,    95,    96,    97,   101,   104,   105,   106,    85,    28,
      91,    65,    91,    61,    61,    61,    61,    61,    61,    61,
      61,    61,    61,    61,    61,    61,    71,   116,   116,   106,
      61,    65,    57,    48,    47,    50,    55,    56,    51,    52,
      53,    54,    42,    43,    44,    45,    46,    57,    66,    91,
      58,    61,    61,    61,    93,    61,    58,    58,    58,   106,
      61,    61,    92,    58,    64,    95,    58,    86,    66,   106,
     106,   106,   106,   106,   106,   106,   106,   106,   106,   106,
     106,   106,    65,    62,   107,   120,   121,   106,   107,   110,
     111,   112,   113,   113,   114,   114,   114,   114,   115,   115,
     116,   116,   116,    63,    57,   106,    59,    87,   102,   106,
     106,    17,   106,    58,   107,   123,   123,    64,    94,    91,
      60,    62,    62,    62,    62,    62,    62,    62,    62,    62,
      62,    62,    62,   106,    62,    60,    66,    90,   106,    63,
      62,   106,    59,    62,    61,    62,    60,    62,    62,    64,
     106,    66,   107,    60,    64,    90,    95,    59,    59,   106,
      95,   106,    63,   107,    58,    58,    62,   106,    64,    14,
      15,    98,    62,   103,   106,    59,    62,    20,    21,    99,
     100,    61,    95,    14,    15,    95,    62,   103,    58,   106,
      59,    64,   100,   106,    61,    95,    95,    62,    59,    94,
      62,   106,    95,    94,    95,    62,    95
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}

/* Prevent warnings from -Wmissing-prototypes.  */
#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */


/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*-------------------------.
| yyparse or yypush_parse.  |
`-------------------------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{


    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.

       Refer to the stacks thru separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yytoken = 0;
  yyss = yyssa;
  yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */
  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss_alloc, yyss);
	YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:

/* Line 1455 of yacc.c  */
#line 383 "src\\minionlang.y"
    {
            if (syntax_errors == 0) {
                printf("Program parsed successfully.\n");
            }
        ;}
    break;

  case 9:

/* Line 1455 of yacc.c  */
#line 401 "src\\minionlang.y"
    {
            yyerror("Invalid global declaration");
            yyerrok;
        ;}
    break;

  case 10:

/* Line 1455 of yacc.c  */
#line 413 "src\\minionlang.y"
    { printf("  [Line %d] Include directive parsed\n", yylineno); ;}
    break;

  case 11:

/* Line 1455 of yacc.c  */
#line 415 "src\\minionlang.y"
    {
            printf("  [Line %d] Macro definition parsed\n", yylineno);
            free((yyvsp[(2) - (3)].sval));
        ;}
    break;

  case 12:

/* Line 1455 of yacc.c  */
#line 420 "src\\minionlang.y"
    {
            printf("  [Line %d] Macro definition parsed\n", yylineno);
            free((yyvsp[(2) - (3)].sval));
        ;}
    break;

  case 13:

/* Line 1455 of yacc.c  */
#line 425 "src\\minionlang.y"
    {
            printf("  [Line %d] Macro definition parsed\n", yylineno);
            free((yyvsp[(2) - (2)].sval));
        ;}
    break;

  case 14:

/* Line 1455 of yacc.c  */
#line 437 "src\\minionlang.y"
    {
            begin_function_context((yyvsp[(1) - (2)].type), (yyvsp[(2) - (2)].sval));
                current_function_param_count = 0;
            (yyval.type) = (yyvsp[(1) - (2)].type);
            free((yyvsp[(2) - (2)].sval));
        ;}
    break;

  case 15:

/* Line 1455 of yacc.c  */
#line 447 "src\\minionlang.y"
    {
            begin_function_context((yyvsp[(1) - (2)].type), "starta");
                current_function_param_count = 0;
            (yyval.type) = (yyvsp[(1) - (2)].type);
        ;}
    break;

  case 16:

/* Line 1455 of yacc.c  */
#line 456 "src\\minionlang.y"
    {
                register_current_function_signature();
            end_function_context();
            printf("  [Line %d] Main function parsed\n", yylineno);
        ;}
    break;

  case 17:

/* Line 1455 of yacc.c  */
#line 462 "src\\minionlang.y"
    {
                register_current_function_signature();
            end_function_context();
            printf("  [Line %d] Main function with parameters parsed\n", yylineno);
        ;}
    break;

  case 18:

/* Line 1455 of yacc.c  */
#line 468 "src\\minionlang.y"
    {
                register_current_function_signature();
            end_function_context();
            printf("  [Line %d] Function definition parsed\n", yylineno);
        ;}
    break;

  case 19:

/* Line 1455 of yacc.c  */
#line 474 "src\\minionlang.y"
    {
                register_current_function_signature();
            end_function_context();
            printf("  [Line %d] Function with parameters parsed\n", yylineno);
        ;}
    break;

  case 20:

/* Line 1455 of yacc.c  */
#line 480 "src\\minionlang.y"
    {
            if (!is_assignment_compatible((yyvsp[(1) - (6)].type), (yyvsp[(5) - (6)].type))) {
                semantic_error("Function header type and return-indicator type mismatch", current_function_name);
            }
                register_current_function_signature();
            end_function_context();
            printf("  [Line %d] Function with return type indicator parsed\n", yylineno);
        ;}
    break;

  case 21:

/* Line 1455 of yacc.c  */
#line 489 "src\\minionlang.y"
    {
            if (!is_assignment_compatible((yyvsp[(1) - (7)].type), (yyvsp[(6) - (7)].type))) {
                semantic_error("Function header type and return-indicator type mismatch", current_function_name);
            }
                register_current_function_signature();
            end_function_context();
            printf("  [Line %d] Function with parameters and return type parsed\n", yylineno);
        ;}
    break;

  case 24:

/* Line 1455 of yacc.c  */
#line 506 "src\\minionlang.y"
    {
            if (declare_symbol((yyvsp[(2) - (2)].sval), (yyvsp[(1) - (2)].type), 1, 0) == -1) {
                semantic_error("Parameter redeclared", (yyvsp[(2) - (2)].sval));
            }
            if (inside_function && current_function_param_count < MAX_PARAMS) {
                current_function_param_types[current_function_param_count++] = (yyvsp[(1) - (2)].type);
            }
            free((yyvsp[(2) - (2)].sval));
        ;}
    break;

  case 25:

/* Line 1455 of yacc.c  */
#line 516 "src\\minionlang.y"
    {
            if (declare_symbol((yyvsp[(2) - (4)].sval), (yyvsp[(1) - (4)].type), 1, 1) == -1) {
                semantic_error("Parameter redeclared", (yyvsp[(2) - (4)].sval));
            }
            if (inside_function && current_function_param_count < MAX_PARAMS) {
                current_function_param_types[current_function_param_count++] = (yyvsp[(1) - (4)].type);
            }
            free((yyvsp[(2) - (4)].sval));
        ;}
    break;

  case 26:

/* Line 1455 of yacc.c  */
#line 532 "src\\minionlang.y"
    { (yyval.type) = T_INT; current_decl_type = (yyval.type); ;}
    break;

  case 27:

/* Line 1455 of yacc.c  */
#line 533 "src\\minionlang.y"
    { (yyval.type) = T_FLOAT; current_decl_type = (yyval.type); ;}
    break;

  case 28:

/* Line 1455 of yacc.c  */
#line 534 "src\\minionlang.y"
    { (yyval.type) = T_DOUBLE; current_decl_type = (yyval.type); ;}
    break;

  case 29:

/* Line 1455 of yacc.c  */
#line 535 "src\\minionlang.y"
    { (yyval.type) = T_LONG_INT; current_decl_type = (yyval.type); ;}
    break;

  case 30:

/* Line 1455 of yacc.c  */
#line 536 "src\\minionlang.y"
    { (yyval.type) = T_CHAR; current_decl_type = (yyval.type); ;}
    break;

  case 31:

/* Line 1455 of yacc.c  */
#line 537 "src\\minionlang.y"
    { (yyval.type) = T_BOOL; current_decl_type = (yyval.type); ;}
    break;

  case 32:

/* Line 1455 of yacc.c  */
#line 538 "src\\minionlang.y"
    { (yyval.type) = T_VOID; current_decl_type = (yyval.type); ;}
    break;

  case 36:

/* Line 1455 of yacc.c  */
#line 556 "src\\minionlang.y"
    {
            if (declare_symbol((yyvsp[(1) - (1)].sval), current_decl_type, 0, 0) == -1) {
                semantic_error("Variable redeclared", (yyvsp[(1) - (1)].sval));
            } else {
                printf("  [Line %d] Variable declared: %s\n", yylineno, (yyvsp[(1) - (1)].sval));
            }
            free((yyvsp[(1) - (1)].sval));
        ;}
    break;

  case 37:

/* Line 1455 of yacc.c  */
#line 565 "src\\minionlang.y"
    {
            int idx = declare_symbol((yyvsp[(1) - (3)].sval), current_decl_type, 1, 0);
            if (idx == -1) {
                semantic_error("Variable redeclared", (yyvsp[(1) - (3)].sval));
            } else if (!is_assignment_compatible(current_decl_type, (yyvsp[(3) - (3)].type))) {
                semantic_error("Type mismatch in declaration assignment", (yyvsp[(1) - (3)].sval));
            } else {
                printf("  [Line %d] Variable declared and initialized\n", yylineno);
            }
            free((yyvsp[(1) - (3)].sval));
        ;}
    break;

  case 38:

/* Line 1455 of yacc.c  */
#line 577 "src\\minionlang.y"
    {
            int idx = declare_symbol((yyvsp[(1) - (4)].sval), current_decl_type, 1, 1);
            if (idx == -1) {
                semantic_error("Array redeclared", (yyvsp[(1) - (4)].sval));
            } else {
                printf("  [Line %d] Array declared\n", yylineno);
            }
            free((yyvsp[(1) - (4)].sval));
        ;}
    break;

  case 39:

/* Line 1455 of yacc.c  */
#line 587 "src\\minionlang.y"
    {
            int idx = declare_symbol((yyvsp[(1) - (8)].sval), current_decl_type, 1, 1);
            if (idx == -1) {
                semantic_error("Array redeclared", (yyvsp[(1) - (8)].sval));
            } else {
                printf("  [Line %d] Array declared and initialized\n", yylineno);
            }
            free((yyvsp[(1) - (8)].sval));
        ;}
    break;

  case 40:

/* Line 1455 of yacc.c  */
#line 597 "src\\minionlang.y"
    {
            int idx = declare_symbol((yyvsp[(1) - (7)].sval), current_decl_type, 1, 1);
            if (idx == -1) {
                semantic_error("Array redeclared", (yyvsp[(1) - (7)].sval));
            } else {
                printf("  [Line %d] Array declared with inferred size\n", yylineno);
            }
            free((yyvsp[(1) - (7)].sval));
        ;}
    break;

  case 45:

/* Line 1455 of yacc.c  */
#line 624 "src\\minionlang.y"
    { enter_scope(); ;}
    break;

  case 46:

/* Line 1455 of yacc.c  */
#line 629 "src\\minionlang.y"
    { exit_scope(); ;}
    break;

  case 47:

/* Line 1455 of yacc.c  */
#line 631 "src\\minionlang.y"
    { exit_scope(); ;}
    break;

  case 58:

/* Line 1455 of yacc.c  */
#line 649 "src\\minionlang.y"
    {
            yyerror("Invalid statement");
            yyerrok;
        ;}
    break;

  case 60:

/* Line 1455 of yacc.c  */
#line 665 "src\\minionlang.y"
    { printf("  [Line %d] If statement parsed\n", yylineno); ;}
    break;

  case 61:

/* Line 1455 of yacc.c  */
#line 667 "src\\minionlang.y"
    { printf("  [Line %d] If-else statement parsed\n", yylineno); ;}
    break;

  case 62:

/* Line 1455 of yacc.c  */
#line 669 "src\\minionlang.y"
    { printf("  [Line %d] If-else-if chain parsed\n", yylineno); ;}
    break;

  case 63:

/* Line 1455 of yacc.c  */
#line 671 "src\\minionlang.y"
    { printf("  [Line %d] Complete if-else-if-else parsed\n", yylineno); ;}
    break;

  case 64:

/* Line 1455 of yacc.c  */
#line 673 "src\\minionlang.y"
    { printf("  [Line %d] Switch statement parsed\n", yylineno); ;}
    break;

  case 69:

/* Line 1455 of yacc.c  */
#line 688 "src\\minionlang.y"
    { printf("  [Line %d] Case clause parsed\n", yylineno); ;}
    break;

  case 70:

/* Line 1455 of yacc.c  */
#line 690 "src\\minionlang.y"
    { printf("  [Line %d] Empty case clause parsed\n", yylineno); ;}
    break;

  case 71:

/* Line 1455 of yacc.c  */
#line 692 "src\\minionlang.y"
    { printf("  [Line %d] Default clause parsed\n", yylineno); ;}
    break;

  case 72:

/* Line 1455 of yacc.c  */
#line 694 "src\\minionlang.y"
    { printf("  [Line %d] Empty default clause parsed\n", yylineno); ;}
    break;

  case 73:

/* Line 1455 of yacc.c  */
#line 703 "src\\minionlang.y"
    { printf("  [Line %d] While loop parsed\n", yylineno); ;}
    break;

  case 74:

/* Line 1455 of yacc.c  */
#line 705 "src\\minionlang.y"
    { printf("  [Line %d] Do-while loop parsed\n", yylineno); ;}
    break;

  case 75:

/* Line 1455 of yacc.c  */
#line 707 "src\\minionlang.y"
    { printf("  [Line %d] For loop parsed\n", yylineno); ;}
    break;

  case 76:

/* Line 1455 of yacc.c  */
#line 709 "src\\minionlang.y"
    { printf("  [Line %d] For loop (infinite condition) parsed\n", yylineno); ;}
    break;

  case 77:

/* Line 1455 of yacc.c  */
#line 711 "src\\minionlang.y"
    { printf("  [Line %d] For loop (no init/update) parsed\n", yylineno); ;}
    break;

  case 82:

/* Line 1455 of yacc.c  */
#line 730 "src\\minionlang.y"
    {
            if (!inside_function) {
                semantic_error("Return used outside a function", NULL);
            } else if (current_function_return_type != T_VOID) {
                semantic_error("Non-void function must return a value", current_function_name);
            }
            current_function_has_return = 1;
            printf("  [Line %d] Return statement (void) parsed\n", yylineno);
        ;}
    break;

  case 83:

/* Line 1455 of yacc.c  */
#line 740 "src\\minionlang.y"
    {
            if (!inside_function) {
                semantic_error("Return used outside a function", NULL);
            } else if (current_function_return_type == T_VOID) {
                semantic_error("Void function cannot return a value", current_function_name);
            } else if (!is_assignment_compatible(current_function_return_type, (yyvsp[(2) - (3)].type))) {
                semantic_error("Return type mismatch", current_function_name);
            }
            current_function_has_return = 1;
            printf("  [Line %d] Return statement parsed\n", yylineno);
        ;}
    break;

  case 84:

/* Line 1455 of yacc.c  */
#line 752 "src\\minionlang.y"
    { printf("  [Line %d] Break statement parsed\n", yylineno); ;}
    break;

  case 85:

/* Line 1455 of yacc.c  */
#line 754 "src\\minionlang.y"
    { printf("  [Line %d] Continue statement parsed\n", yylineno); ;}
    break;

  case 86:

/* Line 1455 of yacc.c  */
#line 763 "src\\minionlang.y"
    { printf("  [Line %d] Print statement parsed\n", yylineno); ;}
    break;

  case 87:

/* Line 1455 of yacc.c  */
#line 765 "src\\minionlang.y"
    { printf("  [Line %d] Scan statement parsed\n", yylineno); ;}
    break;

  case 88:

/* Line 1455 of yacc.c  */
#line 774 "src\\minionlang.y"
    { (yyval.type) = (yyvsp[(1) - (1)].type); ;}
    break;

  case 89:

/* Line 1455 of yacc.c  */
#line 779 "src\\minionlang.y"
    { (yyval.type) = (yyvsp[(1) - (1)].type); ;}
    break;

  case 90:

/* Line 1455 of yacc.c  */
#line 781 "src\\minionlang.y"
    {
            int idx = find_symbol((yyvsp[(1) - (3)].sval));
            if (idx < 0) {
                semantic_error("Assignment to undeclared variable", (yyvsp[(1) - (3)].sval));
                (yyval.type) = T_ERR;
            } else {
                if (!is_assignment_compatible(symbols[idx].type, (yyvsp[(3) - (3)].type))) {
                    semantic_error("Type mismatch in assignment", (yyvsp[(1) - (3)].sval));
                }
                symbols[idx].initialized = 1;
                (yyval.type) = symbols[idx].type;
                printf("  [Line %d] Assignment expression parsed\n", yylineno);
            }
            free((yyvsp[(1) - (3)].sval));
        ;}
    break;

  case 91:

/* Line 1455 of yacc.c  */
#line 800 "src\\minionlang.y"
    { (yyval.sval) = (yyvsp[(1) - (1)].sval); ;}
    break;

  case 92:

/* Line 1455 of yacc.c  */
#line 802 "src\\minionlang.y"
    {
            int idx = find_symbol((yyvsp[(1) - (4)].sval));
            if (idx < 0) {
                semantic_error("Array used before declaration", (yyvsp[(1) - (4)].sval));
            } else if (!symbols[idx].is_array) {
                semantic_error("Indexed access on non-array variable", (yyvsp[(1) - (4)].sval));
            }
            (yyval.sval) = (yyvsp[(1) - (4)].sval);
        ;}
    break;

  case 93:

/* Line 1455 of yacc.c  */
#line 815 "src\\minionlang.y"
    { (yyval.type) = (yyvsp[(1) - (1)].type); ;}
    break;

  case 94:

/* Line 1455 of yacc.c  */
#line 817 "src\\minionlang.y"
    {
            if (!is_numeric((yyvsp[(1) - (3)].type)) || !is_numeric((yyvsp[(3) - (3)].type))) {
                semantic_error("Logical OR expects numeric/bool operands", NULL);
                (yyval.type) = T_ERR;
            } else {
                (yyval.type) = T_BOOL;
            }
        ;}
    break;

  case 95:

/* Line 1455 of yacc.c  */
#line 829 "src\\minionlang.y"
    { (yyval.type) = (yyvsp[(1) - (1)].type); ;}
    break;

  case 96:

/* Line 1455 of yacc.c  */
#line 831 "src\\minionlang.y"
    {
            if (!is_numeric((yyvsp[(1) - (3)].type)) || !is_numeric((yyvsp[(3) - (3)].type))) {
                semantic_error("Logical AND expects numeric/bool operands", NULL);
                (yyval.type) = T_ERR;
            } else {
                (yyval.type) = T_BOOL;
            }
        ;}
    break;

  case 97:

/* Line 1455 of yacc.c  */
#line 843 "src\\minionlang.y"
    { (yyval.type) = (yyvsp[(1) - (1)].type); ;}
    break;

  case 98:

/* Line 1455 of yacc.c  */
#line 845 "src\\minionlang.y"
    {
            if (!is_numeric((yyvsp[(1) - (3)].type)) || !is_numeric((yyvsp[(3) - (3)].type))) {
                semantic_error("Logical XOR expects numeric/bool operands", NULL);
                (yyval.type) = T_ERR;
            } else {
                (yyval.type) = T_BOOL;
            }
        ;}
    break;

  case 99:

/* Line 1455 of yacc.c  */
#line 857 "src\\minionlang.y"
    { (yyval.type) = (yyvsp[(1) - (1)].type); ;}
    break;

  case 100:

/* Line 1455 of yacc.c  */
#line 859 "src\\minionlang.y"
    { (yyval.type) = T_BOOL; ;}
    break;

  case 101:

/* Line 1455 of yacc.c  */
#line 861 "src\\minionlang.y"
    { (yyval.type) = T_BOOL; ;}
    break;

  case 102:

/* Line 1455 of yacc.c  */
#line 866 "src\\minionlang.y"
    { (yyval.type) = (yyvsp[(1) - (1)].type); ;}
    break;

  case 103:

/* Line 1455 of yacc.c  */
#line 868 "src\\minionlang.y"
    { (yyval.type) = T_BOOL; ;}
    break;

  case 104:

/* Line 1455 of yacc.c  */
#line 870 "src\\minionlang.y"
    { (yyval.type) = T_BOOL; ;}
    break;

  case 105:

/* Line 1455 of yacc.c  */
#line 872 "src\\minionlang.y"
    { (yyval.type) = T_BOOL; ;}
    break;

  case 106:

/* Line 1455 of yacc.c  */
#line 874 "src\\minionlang.y"
    { (yyval.type) = T_BOOL; ;}
    break;

  case 107:

/* Line 1455 of yacc.c  */
#line 879 "src\\minionlang.y"
    { (yyval.type) = (yyvsp[(1) - (1)].type); ;}
    break;

  case 108:

/* Line 1455 of yacc.c  */
#line 881 "src\\minionlang.y"
    {
            (yyval.type) = merge_numeric_type((yyvsp[(1) - (3)].type), (yyvsp[(3) - (3)].type));
            if ((yyval.type) == T_ERR) semantic_error("Invalid operands for addition", NULL);
        ;}
    break;

  case 109:

/* Line 1455 of yacc.c  */
#line 886 "src\\minionlang.y"
    {
            (yyval.type) = merge_numeric_type((yyvsp[(1) - (3)].type), (yyvsp[(3) - (3)].type));
            if ((yyval.type) == T_ERR) semantic_error("Invalid operands for subtraction", NULL);
        ;}
    break;

  case 110:

/* Line 1455 of yacc.c  */
#line 894 "src\\minionlang.y"
    { (yyval.type) = (yyvsp[(1) - (1)].type); ;}
    break;

  case 111:

/* Line 1455 of yacc.c  */
#line 896 "src\\minionlang.y"
    {
            (yyval.type) = merge_numeric_type((yyvsp[(1) - (3)].type), (yyvsp[(3) - (3)].type));
            if ((yyval.type) == T_ERR) semantic_error("Invalid operands for multiplication", NULL);
        ;}
    break;

  case 112:

/* Line 1455 of yacc.c  */
#line 901 "src\\minionlang.y"
    {
            (yyval.type) = merge_numeric_type((yyvsp[(1) - (3)].type), (yyvsp[(3) - (3)].type));
            if ((yyval.type) == T_ERR) semantic_error("Invalid operands for division", NULL);
        ;}
    break;

  case 113:

/* Line 1455 of yacc.c  */
#line 906 "src\\minionlang.y"
    {
            if (!is_numeric((yyvsp[(1) - (3)].type)) || !is_numeric((yyvsp[(3) - (3)].type))) {
                semantic_error("Modulus expects numeric operands", NULL);
                (yyval.type) = T_ERR;
            } else {
                (yyval.type) = T_INT;
            }
        ;}
    break;

  case 114:

/* Line 1455 of yacc.c  */
#line 918 "src\\minionlang.y"
    { (yyval.type) = (yyvsp[(1) - (1)].type); ;}
    break;

  case 115:

/* Line 1455 of yacc.c  */
#line 920 "src\\minionlang.y"
    {
            if (!is_numeric((yyvsp[(2) - (2)].type))) {
                semantic_error("Logical NOT expects numeric/bool operand", NULL);
                (yyval.type) = T_ERR;
            } else {
                (yyval.type) = T_BOOL;
            }
        ;}
    break;

  case 116:

/* Line 1455 of yacc.c  */
#line 929 "src\\minionlang.y"
    {
            if (!is_numeric((yyvsp[(2) - (2)].type))) {
                semantic_error("Unary minus expects numeric operand", NULL);
                (yyval.type) = T_ERR;
            } else {
                (yyval.type) = (yyvsp[(2) - (2)].type);
            }
        ;}
    break;

  case 117:

/* Line 1455 of yacc.c  */
#line 941 "src\\minionlang.y"
    { (yyval.type) = (yyvsp[(1) - (1)].type); ;}
    break;

  case 118:

/* Line 1455 of yacc.c  */
#line 943 "src\\minionlang.y"
    {
            int idx = find_symbol((yyvsp[(1) - (4)].sval));
            if (idx < 0) {
                semantic_error("Array used before declaration", (yyvsp[(1) - (4)].sval));
                (yyval.type) = T_ERR;
            } else if (!symbols[idx].is_array) {
                semantic_error("Indexed access on non-array variable", (yyvsp[(1) - (4)].sval));
                (yyval.type) = T_ERR;
            } else {
                (yyval.type) = symbols[idx].type;
                printf("  [Line %d] Array access parsed\n", yylineno);
            }
            free((yyvsp[(1) - (4)].sval));
        ;}
    break;

  case 119:

/* Line 1455 of yacc.c  */
#line 958 "src\\minionlang.y"
    { (yyval.type) = (yyvsp[(1) - (1)].type); ;}
    break;

  case 120:

/* Line 1455 of yacc.c  */
#line 963 "src\\minionlang.y"
    {
            int idx = find_symbol((yyvsp[(1) - (1)].sval));
            if (idx < 0) {
                semantic_error("Variable used before declaration", (yyvsp[(1) - (1)].sval));
                (yyval.type) = T_ERR;
            } else {
                if (!symbols[idx].initialized) {
                    semantic_error("Variable may be uninitialized", (yyvsp[(1) - (1)].sval));
                }
                (yyval.type) = symbols[idx].type;
            }
            free((yyvsp[(1) - (1)].sval));
        ;}
    break;

  case 121:

/* Line 1455 of yacc.c  */
#line 977 "src\\minionlang.y"
    { (yyval.type) = T_INT; ;}
    break;

  case 122:

/* Line 1455 of yacc.c  */
#line 979 "src\\minionlang.y"
    { (yyval.type) = T_FLOAT; ;}
    break;

  case 123:

/* Line 1455 of yacc.c  */
#line 981 "src\\minionlang.y"
    {
            (yyval.type) = T_STRING;
            free((yyvsp[(1) - (1)].sval));
        ;}
    break;

  case 124:

/* Line 1455 of yacc.c  */
#line 986 "src\\minionlang.y"
    { (yyval.type) = T_CHAR; ;}
    break;

  case 125:

/* Line 1455 of yacc.c  */
#line 988 "src\\minionlang.y"
    { (yyval.type) = (yyvsp[(2) - (3)].type); ;}
    break;

  case 126:

/* Line 1455 of yacc.c  */
#line 997 "src\\minionlang.y"
    {
            printf("  [Line %d] Function call parsed\n", yylineno);
            (yyval.type) = validate_function_call((yyvsp[(1) - (4)].sval));
            free((yyvsp[(1) - (4)].sval));
        ;}
    break;

  case 127:

/* Line 1455 of yacc.c  */
#line 1003 "src\\minionlang.y"
    { (yyval.type) = (yyvsp[(1) - (1)].type); ;}
    break;

  case 128:

/* Line 1455 of yacc.c  */
#line 1008 "src\\minionlang.y"
    {
            reset_call_args();
            (yyval.type) = T_VOID;
        ;}
    break;

  case 129:

/* Line 1455 of yacc.c  */
#line 1013 "src\\minionlang.y"
    {
            (yyval.type) = T_VOID;
        ;}
    break;

  case 130:

/* Line 1455 of yacc.c  */
#line 1020 "src\\minionlang.y"
    {
            reset_call_args();
            push_call_arg((yyvsp[(1) - (1)].type));
            (yyval.type) = T_VOID;
        ;}
    break;

  case 131:

/* Line 1455 of yacc.c  */
#line 1026 "src\\minionlang.y"
    {
            push_call_arg((yyvsp[(3) - (3)].type));
            (yyval.type) = T_VOID;
        ;}
    break;

  case 132:

/* Line 1455 of yacc.c  */
#line 1034 "src\\minionlang.y"
    { printf("  [Line %d] Power function call parsed\n", yylineno); (yyval.type) = T_DOUBLE; ;}
    break;

  case 133:

/* Line 1455 of yacc.c  */
#line 1036 "src\\minionlang.y"
    { printf("  [Line %d] Square root function call parsed\n", yylineno); (yyval.type) = T_DOUBLE; ;}
    break;

  case 134:

/* Line 1455 of yacc.c  */
#line 1038 "src\\minionlang.y"
    { printf("  [Line %d] Floor function call parsed\n", yylineno); (yyval.type) = T_DOUBLE; ;}
    break;

  case 135:

/* Line 1455 of yacc.c  */
#line 1040 "src\\minionlang.y"
    { printf("  [Line %d] Ceil function call parsed\n", yylineno); (yyval.type) = T_DOUBLE; ;}
    break;

  case 136:

/* Line 1455 of yacc.c  */
#line 1042 "src\\minionlang.y"
    { printf("  [Line %d] Abs function call parsed\n", yylineno); (yyval.type) = T_INT; ;}
    break;

  case 137:

/* Line 1455 of yacc.c  */
#line 1044 "src\\minionlang.y"
    { printf("  [Line %d] Log function call parsed\n", yylineno); (yyval.type) = T_DOUBLE; ;}
    break;

  case 138:

/* Line 1455 of yacc.c  */
#line 1046 "src\\minionlang.y"
    { printf("  [Line %d] Sine function call parsed\n", yylineno); (yyval.type) = T_DOUBLE; ;}
    break;

  case 139:

/* Line 1455 of yacc.c  */
#line 1048 "src\\minionlang.y"
    { printf("  [Line %d] Cosine function call parsed\n", yylineno); (yyval.type) = T_DOUBLE; ;}
    break;

  case 140:

/* Line 1455 of yacc.c  */
#line 1050 "src\\minionlang.y"
    { printf("  [Line %d] Tangent function call parsed\n", yylineno); (yyval.type) = T_DOUBLE; ;}
    break;

  case 141:

/* Line 1455 of yacc.c  */
#line 1052 "src\\minionlang.y"
    { printf("  [Line %d] Arc sine function call parsed\n", yylineno); (yyval.type) = T_DOUBLE; ;}
    break;

  case 142:

/* Line 1455 of yacc.c  */
#line 1054 "src\\minionlang.y"
    { printf("  [Line %d] Arc cosine function call parsed\n", yylineno); (yyval.type) = T_DOUBLE; ;}
    break;

  case 143:

/* Line 1455 of yacc.c  */
#line 1056 "src\\minionlang.y"
    { printf("  [Line %d] Arc tangent function call parsed\n", yylineno); (yyval.type) = T_DOUBLE; ;}
    break;

  case 144:

/* Line 1455 of yacc.c  */
#line 1058 "src\\minionlang.y"
    { printf("  [Line %d] Prime check function call parsed\n", yylineno); (yyval.type) = T_BOOL; ;}
    break;



/* Line 1455 of yacc.c  */
#line 3166 "src\\minionlang.tab.c"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined(yyoverflow) || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}



/* Line 1675 of yacc.c  */
#line 1066 "src\\minionlang.y"


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

