/*
 * author: Thomas Yao
 */

#include <err.h>
#include <stdio.h>
#include <stdlib.h>

#include "hits.h"
#include "top_docs.h"

struct _hit_doc*
hit_doc_initial(float _score, int _id)
{
  struct _hit_doc* hitdoc = (struct _hit_doc*) malloc(sizeof(struct _hit_doc));
  if (hitdoc == NULL)
    err(1, "hitdoc is null");
  hitdoc->score = _score;
  hitdoc->id = _id;
  hitdoc->doc = NULL;
  return hitdoc;
}

struct _hit_docs*
hit_docs_initial()
{
  struct _hit_docs* s = (struct _hit_docs*) malloc(sizeof(struct _hit_docs));
  if (s == NULL)
    err(1, "s is null");
  s->curr = NULL;
  s->size = 0;
  return s;
}

void
hit_docs_add(struct _hit_docs* _s, struct _hit_doc* _data)
{
  struct _hit_docs_node* node = (struct _hit_docs_node*) malloc(
      sizeof(struct _hit_docs_node));
  if (node == NULL)
    err(1, "node is null");

  if (_s->size == 0)
    {
      node->next = node;
      node->prev = node;
      _s->curr = node;
    }

  struct _hit_docs_node* cn = _s->curr;
  struct _hit_docs_node* nn = _s->curr->next;

  cn->next = node;
  nn->prev = node;

  node->next = nn;
  node->prev = cn;

  node->data = _data;

  _s->curr = node;
  _s->curr->index = _s->size++;
}

struct _hit_doc*
hit_docs_get_curr(struct _hit_docs* _s)
{
  return _s->curr->data;
}

struct _hit_doc*
hit_docs_get_index(struct _hit_docs* s, int _index)
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
hit_docs_rm_curr(struct _hit_docs* s)
{
  if (s == NULL)
    err(1, "_s is null");
  struct _hit_docs_node* cn = s->curr;
  cn->prev->next = cn->next;
  cn->next->prev = cn->prev;

  s->curr = cn->prev;
  s->size--;
  free(cn);
}

void
hit_docs_destory(struct _hit_docs* _s)
{
  if (_s == NULL)
    err(1, "_s is null");
  while (_s->size != 0)
    {
      hit_docs_rm_curr(_s);
    }
  free(_s);
}

void
hit_docs_clear(struct _hit_docs* _s)
{
  if (_s == NULL)
    err(1, "_s is null");
  while (_s->size != 0)
    {
      hit_docs_rm_curr(_s);
    }
}

void
hit_docs_print(struct _hit_docs* _s)
{
  int i;
  for (i = 0; i < _s->size; i++)
    {
      _s->curr = _s->curr->prev;
    }
}

void
hits_get_more_docs(struct _hits* hits, int min)
{
  printf("hits_get_more_docs:hit_docs:size: %d\n", hits->hit_docs->size);
  if (hits->hit_docs->size > min)
    min = hits->hit_docs->size;

  int n = min * 2; //double # retrieved

  struct _top_docs* topdocs = index_searcher_search_top_docs(hits->is,
      hits->query, hits->filter, n, hits->sort);
  hits->length = topdocs->total_hits;

  struct _score_doc* scoredoc;
  struct _score_docs* scoredocs = topdocs->score_docs;
  printf("hits_get_more_docs:scoredocs:size: %d\n", scoredocs->size);

  float score_norm = 1.0f;
  scoredoc = score_docs_get_index(scoredocs, 0);
  if (hits->length > 0 && scoredoc->score > 1.0f)
    {
      score_norm = 1.0f / scoredoc->score;
    }

  int end = scoredocs->size < hits->length ? scoredocs->size : hits->length;
  int i;
  for (i = hits->hit_docs->size; i < end; i++)
    {
      scoredoc = score_docs_get_index(scoredocs, i);
      hit_docs_add(hits->hit_docs, hit_doc_initial(
          scoredoc->score * score_norm, scoredoc->doc));
    }
}

struct _hits*
hits_initial(struct _index_searcher* _is, struct _query* query,
    struct _filter* _flt)
{
  struct _hits* ht = (struct _hits*) malloc(sizeof(struct _hits));
  if (ht == NULL)
    err(1, "ht is null");

  /* defaults */
  ht->hit_docs = hit_docs_initial();
  ht->num_docs = 0;
  ht->max_docs = 200;
  /* defaults */

  ht->query = query;
  ht->is = _is;
  ht->filter = _flt;

  hits_get_more_docs(ht, 50);
  return ht;
}

struct _document*
hits_doc(struct _hits* hits, int n)
{
  struct _hit_doc* hit_doc = hit_docs_get_index(hits->hit_docs, n);
  if (hit_doc->doc == NULL)
    {
      hit_doc->doc = index_searcher_doc(hits->is, hit_doc->id);
    }
  return hit_doc->doc;
}
