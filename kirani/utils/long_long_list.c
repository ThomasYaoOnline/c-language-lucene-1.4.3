/*
 * author: Thomas Yao
 */

#include <err.h>
#include <stdio.h>
#include <stdlib.h>

#include "long_long_list.h"

struct _long_long_list*
long_long_list_initial()
{
  struct _long_long_list* s = (struct _long_long_list*) malloc(
      sizeof(struct _long_long_list));
  if (s == NULL)
    err(1, "long_long_list is null");
  s->curr = NULL;
  s->size = 0;
  return s;
}

struct _long_long_list*
long_long_list_initial_fill(int length, long long value)
{
  struct _long_long_list* s = long_long_list_initial();
  int i;
  for (i = 0; i < length; i++)
    long_long_list_add(s, value);
  return s;
}

void
long_long_list_add(struct _long_long_list* _s, long long _data)
{
  struct _long_long_list_node* node = (struct _long_long_list_node*) malloc(
      sizeof(struct _long_long_list_node));
  if (node == NULL)
    err(1, "long_long_list_node is null");

  if (_s->size == 0)
    {
      node->next = node;
      node->prev = node;
      _s->curr = node;
    }

  struct _long_long_list_node* cn = _s->curr;
  struct _long_long_list_node* nn = _s->curr->next;

  cn->next = node;
  nn->prev = node;

  node->next = nn;
  node->prev = cn;

  node->data = _data;

  _s->curr = node;
  _s->curr->index = _s->size++;
}

struct _long_long_list*
long_long_list_initial_by_array(long long _array[], int _size)
{
  struct _long_long_list* s = long_long_list_initial();
  int i;
  for (i = 0; i < _size; i++)
    long_long_list_add(s, _array[i]);
  return s;
}

long long
long_long_list_get_curr(struct _long_long_list* _s)
{
  return _s->curr->data;
}

long long
long_long_list_get_index(struct _long_long_list* s, int _index)
{
  int i;
  for (i = 0; i < s->size; i++)
    {
      if (s->curr->index == _index)
        return s->curr->data;
      s->curr = s->curr->next;
    }
  return 0;
}

int
long_long_list_set_index(struct _long_long_list* s, int _index, long long value)
{
  int i;
  for (i = 0; i < s->size; i++)
    {
      if (s->curr->index == _index)
        {
          s->curr->data = value;
          return 0;
        }
      s->curr = s->curr->next;
    }
  return -1;
}

void
long_long_list_rm_curr(struct _long_long_list* s)
{
  if (s == NULL)
    err(1, "long_long_list is null");

  struct _long_long_list_node* cn = s->curr;

  cn->prev->next = cn->next;
  cn->next->prev = cn->prev;

  s->curr = cn->prev;
  s->size--;

  free(cn);
}

void
long_long_list_destory(struct _long_long_list* _s)
{
  if (_s == NULL)
    err(1, "_s is null");

  while (_s->size != 0)
    {
      long_long_list_rm_curr(_s);
    }
  free(_s);
}

void
long_long_list_rmall_by_data(struct _long_long_list* _s, long long _data)
{
  int i;
  int j = _s->size;
  for (i = 0; i < j; i++)
    {
      if (_s->curr->data == _data)
        {
          long_long_list_rm_curr(_s);
        }
      _s->curr = _s->curr->next;
    }
}

void
long_long_list_print(struct _long_long_list* _s)
{
  int i;
  for (i = 0; i < _s->size; i++)
    {
      _s->curr = _s->curr->prev;
    }
}

