/*
 * author: Thomas Yao
 */

#include <err.h>

#include <stdio.h>
#include <stdlib.h>

#include "tv_term.h"

struct _tv_term*
tv_term_initial()
{
  struct _tv_term* tvt = (struct _tv_term*) malloc(sizeof(struct _tv_term));
  if (tvt == NULL)
    err(1, "tvt is null");
  tvt->freq = 0;
  return tvt;
}

struct _tv_terms*
tv_terms_initial()
{
  struct _tv_terms* s = (struct _tv_terms*) malloc(sizeof(struct _tv_terms));
  if (s == NULL)
    err(1, "s is null");
  s->curr = NULL;
  s->size = 0;
  return s;
}

void
tv_terms_add(struct _tv_terms* _s, struct _tv_term* _data)
{
  struct _tv_terms_node* node = (struct _tv_terms_node*) malloc(
      sizeof(struct _tv_terms_node));
  if (node == NULL)
    err(1, "node is null");

  if (_s->size == 0)
    {
      node->next = node;
      node->prev = node;
      _s->curr = node;
    }

  struct _tv_terms_node* cn = _s->curr;
  struct _tv_terms_node* nn = _s->curr->next;

  cn->next = node;
  nn->prev = node;

  node->next = nn;
  node->prev = cn;

  node->data = _data;

  _s->curr = node;
  _s->curr->index = _s->size++;
}

struct _tv_term*
tv_terms_get_curr(struct _tv_terms* _s)
{
  return _s->curr->data;
}

struct _tv_term*
tv_terms_get_index(struct _tv_terms* s, int _index)
{
  int i;
  for (i = 0; i < s->size; i++)
    {
      if (s->curr->index == _index)
        return s->curr->data;
      s->curr = s->curr->next;
    }
  return NULL;
}

void
tv_terms_rm_curr(struct _tv_terms* s)
{
  if (s == NULL)
    err(1, "_s is null");
  struct _tv_terms_node* cn = s->curr;

  cn->prev->next = cn->next;
  cn->next->prev = cn->prev;

  s->curr = cn->prev;
  s->size--;
  free(cn);
}

void
tv_terms_destory(struct _tv_terms* _s)
{
  if (_s == NULL)
    err(1, "_s is null");

  while (_s->size != 0)
    {
      tv_terms_rm_curr(_s);
    }
  free(_s);
}

void
tv_terms_clear(struct _tv_terms* _s)
{
  if (_s == NULL)
    err(1, "_s is null");
  while (_s->size != 0)
    {
      tv_terms_rm_curr(_s);
    }
}

void
tv_terms_print(struct _tv_terms* _s)
{
  int i;
  for (i = 0; i < _s->size; i++)
    {
      _s->curr = _s->curr->prev;
    }
}

