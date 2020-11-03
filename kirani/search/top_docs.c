/*
 * author: Thomas Yao
 */

#include <err.h>
#include <stdlib.h>
#include <stdio.h>

#include "top_docs.h"

struct _score_doc*
score_doc_initial(int _doc, float _score)
{
  struct _score_doc* sdoc = (struct _score_doc*) malloc(
      sizeof(struct _score_doc));
  if (sdoc == NULL)
    err(1, "sc_doc is null");
  sdoc->doc = _doc;
  sdoc->score = _score;
  return sdoc;
}

struct _score_docs*
score_docs_initial()
{
  struct _score_docs* _s = (struct _score_docs*) malloc(
      sizeof(struct _score_docs));
  if (_s == NULL)
    err(1, "_s is null");
  _s->curr = NULL;
  _s->size = 0;
  return _s;
}

void
score_docs_add(struct _score_docs* _s, struct _score_doc* _data)
{
  struct _score_docs_node* node = (struct _score_docs_node*) malloc(
      sizeof(struct _score_docs_node));
  if (node == NULL)
    err(1, "node is null");

  if (_s->size == 0)
    {
      node->next = node;
      node->prev = node;
      _s->curr = node;
    }

  struct _score_docs_node* cn = _s->curr;
  struct _score_docs_node* nn = _s->curr->next;

  cn->next = node;
  nn->prev = node;

  node->next = nn;
  node->prev = cn;

  node->data = _data;

  _s->curr = node;
  _s->curr->index = _s->size++;
}

struct _score_doc*
score_docs_get_curr(struct _score_docs* _s)
{
  return _s->curr->data;
}

struct _score_doc*
score_docs_get_index(struct _score_docs* _s, int _index)
{
  int i;
  for (i = 0; i < _s->size; i++)
    {
      if (_s->curr->index == _index)
        return _s->curr->data;
      _s->curr = _s->curr->next;
    }
  return NULL;
}

void
score_docs_rm_curr(struct _score_docs* s)
{
  if (s == NULL)
    err(1, "_s is null");
  struct _score_docs_node* cn = s->curr;
  cn->prev->next = cn->next;
  cn->next->prev = cn->prev;

  s->curr = cn->prev;
  s->size--;
  free(cn);
}

void
score_docs_destory(struct _score_docs* _s)
{
  if (_s == NULL)
    err(1, "_s is null");
  while (_s->size != 0)
    {
      score_docs_rm_curr(_s);
    }
  free(_s);
}

void
score_docs_clear(struct _score_docs* _s)
{
  if (_s == NULL)
    err(1, "_s is null");
  while (_s->size != 0)
    {
      score_docs_rm_curr(_s);
    }
}

void
score_docs_print(struct _score_docs* _s)
{
  int i;
  for (i = 0; i < _s->size; i++)
    {
      _s->curr = _s->curr->next;
    }
}

struct _top_docs*
top_docs_initial(int _total_hits, struct _score_docs* _sdocs)
{
  struct _top_docs* tp_docs = (struct _top_docs*) malloc(
      sizeof(struct _top_docs));
  if (tp_docs == NULL)
    err(1, "tp_docs is null");
  tp_docs->total_hits = _total_hits;
  tp_docs->score_docs = _sdocs;
  return tp_docs;
}

