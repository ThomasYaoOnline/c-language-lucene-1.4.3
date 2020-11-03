/*
 * author: Thomas Yao
 */

#ifndef _SCORER_H_
#define _SCORER_H_

#include "similarity.h"
#include "../utils/float_list.h"

#define SCORE_CACHE_SIZE 32

struct _scorer
{
  struct _similarity* sml;

  struct _weight* weight;
  struct _term_docs* termdocs;
  struct _int_list* norms;
  float weightvalue;
  int doc;

  struct _int_list* docs; //32ge	  // buffered doc numbers
  struct _int_list* freqs; //32	  // buffered term freqs
  int pointer;
  int pointer_max;

  struct _float_list* scorecache;
};

struct _scorer*
scorer_initial(struct _weight* _wt, struct _term_docs* _td,
    struct _similarity* _sml, struct _int_list* _norms);

int
scorer_next(struct _scorer* _sco);

float
scorer_score(struct _scorer* _sco);

#endif

