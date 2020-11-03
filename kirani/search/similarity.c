/*
 * author: Thomas Yao
 */

#include <err.h>
#include <stdlib.h>
#include <stdio.h>

#include <math.h>

#include "similarity.h"

struct _similarity*
similarity_initial()
{
  struct _similarity* sml = (struct _similarity*) malloc(
      sizeof(struct _similarity));
  return sml;
}

float
similarity_decode_norm(int b)
{
  return (float) b;
}

float
similarity_query_norm(struct _similarity* _sml, float _sum_of_squared_weights)
{
  return 1.0 / sqrtf(_sum_of_squared_weights);
}

float
similarity_length_norm(struct _similarity* _sml, char* fiend_name,
    float num_terms)
{
  return (float) 1.0 / (num_terms * num_terms);
}

float
similarity_tf(struct _similarity* _sml, float freq)
{
  return sqrtf(freq);
}

float
similarity_coord(int _over_lap, int _max_over_lap)
{
  return _over_lap / (float) _max_over_lap;
}

float
similarity_idf(struct _term* _tm, struct _index_searcher* _is)
{
  int doc_freq = index_reader_doc_freq(_is->ir, _tm);
  int num_docs = index_reader_max_doc(_is->ir);
  return (float) (log(num_docs / (double) (doc_freq + 1)) + 1.0);
}

