_NUM_CHAR         [0-9]
_ESCAPED_CHAR     [\\\+\-\!\(\)\:\^\[\]\"\{\}\~\*\?]
_TERM_START_CHAR  [^\ \t\n\r\+\-\!\(\)\:\^\[\]\"\{\}\~\*\?]
_TERM_CHAR        {_TERM_START_CHAR}
_WHITESPACE       [\ \t\r\n]

%{
#undef YY_INPUT
#define YY_INPUT(b, r, ms)  (r = qp_yyinput(b, ms))

#include <string.h>
#include <math.h>

#include "../kirani.h"
#include "query_parser.h"

void qp_echo(char* name, char* text);
int qp_yyinput(char *buf, int max_size);

 //\~({_NUM_CHAR}+(\.{_NUM_CHAR}+)?)?  (  qp_echo("FUZZY_SLOP", yytext); qplval.sval = strdup(yytext);  return FUZZY_SLOP;  }
 //{_TERM_START_CHAR}({_TERM_CHAR})*"*" {  qp_echo("PREFIXTERM", yytext); qplval.sval = strdup(yytext);  return PREFIXTERM;  }
 //{_TERM_START_CHAR}({_TERM_CHAR}|[\?\*])*   {  qp_echo("WILDTERM", yytext); qplval.sval = strdup(yytext);  return WILDTERM;  }
 //"["          {  qp_echo("RANGEIN_START", yytext); qplval.sval = strdup(yytext);  return RANGEIN_START;   }
 //"{"          {  qp_echo("RANGEEX_START", yytext); qplval.sval = strdup(yytext);  return RANGEEX_START;   }
 //"TO"         {  qp_echo("RANGEIN_TO", yytext); qplval.sval = strdup(yytext);  return RANGEIN_TO;  }
 //"]"          {  qp_echo("RANGEIN_END", yytext); qplval.sval = strdup(yytext);  return RANGEIN_END;  }
 //[^\ \]]+     {  qp_echo("RANGEIN_GOOP", yytext); qplval.sval = strdup(yytext);  return RANGEIN_GOOP;  }
 //"TO"         {  qp_echo("RANGEEX_TO", yytext); qplval.sval = strdup(yytext);  return RANGEEX_TO;  }
 //"}"          {  qp_echo("RANGEEX_END", yytext); qplval.sval = strdup(yytext);  return RANGEEX_END;  }
 //[^\ \}]+     {  qp_echo("RANGEEX_GOOP", yytext); qplval.sval = strdup(yytext);  return RANGEEX_GOOP;  }

%}

%%

"AND"|"&&"   {  qp_echo("AND", yytext);  qplval.sval = strdup(yytext);  return AND;             }
"OR"|"||"    {  qp_echo("OR",  yytext);  qplval.sval = strdup(yytext);  return OR;              }
"NOT"|"!"    {  qp_echo("NOT", yytext);  qplval.sval = strdup(yytext);  return NOT;             }
"+"          {  qp_echo("PLUS", yytext);  qplval.sval = strdup(yytext);  return PLUS;            }
"-"          {  qp_echo("MINUS", yytext); qplval.sval = strdup(yytext);  return MINUS;           }
"("          {  qp_echo("LPAREN", yytext); qplval.sval = strdup(yytext);  return LPAREN;          }
")"          {  qp_echo("RPAREN", yytext); qplval.sval = strdup(yytext);  return RPAREN;          }
":"          {  qp_echo("COLON", yytext); qplval.sval = strdup(yytext);  return COLON;           }
"^"          {  qp_echo("CARAT", yytext); qplval.sval = strdup(yytext);  return CARAT;           }

\"[^\"]+\"   {  qp_echo("QUOTED", yytext);  qplval.sval = strdup(yytext);  return QUOTED;        }
 
{_NUM_CHAR}+([\.]{_NUM_CHAR}+)?     {  qp_echo("NUMBER", yytext); qplval.sval = strdup(yytext);  return NUMBER;  }
{_TERM_START_CHAR}({_TERM_CHAR})*          {  qp_echo("TERM", yytext); qplval.sval = strdup(yytext);  return TERM;  }

%%
void qp_echo(char* name, char* text)
{
  //printf("%s: %s\n", name, text);
}


int qp_yyinput(char *buf, int max_size)
{
  int n = qpinputlim - qpinputptr;
  //printf("n: %d\n", n);
  if (n > 0)
    {
      memcpy(buf, qpinputptr, n);
      //printf("buf: %s\n", buf);
      qpinputptr += n;
    }
  return n;
}

int yywrap()
{
  return 1;
}

