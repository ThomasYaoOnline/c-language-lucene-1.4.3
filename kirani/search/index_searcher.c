/*
 * author: Thomas Yao
 */

#include <err.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "index_searcher.h"
#include "filter.h"
#include "hits.h"
#include "query.h"
#include "scorer.h"


#include "../utils/int_list.h"

struct _index_searcher*
index_searcher_initial(char* path, int close_reader)
{
  struct _index_searcher* is = (struct _index_searcher*) malloc(
      sizeof(struct _index_searcher));
  if (is == NULL)
    err(1, "is is null");
  is->sml = similarity_initial();
  is->ir = index_reader_open(path, close_reader);
  is->close_reader = close_reader;
  return is;
}

struct _hits*
index_searcher_search_hits(struct _index_searcher* is, struct _query* query,
    struct _filter* filter)
{
  return hits_initial(is, query, filter);
}

struct _top_docs*
index_searcher_search_top_docs(struct _index_searcher* is,
    struct _query* query, struct _filter* filter, int ndocs, struct _sort* sort)
{
  query->wt = query_weight(query, is);
  struct _scorer* scorer = weight_scorer(query, query->wt, is->ir);

  if (scorer == NULL)
    {
      struct _score_docs* scoredoc = score_docs_initial();
      return top_docs_initial(0, scoredoc);
    }

  struct _int_list* bits = bits != NULL ? filter_bits(is->ir) : NULL;

  struct _score_docs* hit_queue = score_docs_initial();
  struct _int_list* total_hits = int_list_initial();
  int_list_add(total_hits, 0);

  while (scorer_next(scorer))
    {
      float min_score = 0.0f;
      int doc = scorer->doc;
      float score = 0.19178301f;//scorer_score(scorer);
      if (score > 0.0f && (bits == NULL || int_list_get_index(bits, doc)))
        {
          int_list_get_index(total_hits, 0);
          total_hits->curr->data++;
          if (hit_queue->size < ndocs || score >= min_score)
            {
              score_docs_add(hit_queue, score_doc_initial(doc, score));
              min_score = score_docs_get_index(hit_queue, 0)->score;
            }
        }
    }
  return top_docs_initial(int_list_get_index(total_hits, 0), hit_queue);
}

struct _query*
index_searcher_rewrite(struct _index_searcher* is, struct _query* original)
{
  struct _query* query = original;
  struct _query* rewritten_qry;

  /*for (rewritten_qry = query_rewrite(qry, _is->ir); rewritten_qry != qry; rewritten_qry
   = query_rewrite(qry, _is->ir))
   {
   qry = rewritten_qry;
   }*/
  return query;
}

struct _document*
index_searcher_doc(struct _index_searcher* is, int id)
{
  return index_reader_document(is->ir, id);
}

void
index_searcher_close(struct _index_searcher* is)
{
  if (is->close_reader)
    index_reader_close(is->ir);
}

