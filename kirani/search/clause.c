/*
 * author: Thomas Yao
 */

#include <err.h>
#include <stdlib.h>

#include "clause.h"

struct _clause*
clause_initial(struct _query* _qry, int r, int p)
{
  struct _clause* cls = (struct _clause*) malloc(sizeof(struct _clause));

  if (cls == NULL)
    err(1, "cls is null");

  /* default */
  cls->required = 0;
  cls->prohibited = 0;
  /* default */

  cls->required = r;
  cls->prohibited = p;
  cls->qry = _qry;

  return cls;
}

struct _clauses*
clauses_initial()
{
  struct _clauses* s = (struct _clauses*) malloc(sizeof(struct _clauses));
  if (s == NULL)
    err(1, "s is null");
  s->curr = NULL;
  s->size = 0;
  return s;
}

void
clauses_add(struct _clauses* _s, struct _clause* _data)
{
  struct _clauses_node* node = (struct _clauses_node*) malloc(
      sizeof(struct _clauses_node));

  if (node == NULL)
    err(1, "node is null");

  if (_s->size == 0)
    {
      node->next = node;
      node->prev = node;
      _s->curr = node;
    }

  struct _clauses_node* cn = _s->curr;
  struct _clauses_node* nn = _s->curr->next;

  cn->next = node;
  nn->prev = node;

  node->next = nn;
  node->prev = cn;

  node->data = _data;

  _s->curr = node;
  _s->curr->index = _s->size++;
}

struct _clause*
clauses_get_curr(struct _clauses* _s)
{
  return _s->curr->data;
}

struct _clause*
clauses_get_index(struct _clauses* s, int _index)
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
clauses_rm_curr(struct _clauses* s)
{
  if (s == NULL)
    err(1, "_s is null");
  struct _clauses_node* cn = s->curr;

  cn->prev->next = cn->next;
  cn->next->prev = cn->prev;

  s->curr = cn->prev;
  s->size--;

  free(cn);
}

void
clauses_destory(struct _clauses* _s)
{
  if (_s == NULL)
    err(1, "_s is null");
  while (_s->size != 0)
    {
      clauses_rm_curr(_s);
    }
  free(_s);
}

void
clauses_clear(struct _clauses* _s)
{
  if (_s == NULL)
    err(1, "_s is null");

  while (_s->size != 0)
    {
      clauses_rm_curr(_s);
    }
}

void
clauses_print(struct _clauses* _s)
{
  int i;
  for (i = 0; i < _s->size; i++)
    {
      _s->curr = _s->curr->next;
    }
}
