/*
 * author: Thomas Yao
 */

#ifndef _CHAR_LIST_H_
#define _CHAR_LIST_H_

struct _char_list_node
{
  struct _char_list_node* next;
  struct _char_list_node* prev;
  char* data;
  int index;
};

struct _char_list
{
  struct _char_list_node* curr;
  int size;
};

struct _char_list*
char_list_initial();

void
char_list_add(struct _char_list* _s, char* _data);

char*
char_list_get_curr(struct _char_list* _s);

char*
char_list_get_index(struct _char_list* s, int _index);

char*
char_list_pop(struct _char_list* _s);

struct _char_list*
char_list_initial_by_array(char* _array[], int _size);

void
char_list_rm_curr(struct _char_list* _s);

void
char_list_destory(struct _char_list* _s);

void
char_list_rmall_by_data(struct _char_list* _s, char* _data);

void
char_list_print(struct _char_list* _s);

#endif

