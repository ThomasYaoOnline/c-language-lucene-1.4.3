/*
 * author: Thomas Yao
 */

#include <stdio.h>
#include <stdlib.h>
#include <err.h>

#include "tv_field.h"

struct _tv_field*
tv_field_initial(int _number)
{
  struct _tv_field* tvf = (struct _tv_field*) malloc(sizeof(struct _tv_field));
  if (tvf == NULL)
    err(1, "tvf is null");
  tvf->number = _number;
  tvf->length = 0;
  tvf->tvf_pointer = 0;
  return tvf;
}

struct _tv_fields*
tv_fields_initial()
{
  struct _tv_fields* tvfs = (struct _tv_fields*) malloc(
      sizeof(struct _tv_fields));
  if (tvfs == NULL)
    err(1, "tvfs is null");
  tvfs->curr = NULL;
  tvfs->size = 0;
  return tvfs;
}

void
tv_fields_add(struct _tv_fields* _s, struct _tv_field* _data)
{
  struct _tv_fields_node* node = (struct _tv_fields_node*) malloc(
      sizeof(struct _tv_fields_node));
  if (node == NULL)
    err(1, "node is null");

  if (_s->size == 0)
    {
      node->next = node;
      node->prev = node;
      _s->curr = node;
    }

  struct _tv_fields_node* cn = _s->curr;
  struct _tv_fields_node* nn = _s->curr->next;

  cn->next = node;
  nn->prev = node;

  node->next = nn;
  node->prev = cn;

  node->data = _data;

  _s->curr = node;
  _s->curr->index = _s->size++;
}

struct _tv_field*
tv_fields_get_curr(struct _tv_fields* _s)
{
  return _s->curr->data;
}
struct _tv_field*
tv_fields_get_index(struct _tv_fields* s, int _index)
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
tv_fields_rm_curr(struct _tv_fields* _s)
{
  if (_s == NULL)
    err(1, "_s is null");

  struct _tv_fields_node* cn = _s->curr;

  cn->prev->next = cn->next;
  cn->next->prev = cn->prev;

  _s->curr = cn->prev;
  _s->size--;

  free(cn);
}

void
tv_fields_destory(struct _tv_fields* _s)
{
  if (_s == NULL)
    err(1, "_s is null");

  while (_s->size != 0)
    {
      tv_fields_rm_curr(_s);
    }

  free(_s);
}

void
tv_fields_clear(struct _tv_fields* _s)
{
  if (_s == NULL)
    err(1, "_s is null");
  while (_s->size != 0)
    {
      tv_fields_rm_curr(_s);
    }
}

void
tv_fields_print(struct _tv_fields* _s)
{
  int i;
  for (i = 0; i < _s->size; i++)
    {
      _s->curr = _s->curr->prev;
    }
}

