/*
 * author: Thomas Yao
 */

#include <err.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "segment_info.h"

struct _segment_info*
segment_info_initial(char* _name, int _doc_count, struct _directory* _dir)
{
  struct _segment_info* si = (struct _segment_info*) malloc(
      sizeof(struct _segment_info));
  if (si == NULL)
    err(1, "si is null");

  si->name = _name;
  si->doc_count = _doc_count;
  si->dir = _dir;
  return si;
}

struct _segment_infos*
segment_infos_initial()
{
  struct _segment_infos* s = (struct _segment_infos*) malloc(
      sizeof(struct _segment_infos));
  if (s == NULL)
    err(1, "s is null");
  s->curr = NULL;
  s->size = 0;

  /* default */
  s->counter = 0;
  s->version = 0;
  /* default */
  return s;
}

void
segment_infos_add(struct _segment_infos* s, struct _segment_info* _data)
{
  struct _segment_infos_node* node = (struct _segment_infos_node*) malloc(
      sizeof(struct _segment_infos_node));

  if (node == NULL)
    err(1, "node is null");

  if (s->size == 0)
    {
      node->next = node;
      node->prev = node;
      s->curr = node;
    }

  struct _segment_infos_node* cn = s->curr;
  struct _segment_infos_node* nn = s->curr->next;

  cn->next = node;
  nn->prev = node;

  node->next = nn;
  node->prev = cn;

  node->data = _data;

  s->curr = node;
  s->curr->index = s->size++;
}

struct _segment_info*
segment_infos_get_curr(struct _segment_infos* s)
{
  return s->curr->data;
}

struct _segment_info*
segment_infos_get_index(struct _segment_infos* s, int _index)
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
segment_infos_rm_curr(struct _segment_infos* s)
{
  if (s == NULL)
    err(1, "_s is null");
  struct _segment_infos_node* cn = s->curr;
  cn->prev->next = cn->next;
  cn->next->prev = cn->prev;

  s->curr = cn->prev;
  s->size--;
  free(cn);
}

void
segment_infos_set_size(struct _segment_infos* s, int min_segment)
{
  int i;
  if (min_segment > s->size)
    {
      for (i = s->size; i < min_segment; i++)
        segment_infos_add(s, NULL);
    }
  else
    {
      int j = s->size;
      for (i = min_segment; i < j; i++)
        {
          segment_infos_get_index(s, i);
          segment_infos_rm_curr(s);
        }
    }
}

void
segment_infos_destory(struct _segment_infos* s)
{
  if (s == NULL)
    err(1, "_s is null");
  while (s->size != 0)
    {
      segment_infos_rm_curr(s);
    }
  free(s);
}

void
segment_infos_clear(struct _segment_infos* s)
{
  if (s == NULL)
    err(1, "s is null");
  while (s->size != 0)
    {
      segment_infos_rm_curr(s);
    }
}

void
segment_infos_print(struct _segment_infos* s)
{
  int i;
  for (i = 0; i < s->size; i++)
    {
      s->curr = s->curr->next;
    }
}

void
segment_infos_read(struct _segment_infos* sis, struct _directory* dir)
{
  struct _file_stream* in = directory_open_file(dir, "segments", ".x", "r");

  int format = fs_read_int(in);

  printf("segment_infos_read:format: %d\n", format);

  if (format < 0) // file contains explicit format info
    {
      // check that it is a format we can understand
      if (format < SEGMENT_INFOS_FORMAT)
        {
          err(1, "unknown format version");
        }
      sis->version = fs_read_long(in); // read version

      printf("segment_infos_read:version: %lli\n", sis->version);
      sis->counter = fs_read_int(in); // read counter
      printf("segment_infos_read:counter: %d\n", sis->counter);
    }
  else // file is in old format without explicit format info
    {
      sis->counter = format;
    }

  int j = fs_read_int(in); //read sis->size

  printf("segment_infos_read:sis->size: %d\n", j);
  struct _segment_info* si;
  int i;
  char* name;
  int doc_count;
  for (i = j; i > 0; i--)
    {
      name = fs_read_string(in);
      doc_count = fs_read_int(in);
      si = segment_info_initial(name, doc_count, dir);
      printf("segment_infos_read:si:name: %s\n", si->name);
      printf("segment_infos_read:si:doc_count: %d\n", si->doc_count);
      segment_infos_add(sis, si);
    }

  if (format >= 0)
    {
      sis->version = fs_read_long(in);
    }

  fs_close(in);
}

void
segment_infos_write(struct _segment_infos* sis, struct _directory* dir)
{
  struct _file_stream* out = directory_open_file(dir, "segments", ".x", "w");
  fs_write_int(out, SEGMENT_INFOS_FORMAT); // write FORMAT
  printf("segment_infos_write: format: %d\n", SEGMENT_INFOS_FORMAT);

  fs_write_long(out, ++sis->version); // every write changes the index
  printf("segment_infos_write: version: %lli\n", sis->version);

  fs_write_int(out, sis->counter); // write counter
  printf("segment_infos_write: counter: %d\n", sis->counter);

  fs_write_int(out, sis->size); // write infos
  printf("segment_infos_write: size: %d\n", sis->size);

  // write segmentinfos
  int i;
  struct _segment_info* si;
  for (i = 0; i < sis->size; i++)
    {
      si = segment_infos_get_index(sis, i);
      fs_write_string(out, si->name);
      printf("segment_infos_write: name: %s\n", si->name);

      fs_write_int(out, si->doc_count);
      printf("segment_infos_write: doc_count: %d\n", si->doc_count);
    }
  fs_close(out);
}
