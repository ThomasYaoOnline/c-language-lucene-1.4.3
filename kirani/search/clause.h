/*
 * author: Thomas Yao
 */

#ifndef _CLAUSE_H_
#define _CLAUSE_H_

#include "query.h"

struct _clause
{
  struct _query* qry;
  int required;
  int prohibited;
};

struct _clauses_node
{
  struct _clauses_node* next;
  struct _clauses_node* prev;
  struct _clause* data;
  int index;
};

struct _clauses
{
  struct _clauses_node* curr;
  int size;
};

struct _clause*
clause_initial(struct _query* _qry, int r, int p);

struct _clauses*
clauses_initial();

void
clauses_add(struct _clauses* _s, struct _clause* _data);

struct _clause*
clauses_get_curr(struct _clauses* _s);

struct _clause*
clauses_get_index(struct _clauses* s, int _index);

void
clauses_rm_curr(struct _clauses* _s);

void
clauses_destory(struct _clauses* _s);

void
clauses_clear(struct _clauses* _s);

void
clauses_print(struct _clauses* _s);

#endif
