/*
 * author: Thomas Yao
 */

#include <err.h>
#include <stdio.h>
#include <stdlib.h>

#include "token.h"

struct _token*
token_initial()
{
  struct _token* tk = (struct _token*) malloc(sizeof(struct _token));
  if (tk == NULL)
    err(1, "tk is null");
  tk->type = "word";
  tk->position_increment = 1;

  return tk;
}

struct _token*
token_initial1(char* text, int start, int end)
{
  struct _token* tk = token_initial();
  tk->term_text = text;
  tk->start_offset = start;
  tk->end_offset = end;
  return tk;
}

struct _token*
token_initial2(char* text, int start, int end, char* type)
{
  struct _token* tk = token_initial();
  tk->term_text = text;
  tk->start_offset = start;
  tk->end_offset = end;
  tk->type = type;
  return tk;
}

