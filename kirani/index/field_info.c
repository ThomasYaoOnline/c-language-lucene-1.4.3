/*
 * author: Thomas Yao
 */

#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "field_info.h"

#include "../store/file_stream.h"

struct _field_info*
field_info_initial(char* _name, int _is_indexed, int _number,
    int _store_term_vector)
{
  struct _field_info* fi = (struct _field_info*) malloc(
      sizeof(struct _field_info));

  if (fi == NULL)
    err(1, "field_info is null");

  fi->name = _name;
  fi->is_indexed = _is_indexed;
  fi->number = _number;
  fi->store_term_vector = _store_term_vector;

  return fi;
}

struct _field_infos*
field_infos_initial()
{
  struct _field_infos* s = (struct _field_infos*) malloc(
      sizeof(struct _field_infos));

  if (s == NULL)
    err(1, "field_infos is null");

  s->curr = NULL;
  s->size = 0;

  struct _field_info* fi = field_info_initial("", 0, s->size, 0);
  field_infos_add(s, fi);

  return s;
}

struct _field_infos*
field_infos_initial_read(struct _directory* dir, char* segment_name)
{
  struct _field_infos* fieldinfos = (struct _field_infos*) malloc(
      sizeof(struct _field_infos));

  if (fieldinfos == NULL)
    err(1, "field_infos is null");

  fieldinfos->curr = NULL;
  fieldinfos->size = 0;
  struct _file_stream* in = directory_open_file(dir, segment_name, ".fnm", "r");
  field_infos_read(fieldinfos, in);
  fs_close(in);
  return fieldinfos;
}

void
field_infos_add_doc(struct _field_infos* _s, struct _document* _doc)
{
  struct _field* field;
  struct _field_info* fi;

  int i;
  for (i = 0; i < _doc->fields->size; i++)
    {
      field = fields_get_index(_doc->fields, i);

      fi = field_info_initial(field->name, field->is_indexed, _s->size,
          field->store_term_vector);
      field_infos_add(_s, fi);
    }
}

void
field_infos_add(struct _field_infos* _s, struct _field_info* _data)
{
  struct _field_infos_node* node = (struct _field_infos_node*) malloc(
      sizeof(struct _field_infos_node));

  if (node == NULL)
    err(1, "node is null");

  if (_s->size == 0)
    {
      node->next = node;
      node->prev = node;
      _s->curr = node;
    }

  struct _field_infos_node* cn = _s->curr;
  struct _field_infos_node* nn = _s->curr->next;

  cn->next = node;
  nn->prev = node;

  node->next = nn;
  node->prev = cn;

  node->data = _data;

  _s->curr = node;
  _s->curr->index = _s->size++;
}

struct _field_info*
field_infos_pop(struct _field_infos* _s)
{
  if (_s == NULL)
    err(1, "s is null");

  struct _field_infos_node* cn = _s->curr;

  cn->prev->next = cn->next;
  cn->next->prev = cn->prev;

  _s->curr = cn->prev;
  _s->size--;

  return cn->data;
}

void
field_infos_rm_curr(struct _field_infos* s)
{
  if (s == NULL)
    err(1, "field_infos is null");

  struct _field_infos_node* cn = s->curr;

  cn->prev->next = cn->next;
  cn->next->prev = cn->prev;

  s->curr = cn->prev;
  s->size--;

  free(cn);
}

void
field_infos_destory(struct _field_infos* _s)
{
  if (_s == NULL)
    err(1, "field_infos is null");

  while (_s->size != 0)
    {
      field_infos_rm_curr(_s);
    }
  free(_s);
}

void
field_infos_print(struct _field_infos* s)
{
  printf("%s\n", "fieldinfos print^^^");
  struct _field_info* fi;
  int i;
  for (i = 0; i < s->size; i++)
    {
      fi = field_infos_get_index(s, i);
      printf("name:              %s\n", fi->name);
      printf("number:            %d\n", fi->number);
      printf("is_indexed:        %d\n", fi->is_indexed);
      printf("store_term_vector: %d\n", fi->store_term_vector);
    }
  printf("%s\n", "fieldinfos print$$$");
}

