/*
* author: Thomas Yao
*/

#include <err.h>
#include <stdlib.h>

#include "term_vector.h"

struct _term_vector*
term_vector_initial(char* _field,
                    char* _tms[],
                    int _term_freqs[])
{
  struct _term_vector* tv = (struct _term_vector*) malloc(sizeof(
      struct _term_vector));

  if (tv == NULL)
    err(1, "tv is null");

  tv->field = _field;
  //tv->terms = _tms;
  //tv->term_freqs = _term_freqs;

  return tv;
}



