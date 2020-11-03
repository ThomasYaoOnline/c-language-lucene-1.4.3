/*
 * author: Thomas Yao
 */

#ifndef _INT_LIST_H_
#define _INT_LIST_H_

struct _int_list_node
{
  struct _int_list_node* next;
  struct _int_list_node* prev;
  int data;
  int index;
};

struct _int_list
{
  struct _int_list_node* curr;
  int size;
};

struct _int_list*
int_list_initial();

struct _int_list*
int_list_initial_fill(int length, int value);

void
int_list_add(struct _int_list* _s, int _data);

struct _int_list*
int_list_initial_by_array(int _array[], int _size);

int
int_list_get_curr(struct _int_list* _s);

int
int_list_get_index(struct _int_list* s, int _index);

int
int_list_set_index(struct _int_list* s, int _index, int value);

void
int_list_rm_curr(struct _int_list* _s);

void
int_list_destory(struct _int_list* _s);

void
int_list_rmall_by_data(struct _int_list* _s, int _data);

void
int_list_print(struct _int_list* _s);

#endif

