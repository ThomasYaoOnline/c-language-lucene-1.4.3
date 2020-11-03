/*
 * author: Thomas Yao
 */

#ifndef _WCHAR_LIST_H_
#define _WCHAR_LIST_H_

#include <wchar.h>

struct _wchar_list_node
{
  struct _wchar_list_node* next;
  struct _wchar_list_node* prev;
  wchar_t* data;
  int index;
};

struct _wchar_list
{
  struct _wchar_list_node* curr;
  int size;
};

struct _wchar_list*
wchar_list_initial();

void
wchar_list_add(struct _wchar_list* _s, wchar_t* _data);

struct _wchar_list*
wchar_list_initial_by_array(wchar_t* _array[], int _size);

void
wchar_list_rm_curr(struct _wchar_list* _s);

wchar_t*
wchar_list_get_index(struct _wchar_list* s, int _index);

void
wchar_list_destory(struct _wchar_list* _s);

void
wchar_list_rmall_by_data(struct _wchar_list* _s, wchar_t* _data);

void
wchar_list_print(struct _wchar_list* _s);

#endif

