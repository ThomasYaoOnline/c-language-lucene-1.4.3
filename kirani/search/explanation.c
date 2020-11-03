/*
 * author: Thomas Yao
 */
#include <err.h>
#include <stdio.h>
#include <stdlib.h>

#include "explanation.h"

struct _explanations*
explanations_initial()
{
  struct _explanations* s = (struct _explanations*) malloc(
      sizeof(struct _explanations));
  if (s == NULL)
    err(1, "s is null");
  s->curr = NULL;
  s->size = 0;
  return s;
}

void
explanations_add(struct _explanations* _s, struct _explanation* _data)
{
  struct _explanations_node* node = (struct _explanations_node*) malloc(
      sizeof(struct _explanations_node));

  if (node == NULL)
    err(1, "node is null");

  if (_s->size == 0)
    {
      node->next = node;
      node->prev = node;
      _s->curr = node;
    }

  struct _explanations_node* cn = _s->curr;
  struct _explanations_node* nn = _s->curr->next;

  cn->next = node;
  nn->prev = node;

  node->next = nn;
  node->prev = cn;

  node->data = _data;

  _s->curr = node;
  _s->curr->index = _s->size++;
}

struct _explanation*
explanations_get_curr(struct _explanations* _s)
{
  return _s->curr->data;
}

struct _explanation*
explanations_get_index(struct _explanations* s, int _index)
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
explanations_rm_curr(struct _explanations* s)
{
  if (s == NULL)
    err(1, "_s is null");

  struct _explanations_node* cn = s->curr;

  cn->prev->next = cn->next;
  cn->next->prev = cn->prev;

  s->curr = cn->prev;
  s->size--;
  free(cn);
}

void
explanations_destory(struct _explanations* _s)
{
  if (_s == NULL)
    err(1, "_s is null");
  while (_s->size != 0)
    {
      explanations_rm_curr(_s);
    }
  free(_s);
}

void
explanations_clear(struct _explanations* _s)
{
  if (_s == NULL)
    err(1, "_s is null");
  while (_s->size != 0)
    {
      explanations_rm_curr(_s);
    }
}

void
explanations_print(struct _explanations* _s)
{
  int i;
  for (i = 0; i < _s->size; i++)
    {
      _s->curr = _s->curr->prev;
    }
}

