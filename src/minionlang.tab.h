
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton interface for Bison's Yacc-like parsers in C
   
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

/* Line 1676 of yacc.c  */
#line 295 "src\\minionlang.y"

    int ival;
    double fval;
    char cval;
    char* sval;
    int type;



/* Line 1676 of yacc.c  */
#line 137 "src\\minionlang.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


