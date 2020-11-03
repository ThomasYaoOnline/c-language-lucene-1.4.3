/*
 * author: Thomas Yao
 */

#ifndef _TV_TERM_H_
#define _TV_TERM_H_

#include <wchar.h>

struct _tv_term
{
  char* term_text;
  int freq;
};

struct _tv_term*
tv_term_initial();

struct _tv_terms_node
{
  struct _tv_terms_node* next;
  struct _tv_terms_node* prev;
  struct _tv_term* data;
  int index;
};

struct _tv_terms
{
  struct _tv_terms_node* curr;
  int size;
};

struct _tv_terms*
tv_terms_initial();

void
tv_terms_add(struct _tv_terms* _s, struct _tv_term* _data);

struct _tv_term*
tv_terms_get_curr(struct _tv_terms* _s);

struct _tv_term*
tv_terms_get_index(struct _tv_terms* s, int _index);


void
tv_terms_rm_curr(struct _tv_terms* _s);

void
tv_terms_destory(struct _tv_terms* _s);

void
tv_terms_clear(struct _tv_terms* _s);

void
tv_terms_print(struct _tv_terms* _s);

#endif

