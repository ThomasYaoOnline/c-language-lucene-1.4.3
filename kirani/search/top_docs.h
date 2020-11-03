/*
 * author: Thomas Yao
 */

#ifndef _TOP_DOC_H_
#define _TOP_DOC_H_

#include "../utils/int_list.h"

struct _score_doc
{
  int doc;
  float score;
};

struct _score_doc*
score_doc_initial(int _doc, float _score);

struct _score_docs_node
{
  struct _score_docs_node* next;
  struct _score_docs_node* prev;
  struct _score_doc* data;
  int index;
};

struct _score_docs
{
  struct _score_docs_node* curr;
  int size;
};

struct _score_docs*
score_docs_initial();

void
score_docs_add(struct _score_docs* _s, struct _score_doc* _data);

struct _score_doc*
score_docs_get_curr(struct _score_docs* _s);

struct _score_doc*
score_docs_get_index(struct _score_docs* _s, int _index);

void
score_docs_rm_curr(struct _score_docs* _s);

void
score_docs_destory(struct _score_docs* _s);

void
score_docs_clear(struct _score_docs* _s);

void
score_docs_print(struct _score_docs* _s);

struct _top_docs
{
  int total_hits;
  struct _sort_field* fields;
  struct _score_docs* score_docs;
};

struct _top_docs*
top_docs_initial(int _total_hits, struct _score_docs* _sc_docs);

#endif

