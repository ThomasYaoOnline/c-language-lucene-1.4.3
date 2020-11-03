/*
 * author: Thomas Yao
 */

#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "term_enum.h"
#include "term_infos_writer.h"

#include "../utils/utils.h"

struct _term_enum*
term_enum_initial(struct _file_stream* _in, struct _field_infos* _fieldinfos,
    int _is_index)
{
  struct _term_enum* termenum = (struct _term_enum*) malloc(
      sizeof(struct _term_enum));

  if (termenum == NULL)
    err(1, "term_enum is null");

  /* defaults */
  termenum->position = -1;
  termenum->term = term_initial("", "");
  termenum->terminfo = term_info_initial();
  termenum->is_index = 0;
  termenum->index_pointer = 0;
  /* defaults */

  termenum->input = _in;
  termenum->fieldinfos = _fieldinfos;
  termenum->is_index = _is_index;

  int first_int = fs_read_int(_in); //format default: -2
  printf("term_enum_initial:first_int: %d\n", first_int);

  if (first_int > 0)
    {
      // original-format file, without explicit format version number
      termenum->format = 0;
      termenum->size = first_int;

      // back-compatible settings
      termenum->index_interval = 128;
      termenum->skip_interval = 0x7fffffff;
    }
  else
    {
      // we have a format version number
      termenum->format = first_int;

      // check that it is a format we can understand
      if (termenum->format < TERM_INFOS_WRITER_FORMAT)
        err(1, "unknown format version");

      termenum->size = fs_read_long(_in); // read the size  default:0
      printf("term_enum_initial:size: %lli\n", termenum->size);

      if (termenum->format == -1)
        {
          if (!termenum->is_index)
            {
              termenum->index_interval = fs_read_int(_in); // default:128
              printf("term_enum_initial:format-1:index_interval: %d\n",
                  termenum->index_interval);
              termenum->format_m1_skip_interval = fs_read_int(_in); // default:16
              printf(
                  "term_enum_initial:format-1:format_m1_skip_interval: %d\n",
                  termenum->format_m1_skip_interval);
            }
          // switch off skipTo optimization for file format prior to 1.4rc2
          // in order to avoid a bug in skipTo implementation of these versions
          termenum->skip_interval = 0x7fffffff;
        }
      else
        {
          termenum->index_interval = fs_read_int(_in); // default:128
          printf("term_enum_initial:format-2:index_interval: %d\n",
              termenum->index_interval);
          termenum->skip_interval = fs_read_int(_in); // default:16
          printf("term_enum_initial:format-2:skip_interval: %d\n",
              termenum->skip_interval);
        }
    }
  return termenum;
}

struct _term*
read_term(struct _term_enum* _term_enum)
{
  int start = fs_read_vint(_term_enum->input);
  printf("term_enum:read_term:start: %d\n", start);
  int length = fs_read_vint(_term_enum->input);
  printf("term_enum:read_term:length: %d\n", length);
  int total_length = start + length;

  // lazy grow buffer

  fs_read_chars(_term_enum->input, _term_enum->buffer, start, length);
  printf("term_enum:read_term:buffer: %s\n", _term_enum->buffer);

  int field_number = fs_read_vint(_term_enum->input);
  printf("term_enum:read_term:field_number: %i\n", field_number);

  return term_initial(field_infos_get_name_by_number(_term_enum->fieldinfos,
      field_number), strndup(_term_enum->buffer, total_length));

}

