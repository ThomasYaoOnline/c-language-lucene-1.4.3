/*
 * author: Thomas Yao
 */

#ifndef _TERM_ENUM_H_
#define _TERM_ENUM_H_

#include "field_info.h"
#include "term_posting.h"
#include "term_info.h"

#include "../store/file_stream.h"

struct _term_enum
{
  struct _file_stream* input;
  struct _field_infos* fieldinfos;
  long long size;
  long long position;
  struct _term* term;
  struct _term_info* terminfo;
  int format;
  int is_index;
  long long index_pointer;
  int index_interval;
  int skip_interval;
  int format_m1_skip_interval;
  struct _term* prev;
  char buffer[1024];
};

struct _term_enum*
term_enum_initial(struct _file_stream* _in, struct _field_infos* _fieldinfos,
    int _is_index);

int
term_enum_next(struct _term_enum* _te);

void
term_enum_close(struct _term_enum* _te);
void
term_enum_seek(struct _term_enum* _te, long long pointer, int p,
    struct _term* t, struct _term_info* ti);


struct _term_enums_node
{
  struct _term_enums_node* next;
  struct _term_enums_node* prev;
  struct _term_enum* data;
  int index;
};

struct _term_enums
{
  struct _term_enums_node* curr;
  int size;
};

struct _term_enums*
term_enums_initial();

void
term_enums_add(struct _term_enums* _s, struct _term_enum* _tvf);

struct _term_enum*
term_enums_get_curr(struct _term_enums* _s);

struct _term_enum*
term_enums_get_index(struct _term_enums* s, int _index);

void
term_enums_rm_curr(struct _term_enums* _s);

void
term_enums_destory(struct _term_enums* _s);

void
term_enums_clear(struct _term_enums* _s);

void
term_enums_print(struct _term_enums* _s);


#endif

