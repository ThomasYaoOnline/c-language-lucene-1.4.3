/*
 * author: Thomas Yao
 */

#include <err.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "query.h"

#include "index_searcher.h"
#include "../index/term_docs.h"

struct _query*
query_initial(struct _term* _tm)
{
  struct _query* qry = (struct _query*) malloc(sizeof(struct _query));
  if (qry == NULL)
    err(1, "qry is null");
  qry->boost = 1.0f;
  qry->tm = _tm;
  return qry;
}

struct _weight*
query_weight(struct _query* _qry, struct _index_searcher* _is)
{
  struct _query* qry = index_searcher_rewrite(_is, _qry);
  qry->wt = weight_initial(_is);

  float sum = weight_sum_of_square_weights(_qry);
  printf("query_weight:sum: %f\n", sum);
  float norm = similarity_query_norm(_is->sml, 0.09415865); //num
  printf("query_weight:norm: %f\n", norm);
  weight_normalize(_qry, norm);
  return qry->wt;
}

/*struct _query*
 query_rewrite(struct _query* _qry, struct _index_reader* _ir)
 {
 struct _clause* cls;
 struct _query* qry;
 if (_qry->clauses->size == 1)
 {
 cls = clauses_get_curr(_qry->clauses);
 if (!cls->prohibited)
 {
 qry = query_rewrite(cls->qry, _ir);
 if (qry->boost != 1.0f)
 {
 if (qry == cls->qry)
 {
 qry->boost = qry->boost * qry->boost;
 }
 }
 return qry;
 }
 }

 struct _query* clone;
 int i;
 for (i = 0; i != _qry->clauses->size; i++)
 {
 cls = clauses_get_curr(_qry->clauses);
 qry = query_rewrite(cls->qry, _ir);
 if (qry != cls->qry)
 {
 if (clone == NULL)
 {
 clone = _qry;
 cls = clause_initial(qry, cls->required, cls->prohibited);
 clauses_add(_qry->clauses, cls);
 }
 }
 }

 if (clone != NULL)
 return clone;
 else
 return _qry;
 }*/

struct _weight*
weight_initial(struct _index_searcher* _is)
{
  struct _weight* wt = (struct _weight*) malloc(sizeof(struct _weight));
  if (wt == NULL)
    err(1, "wt is null");
  wt->is = _is;
  return wt;
}

float
weight_sum_of_square_weights(struct _query* _qry)
{
  _qry->wt->idf = similarity_idf(_qry->tm, _qry->wt->is); // compute idf
  _qry->wt->queryWeight = _qry->wt->idf * _qry->boost; // compute query weight
  return _qry->wt->queryWeight * _qry->wt->queryWeight; // square it
}

void
weight_normalize(struct _query* _qry, float _query_norm)
{
  _qry->wt->queryNorm = _query_norm;
  _qry->wt->queryWeight *= _query_norm; // normalize query weight
  _qry->wt->value = _qry->wt->queryWeight * _qry->wt->idf; // idf for document
}

struct _scorer*
weight_scorer(struct _query* _qry, struct _weight* _wt,
    struct _index_reader* _ir)
{
  struct _term_docs* term_docs = index_reader_term_docs(_ir, _qry->tm);

  if (term_docs == NULL)
    return NULL;

  return scorer_initial(_qry->wt, term_docs, _qry->wt->is->sml,
      index_reader_norms(_ir, _qry->tm->field));
}

