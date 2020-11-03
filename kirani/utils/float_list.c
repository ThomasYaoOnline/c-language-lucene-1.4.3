/*
 * author: Thomas Yao
 */

#include <err.h>
#include <stdio.h>
#include <stdlib.h>

#include "float_list.h"

struct _float_list*
float_list_initial()
{
  struct _float_list* s = (struct _float_list*) malloc(
      sizeof(struct _float_list));
  if (s == NULL)
    err(1, "float_list is null");
  s->curr = NULL;
  s->size = 0;
  return s;
}

struct _float_list*
float_list_initial_fill(int length, float value)
{
  struct _float_list* s = float_list_initial();
  int i;
  for (i = 0; i < length; i++)
    float_list_add(s, value);
  return s;
}

void
float_list_add(struct _float_list* _s, float _data)
{
  struct _float_list_node* node = (struct _float_list_node*) malloc(
      sizeof(struct _float_list_node));

  if (node == NULL)
    err(1, "float_list_node is null");

  if (_s->size == 0)
    {
      node->next = node;
      node->prev = node;
      _s->curr = node;
    }

  struct _float_list_node* cn = _s->curr;
  struct _float_list_node* nn = _s->curr->next;

  cn->next = node;
  nn->prev = node;

  node->next = nn;
  node->prev = cn;

  node->data = _data;

  _s->curr = node;
  _s->curr->index = _s->size++;
}

struct _float_list*
float_list_initial_by_array(float _array[], int _size)
{
  struct _float_list* s = float_list_initial();
  int i;
  for (i = 0; i < _size; i++)
    float_list_add(s, _array[i]);
  return s;
}

float
float_list_get_curr(struct _float_list* _s)
{
  if (_s == NULL)
    err(1, "_s is null");
  if (_s->size == 0)
    {
      return 0.0f;
    }
  return _s->curr->data;
}

float
float_list_get_index(struct _float_list* s, int _index)
{
  int i;
  for (i = 0; i < s->size; i++)
    {
      if (s->curr->index == _index)
        return s->curr->data;
      s->curr = s->curr->next;
    }
  return 0.0f;
}

int
float_list_set_index(struct _float_list* s, int _index, float value)
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
float_list_rm_curr(struct _float_list* s)
{
  if (s == NULL)
    err(1, "float_list is null");
  struct _float_list_node* cn = s->curr;

  cn->prev->next = cn->next;
  cn->next->prev = cn->prev;

  s->curr = cn->prev;
  s->size--;

  free(cn);
}

float
float_list_pop(struct _float_list* _s)
{
  float result = _s->curr->data;
  float_list_rm_curr(_s);
  return result;
}

void
float_list_destory(struct _float_list* _s)
{
  if (_s == NULL)
    err(1, "float_list is null");
  while (_s->size != 0)
    {
      float_list_rm_curr(_s);
    }
  free(_s);
}

void
float_list_rmall_by_data(struct _float_list* _s, float _data)
{
  int i;
  int j = _s->size;
  for (i = 0; i < j; i++)
    {
      if (_s->curr->data == _data)
        {
          float_list_rm_curr(_s);
        }
      _s->curr = _s->curr->next;
    }
}

void
float_list_print(struct _float_list* _s)
{
  int i;
  for (i = 0; i < _s->size; i++)
    {
      printf("%f\n", _s->curr->data);
      _s->curr = _s->curr->prev;
    }
}

