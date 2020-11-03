/*
 * author: Thomas Yao
 */

#include "sort_comparator_source.h"

#ifndef _SORT_FIELD_H_
#define _SORT_FIELD_H_

#define SORT_FIELD_SCORE 0
#define SORT_FIELD_DOC 1
#define SORT_FIELD_AUTO 2
#define SORT_FIELD_STRING 3
#define SORT_FIELD_INT 4
#define SORT_FIELD_FLOAT 5
#define SORT_FIELD_CUSTOM 9

struct _sort_field
{
  char* field;
  int type;
  //local
  int reverse;
  struct _sort_comparator_source* scs;
};

static const struct _sort_field* sort_field_FIELD_SCORE;
static const struct _sort_field* sort_field_FIELD_DOC;

#endif
