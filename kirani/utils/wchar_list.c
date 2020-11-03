/*
 * author: Thomas Yao
 */

#include <err.h>
#include <stdio.h>
#include <stdlib.h>

#include "wchar_list.h"

struct _wchar_list*
wchar_list_initial()
{
  struct _wchar_list* s = (struct _wchar_list*) malloc(
      sizeof(struct _wchar_list));
  if (s == NULL)
    err(1, "wchar_list is null");
  s->curr = NULL;
  s->size = 0;
  return s;
}

void
wchar_list_add(struct _wchar_list* _s, wchar_t* _data)
{
  struct _wchar_list_node* node = (struct _wchar_list_node*) malloc(
      sizeof(struct _wchar_list_node));
  if (node == NULL)
    err(1, "wchar_list_node is null");

  if (_s->size == 0)
    {
      node->next = node;
      node->prev = node;
      _s->curr = node;
    }

  struct _wchar_list_node* cn = _s->curr;
  struct _wchar_list_node* nn = _s->curr->next;

  cn->next = node;
  nn->prev = node;

  node->next = nn;
  node->prev = cn;

  node->data = _data;

  _s->curr = node;
  _s->curr->index = _s->size++;
}

struct _wchar_list*
wchar_list_initial_by_array(wchar_t* _array[], int _size)
{
  struct _wchar_list* s = wchar_list_initial();
  int i;
  for (i = 0; i < _size; i++)
    wchar_list_add(s, _array[i]);
  return s;
}

void
wchar_list_rm_curr(struct _wchar_list* s)
{
  if (s == NULL)
    err(1, "wchar_list is null");

  struct _wchar_list_node* cn = s->curr;

  cn->prev->next = cn->next;
  cn->next->prev = cn->prev;

  s->curr = cn->prev;
  s->size--;

  free(cn);
}

wchar_t*
wchar_list_get_index(struct _wchar_list* s, int _index)
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

void
wchar_list_destory(struct _wchar_list* _s)
{
  if (_s == NULL)
    err(1, "wchar_list is null");
  while (_s->size != 0)
    {
      wchar_list_rm_curr(_s);
    }
  free(_s);
}

void
wchar_list_rmall_by_data(struct _wchar_list* _s, wchar_t* _data)
{
  int i;
  int j = _s->size;
  for (i = 0; i < j; i++)
    {
      if (0 == wcscmp(_s->curr->data, _data))
        {
          wchar_list_rm_curr(_s);
        }
      _s->curr = _s->curr->next;
    }
}

void
wchar_list_print(struct _wchar_list* _s)
{
  int i;
  for (i = 0; i < _s->size; i++)
    {
      _s->curr = _s->curr->prev;
    }
}

