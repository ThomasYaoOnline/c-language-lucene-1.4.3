/*
 * author: Thomas Yao
 */

#include <err.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <wchar.h>

#include "field.h"

struct _field*
field_initial(char* _name, char* _string_value,
    struct _file_stream* _reader_value, int _is_stored, int _is_indexed,
    int _is_tokenized, int _store_term_vector)
{
  if (_name == NULL)
    err(1, "_name cannot be null");
  if (_string_value == NULL && _reader_value == NULL)
    err(1, "_string_value and _reader_value is null");
  if (!_is_indexed && _store_term_vector)
    err(1, "cannot store a term vector for fields that are not indexed.");

  struct _field* fd = (struct _field*) malloc(sizeof(struct _field));

  if (fd == NULL)
    err(1, "fd is null");

  fd->name = _name;

  fd->string_value = _string_value;
  fd->reader_value = _reader_value;

  fd->is_stored = _is_stored;
  fd->is_indexed = _is_indexed;
  fd->is_tokenized = _is_tokenized;
  fd->store_term_vector = _store_term_vector;

  fd->boost = 1.0f;

  return fd;
}

void
field_print(struct _field* _fd)
{
  if (_fd == NULL)
    err(1, "_fd is null");
  printf("fd->name: %s\n", _fd->name);
  printf("fd->string_value: %s\n", _fd->string_value);
  printf("fd->is_indexed: %i\n", _fd->is_indexed);
  printf("fd->is_stored: %i\n", _fd->is_stored);
  printf("fd->is_tokenized: %i\n", _fd->is_tokenized);
  printf("fd->store_term_vector: %i\n", _fd->store_term_vector);
  printf("fd->boost: %f\n", _fd->boost);
}

struct _fields*
fields_initial()
{
  struct _fields* s = (struct _fields*) malloc(sizeof(struct _fields));
  if (s == NULL)
    err(1, "s is null");
  s->curr = NULL;
  s->size = 0;
  return s;
}

void
fields_add(struct _fields* s, struct _field* _data)
{
  if (s == NULL)
    err(1, "s is null");

  struct _fields_node* node = (struct _fields_node*) malloc(
      sizeof(struct _fields_node));

  if (node == NULL)
    err(1, "node is null");

  if (s->size == 0)
    {
      s->curr = node;
      node->next = node;
      node->prev = node;
    }

  struct _fields_node* cn = s->curr;
  struct _fields_node* nn = s->curr->next;

  cn->next = node;
  nn->prev = node;

  node->next = nn;
  node->prev = cn;

  node->data = _data;

  s->curr = node;
  s->curr->index = s->size++;
}


void
fields_rm_curr(struct _fields* s)
{
  if (s == NULL)
    err(1, "_s is null");

  struct _fields_node* cn = s->curr;
  cn->prev->next = cn->next;
  cn->next->prev = cn->prev;

  s->curr = cn->prev;

  free(cn);
  s->size--;
}

struct _field*
fields_get_by_name(struct _fields* _s, char* _name)
{
  if (_s == NULL)
    err(1, "_s is null");

  int i;
  for (i = 0; i < _s->size; i++)
    {
      if (0 == strcmp(_s->curr->data->name, _name))
        return _s->curr->data;
      _s->curr = _s->curr->next;
    }
  return NULL;
}

struct _field*
fields_get_curr(struct _fields* _s)
{
  return _s->curr->data;
}

struct _field*
fields_get_index(struct _fields* s, int _index)
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

struct _field*
fields_pop(struct _fields* _s)
{
  if (_s == NULL)
    err(1, "s is null");

  struct _fields_node* cn = _s->curr;

  cn->prev->next = cn->next;
  cn->next->prev = cn->prev;

  _s->curr = cn->prev;
  _s->size--;

  return cn->data;
}

void
fields_rmall_by_data(struct _fields* _s, char* _name)
{
  if (_s == NULL)
    err(1, "_s is null");

  int i;
  for (i = 0; i < _s->size; i++)
    {
      if (0 == strcmp(_s->curr->data->name, _name))
        {
          fields_rm_curr(_s);
        }
      _s->curr = _s->curr->next;
    }
}

void
fields_destory(struct _fields* _s)
{
  if (_s == NULL)
    err(1, "_s is null");

  while (_s->size != 0)
    {
      fields_rm_curr(_s);
    }

  free(_s);
}

void
fields_print(struct _fields* _s)
{
  if (_s == NULL)
    err(1, "_s is null");
  struct _field* fd;
  int i;
  for (i = 0; i < _s->size; i++)
    {
      fd = _s->curr->data;
      printf("\n");
      printf("fd->name: %s\n", fd->name);
      printf("fd->string_value: %s\n", fd->string_value);
      printf("fd->is_indexed: %i\n", fd->is_indexed);
      printf("fd->is_stored: %i\n", fd->is_stored);
      printf("fd->is_tokenized: %i\n", fd->is_tokenized);
      printf("fd->store_term_vector: %i\n", fd->store_term_vector);
      printf("fd->boost: %f\n", fd->boost);
      _s->curr = _s->curr->next;
    }
}

