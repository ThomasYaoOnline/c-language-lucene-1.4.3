/*
 * author: Thomas Yao
 */

#include <err.h>
#include <stdio.h>
#include <stdlib.h>

#include "int_list.h"

struct _int_list*
int_list_initial()
{
  struct _int_list* s = (struct _int_list*) malloc(sizeof(struct _int_list));
  if (s == NULL)
    err(1, "int_list is null");
  s->curr = NULL;
  s->size = 0;
  return s;
}

struct _int_list*
int_list_initial_fill(int length, int value)
{
  struct _int_list* s = int_list_initial();
  int i;
  for (i = 0; i < length; i++)
    int_list_add(s, value);
  return s;
}

void
int_list_add(struct _int_list* _s, int _data)
{
  struct _int_list_node* node = (struct _int_list_node*) malloc(
      sizeof(struct _int_list_node));
  if (node == NULL)
    err(1, "int_list_node is null");

  if (_s->size == 0)
    {
      node->next = node;
      node->prev = node;
      _s->curr = node;
    }

  struct _int_list_node* cn = _s->curr;
  struct _int_list_node* nn = _s->curr->next;

  cn->next = node;
  nn->prev = node;

  node->next = nn;
  node->prev = cn;

  node->data = _data;

  _s->curr = node;
  _s->curr->index = _s->size++;
}

struct _int_list*
int_list_initial_by_array(int _array[], int _size)
{
  struct _int_list* s = int_list_initial();
  int i;
  for (i = 0; i < _size; i++)
    int_list_add(s, _array[i]);
  return s;
}

int
int_list_get_curr(struct _int_list* _s)
{
  return _s->curr->data;
}

int
int_list_get_index(struct _int_list* s, int _index)
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
int_list_set_index(struct _int_list* s, int _index, int value)
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
int_list_rm_curr(struct _int_list* s)
{
  if (s == NULL)
    err(1, "int_list is null");

  struct _int_list_node* cn = s->curr;

  cn->prev->next = cn->next;
  cn->next->prev = cn->prev;

  s->curr = cn->prev;
  s->size--;

  free(cn);
}

void
int_list_destory(struct _int_list* _s)
{
  if (_s == NULL)
    err(1, "_s is null");

  while (_s->size != 0)
    {
      int_list_rm_curr(_s);
    }
  free(_s);
}

void
int_list_rmall_by_data(struct _int_list* _s, int _data)
{
  int i;
  int j = _s->size;
  for (i = 0; i < j; i++)
    {
      if (_s->curr->data == _data)
        {
          int_list_rm_curr(_s);
        }
      _s->curr = _s->curr->next;
    }
}

void
int_list_print(struct _int_list* _s)
{
  int i;
  for (i = 0; i < _s->size; i++)
    {
      _s->curr = _s->curr->next;
    }
}

