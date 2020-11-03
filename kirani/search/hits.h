/*
 * author: Thomas Yao
 */

#ifndef _HITS_H_
#define _HITS_H_

#include "query.h"
#include "index_searcher.h"
#include "filter.h"
#include "sort.h"

#include "../document/document.h"

struct _hit_doc
{
  float score;
  int id;
  struct _document* doc;
  struct _hit_doc* next;
  struct _hit_doc* prev;
};

struct _hit_doc*
hit_doc_initial(float _score, int _id);

struct _hit_docs_node
{
  struct _hit_docs_node* next;
  struct _hit_docs_node* prev;
  struct _hit_doc* data;
  int index;
};

struct _hit_docs
{
  struct _hit_docs_node* curr;
  int size;
};

struct _hit_docs*
hit_docs_initial();

void
hit_docs_add(struct _hit_docs* _s, struct _hit_doc* _data);

struct _hit_doc*
hit_docs_get_curr(struct _hit_docs* _s);

struct _hit_doc*
hit_docs_get_index(struct _hit_docs* s, int _index);

void
hit_docs_rm_curr(struct _hit_docs* _s);

void
hit_docs_destory(struct _hit_docs* _s);

void
hit_docs_clear(struct _hit_docs* _s);

void
hit_docs_print(struct _hit_docs* _s);

#include "query.h"
#include "index_searcher.h"
#include "filter.h"

struct _hits
{
  struct _query* query;
  struct _index_searcher* is;
  struct _filter* filter;
  struct _sort* sort;
  int length;
  struct _hit_docs* hit_docs;
  struct _hit_doc* first;
  struct _hit_doc* last;
  int num_docs;
  int max_docs;
};

struct _hits*
hits_initial(struct _index_searcher* _sc, struct _query* _qry,
    struct _filter* _flt);

void
hits_get_more_docs(struct _hits* _ht, int _min);

struct _document*
hits_doc(struct _hits* hits, int n);

#endif
