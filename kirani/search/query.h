/*
 * author: Thomas Yao
 */

#ifndef _QUERY_H_
#define _QUERY_H_

#include "scorer.h"
#include "clause.h"
#include "index_searcher.h"

#include "../index/term_posting.h"
#include "../index/index_reader.h"

#define QUERY_DEFAULT_MIN_SIMILARITY 0.5f

struct _query
{
  float boost;
  //struct _clauses* clauses;
  struct _term* tm;

  struct _weight
  {
    struct _index_searcher* is ;
    float value;
    float idf;
    float queryNorm;
    float queryWeight;
  } *wt;
};

struct _query*
query_initial(struct _term* _tm);

struct _weight*
query_weight(struct _query* _qry, struct _index_searcher* _is);

struct _query*
query_rewrite(struct _query* _qry, struct _index_reader* _ir);

struct _weight*
weight_initial(struct _index_searcher* _is);

float
weight_sum_of_square_weights(struct _query* _qry);

void
weight_normalize(struct _query* _qry, float _query_norm);

struct _scorer*
weight_scorer(struct _query* _qry, struct _weight* _wt,
    struct _index_reader* _ir);

#endif