int
term_enum_next(struct _term_enum* _term_enum)
{
  printf("term_enum_next:position: %lli\n", _term_enum->position);
  printf("term_enum_next:size: %lli\n", _term_enum->size);

  if (_term_enum->position++ >= _term_enum->size - 1)
    {
      printf("term_enum_next: position++ >= size - 1 term=null %s\n", "return false");
      _term_enum->term = NULL;
      return 0;
    }
  _term_enum->prev = _term_enum->term;
  _term_enum->term = read_term(_term_enum);

  _term_enum->terminfo->doc_freq = fs_read_vint(_term_enum->input);
  printf("term_enum_next:_term_enum->terminfo->doc_freq: %i\n",
      _term_enum->terminfo->doc_freq);

  _term_enum->terminfo->freq_pointer += fs_read_vlong(_term_enum->input);
  printf("term_enum_next:_term_enum->terminfo->freq_pointer: %lli\n",
      _term_enum->terminfo->freq_pointer);

  _term_enum->terminfo->prox_pointer += fs_read_vlong(_term_enum->input);
  printf("term_enum_next:_term_enum->terminfo->prox_pointer: %lli\n",
      _term_enum->terminfo->prox_pointer);

  if (_term_enum->format == -1)
    {
      if (!_term_enum->is_index)
        {
          if (_term_enum->terminfo->doc_freq
              > _term_enum->format_m1_skip_interval)
            _term_enum->terminfo->skip_offset = fs_read_vint(_term_enum->input);
        }
    }
  else
    {
      if (_term_enum->terminfo->doc_freq >= _term_enum->skip_interval)
        {
          _term_enum->terminfo->skip_offset = fs_read_vint(_term_enum->input);
          printf("term_enum_next:_term_enum->terminfo->skip_offset: %i\n",
              _term_enum->terminfo->skip_offset);
        }
    }

  if (_term_enum->is_index)
    {
      _term_enum->index_pointer += fs_read_vlong(_term_enum->input);
      printf("term_enum_next:_term_enum->index_pointer: %lli\n",
          _term_enum->index_pointer);
    }
  return 1;
}

void
term_enum_close(struct _term_enum* _te)
{
  fs_close(_te->input);
}

void
grow_buffer(struct _term_enum* te)
{
  int i;
  for (i = 0; i < strlen(te->term->text); i++)
    {
      te->buffer[i] = te->term->text[i];
    }
}

void
term_enum_seek(struct _term_enum* te, long long pointer, int p,
    struct _term* t, struct _term_info* ti)
{
  fs_seek(te->input, pointer);
  te->position = p;
  te->term = t;
  te->prev = NULL;
  term_info_set_copy(te->terminfo, ti);
  grow_buffer(te); // copy term text into buffer
}

struct _term_enums*
term_enums_initial()
{
  struct _term_enums* tvfs = (struct _term_enums*) malloc(
      sizeof(struct _term_enums));
  if (tvfs == NULL)
    err(1, "tvfs is null");
  tvfs->curr = NULL;
  tvfs->size = 0;
  return tvfs;
}

void
term_enums_add(struct _term_enums* _s, struct _term_enum* _data)
{
  struct _term_enums_node* node = (struct _term_enums_node*) malloc(
      sizeof(struct _term_enums_node));
  if (node == NULL)
    err(1, "node is null");

  if (_s->size == 0)
    {
      node->next = node;
      node->prev = node;
      _s->curr = node;
    }

  struct _term_enums_node* cn = _s->curr;
  struct _term_enums_node* nn = _s->curr->next;

  cn->next = node;
  nn->prev = node;

  node->next = nn;
  node->prev = cn;

  node->data = _data;

  _s->curr = node;
  _s->curr->index = _s->size++;
}

struct _term_enum*
term_enums_get_curr(struct _term_enums* _s)
{
  return _s->curr->data;
}
struct _term_enum*
term_enums_get_index(struct _term_enums* s, int _index)
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
term_enums_rm_curr(struct _term_enums* _s)
{
  if (_s == NULL)
    err(1, "_s is null");

  struct _term_enums_node* cn = _s->curr;

  cn->prev->next = cn->next;
  cn->next->prev = cn->prev;

  _s->curr = cn->prev;
  _s->size--;

  free(cn);
}

void
term_enums_destory(struct _term_enums* _s)
{
  if (_s == NULL)
    err(1, "_s is null");

  while (_s->size != 0)
    {
      term_enums_rm_curr(_s);
    }

  free(_s);
}

void
term_enums_clear(struct _term_enums* _s)
{
  if (_s == NULL)
    err(1, "_s is null");
  while (_s->size != 0)
    {
      term_enums_rm_curr(_s);
    }
}

void
term_enums_print(struct _term_enums* _s)
{
  int i;
  for (i = 0; i < _s->size; i++)
    {
      _s->curr = _s->curr->prev;
    }
}

