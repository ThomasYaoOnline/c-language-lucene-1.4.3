/*
 * author: Thomas Yao
 */

#ifndef _SEGMENT_MERGE_INFO_H_
#define _SEGMENT_MERGE_INFO_H_

#include "term_posting.h"
#include "term_enum.h"
#include "index_reader.h"
#include "term_docs.h"

#include "../utils/int_list.h"

struct _segment_merge_info
{
  struct _term* term;
  int base;
  struct _term_enum* term_enum;
  struct _index_reader* ir;
  struct _term_docs* postings;
  struct _int_list* doc_map;
};

struct _segment_merge_info*
segment_merge_info_initial(int _base, struct _term_enum* _term_enum,
    struct _index_reader* _ir);

int
segment_merger_info_next(struct _segment_merge_info* smi);
void
segment_merge_info_close(struct _segment_merge_info* smi);

struct _segment_merge_infos_node
{
  struct _segment_merge_infos_node* next;
  struct _segment_merge_infos_node* prev;
  struct _segment_merge_info* data;
  int index;
};

struct _segment_merge_infos
{
  struct _segment_merge_infos_node* curr;
  int size;
};

struct _segment_merge_infos*
segment_merge_infos_initial();

void
segment_merge_infos_add(struct _segment_merge_infos* _s,
    struct _segment_merge_info* _tvf);

struct _segment_merge_info*
segment_merge_infos_get_curr(struct _segment_merge_infos* _s);

struct _segment_merge_info*
segment_merge_infos_get_index(struct _segment_merge_infos* s, int _index);

struct _segment_merge_info
segment_merge_infos_get_index2(struct _segment_merge_infos* s, int _index);

void
segment_merge_infos_set_index(struct _segment_merge_infos* s, int _index,
    struct _segment_merge_info* _data);

struct _segment_merge_info*
segment_merge_infos_pop(struct _segment_merge_infos* s);

struct _segment_merge_info*
segment_merge_infos_top(struct _segment_merge_infos* s);

void
segment_merge_infos_rm_curr(struct _segment_merge_infos* _s);

void
segment_merge_infos_destory(struct _segment_merge_infos* _s);

void
segment_merge_infos_clear(struct _segment_merge_infos* _s);

void
segment_merge_infos_print(struct _segment_merge_infos* _s);

void
segment_merge_infos_close(struct _segment_merge_infos* _s);

#endif /* SEGMENT_MERGE_INFO_H_ */
