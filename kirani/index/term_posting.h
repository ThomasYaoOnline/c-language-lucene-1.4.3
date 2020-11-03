/*
 * author: Thomas Yao
 */

#ifndef _TERM_POSTING_H_
#define _TERM_POSTING_H_

#include <wchar.h>

#include "../utils/int_list.h"

struct _term
{
  char* field;
  char* text;
};

struct _term*
term_initial(char* _fld, char* _txt);

int
term_compare_to(struct _term* tm, struct _term* other);

int
term_equals(struct _term* _tm1, struct _term* _tm2);

void
term_set(struct _term* _tm, char* _fd, char* _txt);

struct _posting
{
  struct _term* term;
  int freq;
  struct _int_list* positions;
};

struct _posting*
posting_initial(struct _term* _tm, int _position);

struct _term_postings_node
{
  struct _term_postings_node* next;
  struct _term_postings_node* prev;
  struct _term* term;
  struct _posting* posting;
  int index;
};

struct _term_postings
{
  struct _term_postings_node* curr;
  int size;
};

struct _term_postings*
term_postings_initial();

void
term_postings_add(struct _term_postings* _tps, struct _term* _tm,
    struct _posting* _pst);


struct _posting*
term_postings_get_postings_by_term(struct _term_postings* _tps, struct _term* _tm);

struct _term*
term_postings_get_term_index(struct _term_postings* s, int _index);

struct _posting*
term_postings_get_posting_index(struct _term_postings* s, int _index);

struct _posting
term_postings_get_posting_index2(struct _term_postings* s, int _index);

void
term_postings_clear(struct _term_postings* _tps);

void
term_postings_destory(struct _term_postings* _tps);

#endif

