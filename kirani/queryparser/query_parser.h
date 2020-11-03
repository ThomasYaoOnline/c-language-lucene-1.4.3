/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton interface for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

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
     AND = 258,
     OR = 259,
     NOT = 260,
     PLUS = 261,
     MINUS = 262,
     LPAREN = 263,
     RPAREN = 264,
     COLON = 265,
     CARAT = 266,
     QUOTED = 267,
     TERM = 268,
     FUZZY_SLOP = 269,
     PREFIXTERM = 270,
     WILDTERM = 271,
     RANGEIN_START = 272,
     RANGEEX_START = 273,
     NUMBER = 274,
     RANGEIN_TO = 275,
     RANGEIN_END = 276,
     RANGEIN_QUOTED = 277,
     RANGEIN_GOOP = 278,
     RANGEEX_TO = 279,
     RANGEEX_END = 280,
     RANGEEX_QUOTED = 281,
     RANGEEX_GOOP = 282
   };
#endif
/* Tokens.  */
#define AND 258
#define OR 259
#define NOT 260
#define PLUS 261
#define MINUS 262
#define LPAREN 263
#define RPAREN 264
#define COLON 265
#define CARAT 266
#define QUOTED 267
#define TERM 268
#define FUZZY_SLOP 269
#define PREFIXTERM 270
#define WILDTERM 271
#define RANGEIN_START 272
#define RANGEEX_START 273
#define NUMBER 274
#define RANGEIN_TO 275
#define RANGEIN_END 276
#define RANGEIN_QUOTED 277
#define RANGEIN_GOOP 278
#define RANGEEX_TO 279
#define RANGEEX_END 280
#define RANGEEX_QUOTED 281
#define RANGEEX_GOOP 282




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 61 "queryparser/query_parser.yy"
{
  int ival;
  long lval;
  float fval;
  double dval;
  char cval;
  char* sval;
  struct _query* qval;
}
/* Line 1529 of yacc.c.  */
#line 113 "queryparser/query_parser.h"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE qplval;

