/*
 * author: Thomas Yao
 */

#include <err.h>
#include <stdio.h>
#include <stdlib.h>

#include "term_info.h"

struct _term_info*
term_info_initial()
{
  struct _term_info* tmif = (struct _term_info*) malloc(
      sizeof(struct _term_info));
  if (tmif == NULL)
    err(1, "tmif is null");
  tmif->doc_freq = 0;
  tmif->freq_pointer = 0;
  tmif->prox_pointer = 0;

  return tmif;
}

struct _term_info*
term_info_initial2(int df, long long fp, long long pp)
{
  struct _term_info* ti = term_info_initial();

  if (ti == NULL)
    err(1, "term_info is null");
  ti->doc_freq = df;
  ti->freq_pointer = fp;
  ti->prox_pointer = pp;
  return ti;
}

struct _term_info*
term_info_initial_copy(struct _term_info* _ti2)
{
  if (_ti2 == NULL)
    err(1, "ti2 is null");
  struct _term_info* ti = term_info_initial();

  if (ti == NULL)
    err(1, "ti is null");

  ti->doc_freq = _ti2->doc_freq;
  ti->freq_pointer = _ti2->freq_pointer;
  ti->prox_pointer = _ti2->prox_pointer;
  ti->skip_offset = _ti2->skip_offset;
  return ti;
}

void
term_info_set(struct _term_info* _ti, int _doc_freq, long long _freq_pointer,
    long long _prox_pointer, int _skip_offset)
{
  _ti->doc_freq = _doc_freq;
  _ti->freq_pointer = _freq_pointer;
  _ti->prox_pointer = _prox_pointer;
  _ti->skip_offset = _skip_offset;
}

void
term_info_set_copy(struct _term_info* _ti, struct _term_info* _ti2)
{
  if (_ti == NULL)
    err(1, "ti1 is null");
  if (_ti2 == NULL)
    err(1, "ti2 is null");

  _ti->doc_freq = _ti2->doc_freq;
  _ti->freq_pointer = _ti2->freq_pointer;
  _ti->prox_pointer = _ti2->prox_pointer;
  _ti->skip_offset = _ti2->skip_offset;
}

struct _term_infos*
term_infos_initial()
{
  struct _term_infos* tvfs = (struct _term_infos*) malloc(
      sizeof(struct _term_infos));
  if (tvfs == NULL)
    err(1, "tvfs is null");
  tvfs->curr = NULL;
  tvfs->size = 0;
  return tvfs;
}

void
term_infos_add(struct _term_infos* _s, struct _term_info* _data)
{
  struct _term_infos_node* node = (struct _term_infos_node*) malloc(
      sizeof(struct _term_infos_node));
  if (node == NULL)
    err(1, "node is null");

  if (_s->size == 0)
    {
      node->next = node;
      node->prev = node;
      _s->curr = node;
    }

  struct _term_infos_node* cn = _s->curr;
  struct _term_infos_node* nn = _s->curr->next;

  cn->next = node;
  nn->prev = node;

  node->next = nn;
  node->prev = cn;

  node->data = _data;

  _s->curr = node;
  _s->curr->index = _s->size++;
}

struct _term_info*
term_infos_get_index(struct _term_infos* s, int _index)
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
term_infos_rm_curr(struct _term_infos* _s)
{
  if (_s == NULL)
    err(1, "_s is null");

  struct _term_infos_node* cn = _s->curr;

  cn->prev->next = cn->next;
  cn->next->prev = cn->prev;

  _s->curr = cn->prev;
  _s->size--;

  free(cn);
}

void
term_infos_destory(struct _term_infos* _s)
{
  if (_s == NULL)
    err(1, "_s is null");

  while (_s->size != 0)
    {
      term_infos_rm_curr(_s);
    }

  free(_s);
}

void
term_infos_clear(struct _term_infos* _s)
{
  if (_s == NULL)
    err(1, "_s is null");
  while (_s->size != 0)
    {
      term_infos_rm_curr(_s);
    }
}

void
term_infos_print(struct _term_infos* _s)
{
  int i;
  for (i = 0; i < _s->size; i++)
    {
      _s->curr = _s->curr->prev;
    }
}
