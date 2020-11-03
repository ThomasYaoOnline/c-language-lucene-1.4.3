/*
 * author: Thomas Yao
 */

#include <err.h>
#include <stdlib.h>
#include <stdio.h>

#include "../index/term_docs.h"

#include "scorer.h"

struct _scorer*
scorer_initial(struct _weight* _wt, struct _term_docs* _td,
    struct _similarity* _sml, struct _int_list* _norms)
{
  struct _scorer* sco = (struct _scorer*) malloc(sizeof(struct _scorer));
  if (sco == NULL)
    err(1, "sco is null");

  sco->sml = _sml;
  sco->weight = _wt;
  sco->termdocs = _td;
  sco->norms = _norms;
  sco->weightvalue = _wt->value;
  sco->docs = int_list_initial();
  sco->freqs = int_list_initial();
  sco->scorecache = float_list_initial();

  int i;
  for (i = 0; i < SCORE_CACHE_SIZE; i++)
    {
      int_list_add(sco->docs, 0);
      int_list_add(sco->freqs, 0);
      float_list_add(sco->scorecache, similarity_tf(sco->sml, i)
          * sco->weightvalue);
    }
  return sco;
}


float
scorer_score(struct _scorer* _sco)
{
  int f = int_list_get_index(_sco->freqs, _sco->pointer);
  float raw = // compute tf(f)*weight
      f < SCORE_CACHE_SIZE // check cache
      ? float_list_get_index(_sco->scorecache, f) // cache hit
          : similarity_tf(_sco->sml, f) * _sco->weightvalue; // cache miss

  return raw * similarity_decode_norm(
      int_list_get_index(_sco->norms, _sco->doc)); // normalize for field
}

int
scorer_next(struct _scorer* _sco)
{
  _sco->pointer++;
  printf("scorer_next:sco->pointer: %i\n", _sco->pointer);
  printf("scorer_next:sco->pointer_max: %i\n", _sco->pointer_max);
  if (_sco->pointer >= _sco->pointer_max)
    {
      _sco->pointer_max = term_docs_read(_sco->termdocs, _sco->docs,
          _sco->freqs); // refill buffer
      if (_sco->pointer_max != 0)
        {
          _sco->pointer = 0;
        }
      else
        {
          term_docs_close(_sco->termdocs); // close stream
          _sco->doc = 0x7fffffff; // set to sentinel value
          return 0;
        }
    }
  _sco->doc = int_list_get_index(_sco->docs, _sco->pointer);
  return 1;
}

