/*
 *      Author: Thomas Yao
 */

#include <err.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "ken_stream.h"
#include "token.h"

struct _ken_stream*
ken_stream_initial(FILE* _f)
{
  struct _ken_stream* ks = (struct _ken_stream*) malloc(
      sizeof(struct _ken_stream));
  if (ks == NULL)
    err(1, "ks is null");
  ks->ks_begin_column = 0;
  ks->ks_end_column = 0;
  ks->f = _f;
  return ks;
}

struct _token*
ks_next(struct _ken_stream* ks)
{
  char* kslval = kslex(ks);
  if (kslval == NULL)
    return NULL;
  struct _token* tk = token_initial2(kslval, ks->ks_begin_column,
      ks->ks_end_column, "word");
  return tk;
}

char*
kslex(struct _ken_stream* ks)
{
  int i = fs_read_byte(ks->f);
  if (i == 'a')
    printf("i is %c", i);
  return NULL;
}