struct _field_info*
field_infos_get_by_name(struct _field_infos* _s, char* _name)
{
  int i;
  for (i = 0; i < _s->size; i++)
    {
      if (0 == strcmp(_s->curr->data->name, _name))
        return _s->curr->data;
      else
        _s->curr = _s->curr->next;
    }
  return NULL;
}

int
field_infos_get_number_by_name(struct _field_infos* s, char* _name)
{
  struct _field_info* fi = field_infos_get_by_name(s, _name);
  return fi == NULL ? -1 : fi->number;
}

char*
field_infos_get_name_by_number(struct _field_infos* _s, int _number)
{
  struct _field_info* fi = field_infos_get_by_number(_s, _number);
  if (fi != NULL)
    return fi->name;
  return NULL;
}

struct _field_info*
field_infos_get_by_number(struct _field_infos* _s, int _number)
{
  int i;
  for (i = 0; i < _s->size; i++)
    {
      //printf("_s->curr->data->number: %d  _number: %d\n", _s->curr->data->number, _number);
      if (_s->curr->data->number == _number)
        return _s->curr->data;
      _s->curr = _s->curr->next;
    }
  return NULL;
}

struct _field_info*
field_infos_get_index(struct _field_infos* s, int _index)
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
field_infos_write(struct _field_infos* s, struct _directory* dir,
    char* sagment, char* suffix)
{
  struct _file_stream* fs = directory_open_file(dir, sagment, suffix, "w");

  fs_write_vint(fs, s->size);
  printf("field_infos_write:size: %d\n", s->size);

  int i;
  struct _field_info* fi;
  int bits;
  for (i = 0; i < s->size; i++)
    {
      fi = field_infos_get_index(s, i);
      bits = 0x0;
      if (fi->is_indexed)
        bits |= 0x1;
      if (fi->store_term_vector)
        bits |= 0x2;
      fs_write_string(fs, fi->name);
      printf("field_infos_write:fi->name: %s\n", fi->name);
      fs_write_byte(fs, bits);
      printf("field_infos_write:bits: %d\n", bits);
    }
  fs_close(fs);
}

void
field_infos_read(struct _field_infos* fieldinfos, struct _file_stream* in)
{
  int size = fs_read_vint(in);
  printf("field_infos_read:size: %d\n", size);

  int i;
  char* name;
  int bits;
  int is_indexed;
  int store_term_vector;
  for (i = 0; i < size; i++)
    {
      name = fs_read_string(in);
      printf("field_infos_read:fi->name: %s\n", name);
      bits = fs_read_byte(in);
      printf("field_infos_read:bits: %d\n", bits);
      is_indexed = (bits & 0x1) != 0;
      store_term_vector = (bits & 0x2) != 0;
      add_internal(fieldinfos, name, is_indexed, store_term_vector);
    }
}

void
add_internal(struct _field_infos* fis, char* name, int is_indexed,
    int store_term_vector)
{
  struct _field_info* fi = field_info_initial(name, is_indexed, fis->size,
      store_term_vector);
  field_infos_add(fis, fi);
}

void
field_infos_add_notyet(struct _field_infos* fis, char* _name, int _is_indexed,
    int _store_term_vector)
{
  struct _field_info* fi = field_infos_get_by_name(fis, _name);
  if (fi == NULL)
    add_internal(fis, _name, _is_indexed, _store_term_vector);
  else
    {
      if (fi->is_indexed != _is_indexed)
        fi->is_indexed = 1;
      if (fi->store_term_vector != _store_term_vector)
        fi->store_term_vector = 1;
    }
}

void
field_infos_add_indexed(struct _field_infos* fis,
    struct _char_list* fieldnames, int _store_term_vectors)
{
  int j;
  for (j = 0; j < fieldnames->size; j++)
    {
      field_infos_add_notyet(fis, char_list_get_index(fieldnames, j), 1,
          _store_term_vectors);
    }
}

void
field_infos_add_not_storetermvectors(struct _field_infos* fis,
    struct _char_list* fieldnames, int _is_index)
{
  int j;
  for (j = 0; j < fieldnames->size; j++)
    {
      field_infos_add_notyet(fis, char_list_get_index(fieldnames, j),
          _is_index, 0);
    }
}
