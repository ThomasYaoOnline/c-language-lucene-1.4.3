/*
 * author: Thomas Yao
 */

#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "segment_merge_info.h"

struct _segment_merge_info*
segment_merge_info_initial(int _base, struct _term_enum* _term_enum,
    struct _index_reader* _ir)
{
  struct _segment_merge_info* smi = (struct _segment_merge_info*) malloc(
      sizeof(struct _segment_merge_info));

  if (smi == NULL)
    err(1, "smi is null");

  smi->base = _base;
  smi->ir = _ir;
  smi->term_enum = _term_enum;
  smi->term = _term_enum->term;
  smi->postings = term_docs_initial(_ir);
  return smi;
}

int
segment_merger_info_next(struct _segment_merge_info* smi)
{
  if (term_enum_next(smi->term_enum))
    {
      smi->term = smi->term_enum->term;
      return 1;
    }
  else
    {
      smi->term = NULL;
      return 0;
    }
}

void
segment_merge_info_close(struct _segment_merge_info* smi)
{
  term_enum_close(smi->term_enum);
  term_docs_close(smi->postings);
}

struct _segment_merge_infos*
segment_merge_infos_initial()
{
  struct _segment_merge_infos* tvfs = (struct _segment_merge_infos*) malloc(
      sizeof(struct _segment_merge_infos));
  if (tvfs == NULL)
    err(1, "tvfs is null");
  tvfs->curr = NULL;
  tvfs->size = 0;
  return tvfs;
}

void
segment_merge_infos_add(struct _segment_merge_infos* _s,
    struct _segment_merge_info* _data)
{
  struct _segment_merge_infos_node* node =
      (struct _segment_merge_infos_node*) malloc(
          sizeof(struct _segment_merge_infos_node));
  if (node == NULL)
    err(1, "node is null");

  if (_s->size == 0)
    {
      node->next = node;
      node->prev = node;
      _s->curr = node;
    }

  struct _segment_merge_infos_node* cn = _s->curr;
  struct _segment_merge_infos_node* nn = _s->curr->next;

  cn->next = node;
  nn->prev = node;

  node->next = nn;
  node->prev = cn;

  node->data = _data;

  _s->curr = node;
  _s->curr->index = _s->size++;
}

struct _segment_merge_info*
segment_merge_infos_get_curr(struct _segment_merge_infos* _s)
{
  return _s->curr->data;
}

struct _segment_merge_info*
segment_merge_infos_get_index(struct _segment_merge_infos* s, int _index)
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

struct _segment_merge_info
segment_merge_infos_get_index2(struct _segment_merge_infos* s, int _index)
{
  int i;
  for (i = 0; i < s->size; i++)
    {
      if (s->curr->index == _index)
        return *s->curr->data;
      s->curr = s->curr->next;
    }
}

void
segment_merge_infos_set_index(struct _segment_merge_infos* s, int _index,
    struct _segment_merge_info* _data)
{
  int i;
  for (i = 0; i < s->size; i++)
    {
      if (s->curr->index == _index)
        {
          s->curr->data = _data;
          return;
        }
      s->curr = s->curr->next;
    }
  err(1, "no such index");
}

int
compare_smis(struct _segment_merge_info* p1, struct _segment_merge_info* p2)
{
  int comparison = term_compare_to(p1->term, p2->term);
  if (comparison == 0)
    return p1->base < p2->base;
  else
    return comparison < 0;
}

void
sort(struct _segment_merge_infos* _s)
{
  struct _segment_merge_info smis[100];
  int i;
  int j = _s->size;
  for (i = 0; i < j; i++)
    {
      smis[i] = segment_merge_infos_get_index2(_s, i);
    }

  qsort(&smis, _s->size, sizeof(struct _segment_merge_info), compare_smis);

  segment_merge_infos_clear(_s);

  for (i = 0; i < j; i++)
    {
      segment_merge_infos_add(_s, &smis[i]);
    }
}

struct _segment_merge_info*
segment_merge_infos_pop(struct _segment_merge_infos* s)
{
  sort(s);
  int i;
  for (i = 0; i < s->size; i++)
    {
      if (s->curr->index == s->size - 1)
        {
          struct _segment_merge_info* result = s->curr->data;
          struct _segment_merge_infos_node* cn = s->curr;

          cn->prev->next = cn->next;
          cn->next->prev = cn->prev;

          s->curr = cn->prev;
          s->size--;

          return result;
        }
      s->curr = s->curr->next;
    }
  return NULL;
}

struct _segment_merge_info*
segment_merge_infos_top(struct _segment_merge_infos* s)
{
  sort(s);
  int i;
  for (i = 0; i < s->size; i++)
    {
      if (s->curr->index == s->size - 1)
        return s->curr->data;
      s->curr = s->curr->next;
    }
  return NULL;
}

void
segment_merge_infos_rm_curr(struct _segment_merge_infos* _s)
{
  if (_s == NULL)
    err(1, "_s is null");

  struct _segment_merge_infos_node* cn = _s->curr;

  cn->prev->next = cn->next;
  cn->next->prev = cn->prev;

  _s->curr = cn->prev;
  _s->size--;

  free(cn);
}

void
segment_merge_infos_destory(struct _segment_merge_infos* _s)
{
  if (_s == NULL)
    err(1, "_s is null");

  while (_s->size != 0)
    {
      segment_merge_infos_rm_curr(_s);
    }

  free(_s);
}

void
segment_merge_infos_clear(struct _segment_merge_infos* _s)
{
  if (_s == NULL)
    err(1, "_s is null");
  while (_s->size != 0)
    {
      segment_merge_infos_rm_curr(_s);
    }
}

void
segment_merge_infos_print(struct _segment_merge_infos* _s)
{
  int i;
  for (i = 0; i < _s->size; i++)
    {
      _s->curr = _s->curr->prev;
    }
}

void
segment_merge_infos_close(struct _segment_merge_infos* _s)
{

}

