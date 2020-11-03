/*
 * author: Thomas Yao
 */

#ifndef _TERM_INFO_H_
#define _TERM_INFO_H_

struct _term_info
{
  int doc_freq;
  long long freq_pointer;
  long long prox_pointer;
  int skip_offset;
};

struct _term_info*
term_info_initial();

struct _term_info*
term_info_initial2(int _df, long long _fp, long long _pp);

struct _term_info*
term_info_initial_copy(struct _term_info* _ti2);

void
term_info_set(struct _term_info* _ti, int _df, long long _fp, long long _pp, int _so);

void
term_info_set_copy(struct _term_info* _ti1, struct _term_info* _ti2);

struct _term_infos_node
{
  struct _term_infos_node* next;
  struct _term_infos_node* prev;
  struct _term_info* data;
  int index;
};

struct _term_infos
{
  struct _term_infos_node* curr;
  int size;
};

struct _term_infos*
term_infos_initial();

void
term_infos_add(struct _term_infos* _s, struct _term_info* _tvf);

struct _term_info*
term_infos_get_index(struct _term_infos* s, int _index);

void
term_infos_rm_curr(struct _term_infos* _s);

void
term_infos_destory(struct _term_infos* _s);

void
term_infos_clear(struct _term_infos* _s);

void
term_infos_print(struct _term_infos* _s);

#endif

