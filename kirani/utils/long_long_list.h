/*
 * author: Thomas Yao
 */

#ifndef _LONG_LONG_LIST_H_
#define _LONG_LONG_LIST_H_

struct _long_long_list_node
{
  struct _long_long_list_node* next;
  struct _long_long_list_node* prev;
  long long data;
  int index;
};

struct _long_long_list
{
  struct _long_long_list_node* curr;
  int size;
};

struct _long_long_list*
long_long_list_initial();

struct _long_long_list*
long_long_list_initial_fill(int length, long long value);

void
long_long_list_add(struct _long_long_list* _s, long long _data);

struct _long_long_list*
long_long_list_initial_by_array(long long _array[], int _size);

long long
long_long_list_get_curr(struct _long_long_list* _s);

long long
long_long_list_get_index(struct _long_long_list* s, int _index);

int
long_long_list_set_index(struct _long_long_list* s, int _index, long long value);

void
long_long_list_rm_curr(struct _long_long_list* _s);

void
long_long_list_destory(struct _long_long_list* _s);

void
long_long_list_rmall_by_data(struct _long_long_list* _s, long long _data);

void
long_long_list_print(struct _long_long_list* _s);

#endif /* LONG_LONG_LIST_H_ */
