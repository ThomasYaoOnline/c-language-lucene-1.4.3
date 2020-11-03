/*
 * author: Thomas Yao
 */

#include <err.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "term_posting.h"

struct _term*
term_initial(char* _fld, char* _txt)
{
  struct _term* tm = (struct _term*) malloc(sizeof(struct _term));
  if (tm == NULL)
    err(1, "tm is null");
  tm->field = _fld;
  tm->text = _txt;
  return tm;
}

int
term_compare_to(struct _term* term, struct _term* other)
{
  int i = strcmp(term->field, other->field);
  if (0 == i)
    return strcmp(term->text, other->text);
  else
    return i;
}

int
term_equals(struct _term* _tm1, struct _term* _tm2)
{
  if (_tm2 == NULL)
    return 0;
  return (strcmp(_tm1->field, _tm2->field) != 0 && strcmp(_tm1->text,
      _tm2->text) != 0);
}

void
term_set(struct _term* _tm, char* _fd, char* _txt)
{
  _tm->field = _fd;
  _tm->text = _txt;
}

struct _posting*
posting_initial(struct _term* term, int position)
{
  struct _posting* p = (struct _posting*) malloc(sizeof(struct _posting));
  if (p == NULL)
    err(1, "p is null");
  p->term = term;
  p->freq = 1;
  p->positions = int_list_initial();
  int_list_add(p->positions, position);
  return p;
}

struct _term_postings*
term_postings_initial()
{
  struct _term_postings* tps = (struct _term_postings*) malloc(
      sizeof(struct _term_postings));
  if (tps == NULL)
    err(1, "tps is null");
  tps->curr = NULL;
  tps->size = 0;
  return tps;
}

void
term_postings_add(struct _term_postings* s, struct _term* _tm,
    struct _posting* _pst)
{
  if (s == NULL)
    err(1, "_tps is null");
  struct _term_postings_node* node = (struct _term_postings_node*) malloc(
      sizeof(struct _term_postings_node));

  if (node == NULL)
    err(1, "node is null");

  if (s->size == 0)
    {
      s->curr = node;
      node->next = node;
      node->prev = node;
    }

  struct _term_postings_node* cn = s->curr;
  struct _term_postings_node* nn = s->curr->next;

  cn->next = node;
  nn->prev = node;

  node->next = nn;
  node->prev = cn;

  node->term = _tm;
  node->posting = _pst;

  s->curr = node;
  s->curr->index = s->size++;
}

struct _posting*
term_postings_get_postings_by_term(struct _term_postings* s, struct _term* _term)
{
  struct _term* term;
  int i;
  for (i = 0; i < s->size; i++)
    {
      term = term_postings_get_term_index(s, i);
      if (term_compare_to(term, _term) == 0)
        return term_postings_get_posting_index(s, i);
    }
  return NULL;
}

struct _term*
term_postings_get_term_index(struct _term_postings* s, int _index)
{
  int i;
  for (i = 0; i < s->size; i++)
    {
      if (s->curr->index == _index)
        return s->curr->term;
      s->curr = s->curr->next;
    }
  return NULL;
}

struct _posting*
term_postings_get_posting_index(struct _term_postings* s, int _index)
{
  int i;
  for (i = 0; i < s->size; i++)
    {
      if (s->curr->index == _index)
        return s->curr->posting;
      s->curr = s->curr->next;
    }
  return NULL;
}

struct _posting
term_postings_get_posting_index2(struct _term_postings* s, int _index)
{
  int i;
  for (i = 0; i < s->size; i++)
    {
      if (s->curr->index == _index)
        return *s->curr->posting;
      s->curr = s->curr->next;
    }
}

void
term_postings_rm_curr(struct _term_postings* s)
{
  if (s == NULL)
    err(1, "_tps is null");

  struct _term_postings_node* cn = s->curr;
  cn->prev->next = cn->next;
  cn->next->prev = cn->prev;

  s->curr = cn->prev;
  s->size--;
  free(cn);
}

void
term_postings_clear(struct _term_postings* s)
{
  if (s == NULL)
    err(1, "_tps is null");
  while (s->size != 0)
    {
      term_postings_rm_curr(s);
    }
}

void
term_postings_destory(struct _term_postings* s)
{
  if (s == NULL)
    err(1, "_tps is null");
  while (s->size != 0)
    {
      term_postings_rm_curr(s);
    }
  free(s);
}

