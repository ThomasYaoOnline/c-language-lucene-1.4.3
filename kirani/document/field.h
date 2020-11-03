/*
 * author: Thomas Yao
 */

#ifndef _FIELD_H_
#define _FIELD_H_

#include <wchar.h>
#include <ctype.h>
#include <stdio.h>

#include "../store/file_stream.h"

struct _field
{
  char* name;
  char* string_value;
  struct _file_stream* reader_value;
  int is_stored;
  int is_indexed;
  int is_tokenized;
  int store_term_vector;
  float boost;
};

struct _field*
field_initial(char* _name, char* _string_value,
    struct _file_stream* _reader_value, int _is_stored, int _is_indexed,
    int _is_tokenized, int _store_term_vector);


void
field_print(struct _field* _fd);

struct _fields_node
{
  struct _fields_node* next;
  struct _fields_node* prev;
  struct _field* data;
  int index;
};

struct _fields
{
  struct _fields_node* curr;
  int size;
};

struct _fields*
fields_initial();

void
fields_destory(struct _fields* _s);

void
fields_add(struct _fields* _s, struct _field* _data);

struct _field*
fields_get_by_name(struct _fields* _s, char* _name);

struct _field*
fields_get_curr(struct _fields* _s);

struct _field*
fields_get_index(struct _fields* s, int _index);

void
fields_rm_curr(struct _fields* _s);

void
fields_rmall_by_data(struct _fields* _s, char* _name);

struct _field*
fields_pop(struct _fields* _s);

void
fields_print(struct _fields* _s);

#endif

