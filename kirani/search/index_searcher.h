/*
 * author: Thomas Yao
 */

#ifndef _INDEX_SEARCHER_H_
#define _INDEX_SEARCHER_H_

#include "filter.h"
#include "sort.h"
#include "similarity.h"
#include "top_docs.h"

#include "../index/index_reader.h"

struct _index_searcher
{
  struct _similarity* sml;
  struct _index_reader* ir;
  int close_reader;
};

#include "query.h"

struct _index_searcher*
index_searcher_initial();

struct _hits*
index_searcher_search_hits(struct _index_searcher* is, struct _query* query,
    struct _filter* filter);

struct _top_docs
    *
    index_searcher_search_top_docs(struct _index_searcher* is,
        struct _query* query, struct _filter* filter, int ndocs,
        struct _sort* sort);

struct _query*
index_searcher_rewrite(struct _index_searcher* is, struct _query* original);

struct _document*
index_searcher_doc(struct _index_searcher* is, int id);

void
index_searcher_close(struct _index_searcher* is);

#endif

