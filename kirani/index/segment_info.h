/*
 * author: Thomas Yao
 */

#ifndef _SEGMENT_INFO_H_
#define _SEGMENT_INFO_H_

#include "../store/directory.h"

#define SEGMENT_INFOS_FORMAT -1

struct _segment_info
{
  char* name;
  int doc_count;
  struct _directory* dir;
};

struct _segment_info*
segment_info_initial(char* _name, int _doc_count, struct _directory* _dir);

struct _segment_infos_node
{
  struct _segment_infos_node* next;
  struct _segment_infos_node* prev;
  struct _segment_info* data;
  int index;
};

struct _segment_infos
{
  struct _segment_infos_node* curr;
  int size;
  int counter;
  long long version;
};

struct _segment_infos*
segment_infos_initial();

void
segment_infos_add(struct _segment_infos* _s, struct _segment_info* _data);

struct _segment_info*
segment_infos_get_curr(struct _segment_infos* _s);

struct _segment_info*
segment_infos_get_index(struct _segment_infos* s, int index);

void
segment_infos_rm_curr(struct _segment_infos* _s);

void
segment_infos_set_size(struct _segment_infos* s, int min_segment);

void
segment_infos_destory(struct _segment_infos* _s);

void
segment_infos_clear(struct _segment_infos* _s);

void
segment_infos_print(struct _segment_infos* _s);


void
segment_infos_read(struct _segment_infos* _sis, struct _directory* _dir);

void
segment_infos_write(struct _segment_infos* _sis, struct _directory* _dir);

#endif

