LETTER      [\x41-\x5a\x61-\x7a\xc0-\xd6\xd8-\xf6\xf8-\xff]
DIGET       [\x30-\x39\x660-\x669\x6f0-\x6f9\x966-\x96f\x9e6-\x9ef\xa66-\xa6f\xae6-\xaef\xb66-\xb6f\xbe7-\xbef\xc66-\xc6f\xce6-\xcef\xd66-\xd6f\xe50-\xe59\xed0-\ued9]
CJK         \xe4\xb8[\x80-\xa2]
P           [\_\-\/\.\,]
HAS_DIGET   ({LETTER}|{DIGET})*{DIGET}({LETTER}|{DIGET})*
_ALPHA       {LETTER}+
_ALPHANUM   ({LETTER}|{DIGET})+
_WHITESPACE       [\ \t\r\n]

%{
#include <err.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

#include "token.h"
#include "token_stream.h"

#include "../kirani.h"
#include "../utils/utils.h"
#include "../store/file_stream.h"

//CJK         ['\u3040'-'\u318f','\u3300'-'\u337f','\u3400'-'\u3d2d','\u4e00'-'\u9fff','\uf900'-'\ufaff']

//CJK         [\x3040-\x318f\x3300-\x337f\x3400-\x3d2d\x4e00-\x9fff\xf900-\xfaff]
 //_ALPHANUM                {  st_echo("ALPHANUM", yytext);  stlval.sval = strdup(yytext);  return ALPHANUM;  }
 //{_ALPHA}(\'{_ALPHA})+      {  st_echo("APOSTROPHE", yytext);  stlval.sval = strdup(yytext);  return APOSTROPHE;  }
 //{_ALPHA}\.({_ALPHA}\.)+     {  st_echo("ACRONYM", yytext);  stlval.sval = strdup(yytext);  return ACRONYM;  }
 //{_ALPHANUM}((\.|\-|\_){_ALPHANUM})*\@{_ALPHANUM}((\.|\-){_ALPHANUM})+     {  st_echo("EMAIL", yytext);  stlval.sval = strdup(yytext);  return EMAIL;  }
 //{_ALPHA}(\&|\@){_ALPHA}     {  st_echo("COMPANY", yytext);  stlval.sval = strdup(yytext);  return COMPANY;  }
 //{_ALPHANUM}(\.{_ALPHANUM})+   {  st_echo("HOST", yytext);  stlval.sval = strdup(yytext);  return HOST;  }

//{_ALPHANUM}    {  
//                 tslval = strdup(yytext);
//                 ts_echo("alphanum", yytext);  
//                 ts_begin_column = ts_end_column;   
//                 ts_end_column += yyleng;  
//                 return;  
//               }
//  [^{_ALPHANUM}] {}
%}

%%


{CJK}          { 
                 tslval = strdup(yytext);
                 ts_echo("cjk", yytext);
                 ts_begin_column = ts_end_column;
                 ts_end_column += yyleng;
                 return;
               }


%%

void ts_echo(char* name, char* text)
{
  printf("%s: %s\n", name, text);
}

int yywrap()
{
  return 1;
}

struct _token* ts_next()
{
  int i = yylex();
  if (i == 0)
    return NULL;
  struct _token* tk = token_initial2(tslval, ts_begin_column, ts_end_column, "word");
  return tk;
}

int ts_run()
{
  struct _file_stream* fsin = fs_initial("/root/docs/books", "r");
  struct _token_stream* ts = ts_initial(fsin);
  struct _token* tk;
  for (tk = ts_next(); tk != NULL; tk = ts_next())
    {
      printf("text: %s\n", tk->term_text);
    }
}


struct _token_stream*
ts_initial(struct _file_stream* fsin)
{
  struct _token_stream* ts = (struct _token_stream*) malloc(
      sizeof(struct _token_stream));
  if (ts == NULL)
    err(1, "ts is null");

  yyin = fsin->stream;
  ts_begin_column = 0;
  ts_end_column = 0; 
  return ts;
}

struct _char_list*
ts_make_stop_list(char* _stop_words[33], int _size, int _ignore_case)
{
  if (_ignore_case)
    {
      int i;
      for (i = 0; i < _size; i++)
        utils_tolower(_stop_words[i]);
    }
  return char_list_initial_by_array(_stop_words, _size);
}


