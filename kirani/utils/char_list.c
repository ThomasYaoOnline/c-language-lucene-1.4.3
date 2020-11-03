/*
 * author: Thomas Yao
 */

#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "char_list.h"

struct _char_list*
char_list_initial()
{
  struct _char_list* s = (struct _char_list*) malloc(sizeof(struct _char_list));
  if (s == NULL)
    err(1, "s is null");
  s->curr = NULL;
  s->size = 0;
  return s;
}

void
char_list_add(struct _char_list* _s, char* _data)
{
  struct _char_list_node* node = (struct _char_list_node*) malloc(
      sizeof(struct _char_list_node));

  if (node == NULL)
    err(1, "node is null");

  if (_s->size == 0)
    {
      node->next = node;
      node->prev = node;
      _s->curr = node;
    }

  struct _char_list_node* cn = _s->curr;
  struct _char_list_node* nn = _s->curr->next;

  cn->next = node;
  nn->prev = node;

  node->next = nn;
  node->prev = cn;

  node->data = _data;

  _s->curr = node;
  _s->curr->index = _s->size++;
}

char*
char_list_get_curr(struct _char_list* _s)
{
  return _s->curr->data;
}

char*
char_list_get_index(struct _char_list* s, int _index)
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

char*
char_list_pop(struct _char_list* _s)
{
  char* result = char_list_get_curr(_s);
  char_list_rm_curr(_s);
  return result;
}

struct _char_list*
char_list_initial_by_array(char* _array[], int _size)
{
  struct _char_list* s = char_list_initial();
  int i;
  for (i = 0; i < _size; i++)
    char_list_add(s, _array[i]);
  return s;
}

void
char_list_rm_curr(struct _char_list* s)
{
  if (s == NULL)
    err(1, "char_list is null");

  struct _char_list_node* cn = s->curr;

  cn->prev->next = cn->next;
  cn->next->prev = cn->prev;

  s->curr = cn->prev;
  s->size--;

  free(cn);
}

void
char_list_destory(struct _char_list* _s)
{
  if (_s == NULL)
    err(1, "s is null");
  while (_s->size != 0)
    {
      char_list_rm_curr(_s);
    }
  free(_s);
}

void
char_list_rmall_by_data(struct _char_list* _s, char* _data)
{
  int i;
  int j = _s->size;
  for (i = 0; i < j; i++)
    {
      if (0 == strcmp(_s->curr->data, _data))
        {
          char_list_rm_curr(_s);
        }
      _s->curr = _s->curr->next;
    }
}

void
char_list_print(struct _char_list* _s)
{
  int i;
  for (i = 0; i < _s->size; i++)
    {
      _s->curr = _s->curr->next;
    }
}

