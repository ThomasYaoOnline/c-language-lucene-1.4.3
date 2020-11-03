%{
#include <stdio.h>
#include <string.h>

#include "../kirani.h"

#include "../analysis/analyzer.h"
#include "../analysis/token.h"
#include "../analysis/token_stream.h"

#include "../index/term_posting.h"

#include "../search/query.h"

#include "../utils/char_list.h"

const int CONJ_NONE = 0;
const int CONJ_AND = 1;
const int CONJ_OR = 2;

const int MOD_NONE = 0;
const int MOD_NOT = 10;
const int MOD_REQ = 11;

const int DEFAULT_OPERATOR_OR = 0;
const int DEFAULT_OPERATOR_AND = 1;

int mods;
int conjs;

 //char* term, boost, fuzzy_slop, goop1, goop2;

 //int prefix;
 //int wild_card;
 //int fuzzy;
 //int rangein;

char* term_image;

struct _query_parser
{
  int operator;
  int lowercase_wildcard_terms;
  struct _analyzer* alz;
  char* field;
  int phrase_slop;
  float fuzzy_min_sim;
  char* locale;
} *qp; 

char* discard_escape_char(char* input);
struct _query* get_field_query(struct _query_parser* qp, char* field, char* query_text);
struct _query_parser* query_parser_initial();

 //print  :  print  {  }
 //       |   
 //       ;
%}

%union
{
  int ival;
  long lval;
  float fval;
  double dval;
  char cval;
  char* sval;
  struct _query* qval;
}

%token <sval> AND
%token <sval> OR
%token <sval> NOT
%token <sval> PLUS
%token <sval> MINUS
%token <sval> LPAREN
%token <sval> RPAREN
%token <sval> COLON
%token <sval> CARAT

%token <sval> QUOTED

%token <sval> TERM
%token <sval> FUZZY_SLOP
%token <sval> PREFIXTERM
%token <sval> WILDTERM


%token <sval> RANGEIN_START
%token <sval> RANGEEX_START

%token <sval> NUMBER

%token <sval> RANGEIN_TO
%token <sval> RANGEIN_END
%token <sval> RANGEIN_QUOTED
%token <sval> RANGEIN_GOOP

%token <sval> RANGEEX_TO
%token <sval> RANGEEX_END
%token <sval> RANGEEX_QUOTED
%token <sval> RANGEEX_GOOP


%type <ival> conjunction modifiers
%type <qval> program query clauses term
%%


program  :  query program 
             {
               q = $1;
             }
         |  query
             {
               q = $1;
             }
         ;

query  :  modifiers clauses
            {  
               mods = $1;
               //printf("modifiers: %d\n", mods);  
               $$ = $2;
            }
       |  conjunction modifiers clauses
            {
               conjs = $1;
               //printf("conjunction: %d\n", conjs);
               mods = $2;
               //printf("modifiers: %d\n", mods);
               $$ = $3;
            }
       ;

conjunction  :  AND  { $$ = CONJ_AND;  }
             |  OR   { $$ = CONJ_OR;   }
             |       { $$ = CONJ_NONE; }
             ;

modifiers  :  PLUS   {  $$ = MOD_REQ;  }
           |  MINUS  {  $$ = MOD_NOT;  }
           |  NOT    {  $$ = MOD_NOT;  }
           |         {  $$ = MOD_NONE; }
           ;

clauses  : term 
            {
              $$ = $1;               
            }
         ;


term  :  TERM 
           {
             term_image = discard_escape_char($1);
             //printf("term_image: %s\n", term_image);
             $$ = get_field_query(qp, "", term_image);             
           }
      ;

 

%%

struct _query* qp_parse(char* _line, char* _field_name, struct _analyzer* _alz)
{ 
  qp = query_parser_initial(_field_name); 
  
  qpinput = strdup(_line);
  qpinputptr = qpinput;
  qpinputlim = qpinput+strlen(qpinput);

  yyparse();
  q->tm->field = _field_name;
  return q;
}

struct _query_parser* query_parser_initial()
{
  struct _query_parser* qp = (struct _query_parser*) malloc(sizeof(struct _query_parser));
  if (qp == NULL)
    err(1, "qp is null");
  qp->operator = DEFAULT_OPERATOR_OR;
  qp->lowercase_wildcard_terms = 1;
  qp->phrase_slop = 0;
  qp->fuzzy_min_sim = QUERY_DEFAULT_MIN_SIMILARITY;
  return qp;
}


char* discard_escape_char(char* input)
{
  static char ca_dest[100];
  int i;
  int j = 0;
  for (i=0; i<strlen(input); i++)
    {
      if ( (input[i] != '\\') || ((i>0) && (input[i-1] == '\\')) )
        {
          ca_dest[j++] = input[i];
        }
    }
  ca_dest[j++] = '\0';
  return ca_dest;
}


struct _query* get_field_query(struct _query_parser* qp, char* field, char* query_text)
{
  //struct _token_stream* source = analyzer_token_stream(qp->alz, field, query_text); 
  struct _char_list* termtexts = char_list_initial();
  struct _token* t;

  return query_initial( term_initial(field, query_text) );
  /*
  while(true)
    {
      t = token_stream_next(source);
      if (t == NULL)
        break;
      char_list_add(termtexts, t->term_text); 
    }
  
  token_stream_close(source);
  
  if (termtexts->size == 0)
    return NULL;
  else if (termtexts->size == 1)
    return NULL;
  else
    return NULL;
  */
}



int qperror(char *msg)
{
  printf("Error encountered: %s \n", msg);
}

