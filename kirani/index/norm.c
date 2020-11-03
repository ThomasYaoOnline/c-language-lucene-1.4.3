/*
 * author: Thomas Yao
 */

#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "norm.h"

struct _norm*
norm_initial(struct _file_stream* _in, int _number)
{
  struct _norm* nm = (struct _norm*) malloc(sizeof(struct _norm));
  if (nm == NULL)
    err(1, "nm is null");
  nm->in = _in;
  nm->number = _number;
  nm->bytes = int_list_initial();
  return nm;
}

struct _norms*
norms_initial()
{
  struct _norms* tvfs = (struct _norms*) malloc(sizeof(struct _norms));
  if (tvfs == NULL)
    err(1, "tvfs is null");
  tvfs->curr = NULL;
  tvfs->size = 0;
  return tvfs;
}

void
norms_add(struct _norms* _s, char* _key, struct _norm* _data)
{
  struct _norms_node* node = (struct _norms_node*) malloc(
      sizeof(struct _norms_node));

  if (node == NULL)
    err(1, "node is null");

  if (_s->size == 0)
    {
      node->next = node;
      node->prev = node;
      _s->curr = node;
    }

  struct _norms_node* cn = _s->curr;
  struct _norms_node* nn = _s->curr->next;

  cn->next = node;
  nn->prev = node;

  node->next = nn;
  node->prev = cn;

  node->key = _key;
  node->data = _data;

  _s->curr = node;
  _s->curr->index = _s->size++;
}

struct _norm*
norms_get_curr(struct _norms* _s)
{
  return _s->curr->data;
}

struct _norm*
norms_get_index(struct _norms* s, int _index)
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

struct _norm*
norms_get_by_key(struct _norms* _s, char* _key)
{
  int i;
  for (i = 0; i < _s->size; i++)
    {
      if (0 == strcmp(_s->curr->key, _key))
        return _s->curr->data;
      else
        _s->curr = _s->curr->next;
    }
  return NULL;
}

void
norms_rm_curr(struct _norms* _s)
{
  if (_s == NULL)
    err(1, "_s is null");

  struct _norms_node* cn = _s->curr;

  cn->prev->next = cn->next;
  cn->next->prev = cn->prev;

  _s->curr = cn->prev;
  _s->size--;

  free(cn);
}

void
norms_destory(struct _norms* _s)
{
  if (_s == NULL)
    err(1, "_s is null");

  while (_s->size != 0)
    {
      norms_rm_curr(_s);
    }

  free(_s);
}

void
norms_clear(struct _norms* _s)
{
  if (_s == NULL)
    err(1, "_s is null");
  while (_s->size != 0)
    {
      norms_rm_curr(_s);
    }
}

void
norms_print(struct _norms* _s)
{
  int i;
  for (i = 0; i < _s->size; i++)
    {
      _s->curr = _s->curr->next;
    }
}
