/*
 * author: Thomas Yao
 */

#ifndef _TERM_VECTOR_H_
#define _TERM_VECTOR_H_

#include "term_posting.h"

struct _term_vector
{
  char* field;
  char* terms[1024];
  int term_freqs[1024];
};

struct _term_vector*
term_vector_initial(char* _field, char* _tms[], int _term_freqs[]);

#endif

