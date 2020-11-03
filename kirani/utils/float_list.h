/*
 * author: Thomas Yao
 */

#ifndef _FLOAT_LIST_H_
#define _FLOAT_LIST_H_

struct _float_list_node
{
  struct _float_list_node* next;
  struct _float_list_node* prev;
  float data;
  int index;
};

struct _float_list
{
  struct _float_list_node* curr;
  int size;
};

struct _float_list*
float_list_initial();

struct _float_list*
float_list_initial_fill(int length, float value);

void
float_list_add(struct _float_list* _s, float _data);

struct _float_list*
float_list_initial_by_array(float _array[], int _size);

float
float_list_get_curr(struct _float_list* _s);

float
float_list_get_index(struct _float_list* s, int _index);

int
float_list_set_index(struct _float_list* s, int _index, float value);

float
float_list_pop(struct _float_list* _s);

void
float_list_rm_curr(struct _float_list* _s);

void
float_list_destory(struct _float_list* _s);

void
float_list_rmall_by_data(struct _float_list* _s, float _data);

void
float_list_print(struct _float_list* _s);

#endif
