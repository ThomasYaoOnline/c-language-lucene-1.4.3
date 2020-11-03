/*
 * author: Thomas Yao
 */

#ifndef _TV_FIELD_H_
#define _TV_FIELD_H_

struct _tv_field
{
  int number;
  long long tvf_pointer;
  int length;
};

struct _tv_field*
tv_field_initial(int _number);

struct _tv_fields_node
{
  struct _tv_fields_node* next;
  struct _tv_fields_node* prev;
  struct _tv_field* data;
  int index;
};

struct _tv_fields
{
  struct _tv_fields_node* curr;
  int size;
};

struct _tv_fields*
tv_fields_initial();

void
tv_fields_add(struct _tv_fields* _s, struct _tv_field* _tvf);

struct _tv_field*
tv_fields_get_curr(struct _tv_fields* _s);

struct _tv_field*
tv_fields_get_index(struct _tv_fields* s, int _index);

void
tv_fields_rm_curr(struct _tv_fields* _s);

void
tv_fields_destory(struct _tv_fields* _s);

void
tv_fields_clear(struct _tv_fields* _s);

void
tv_fields_print(struct _tv_fields* _s);

#endif

