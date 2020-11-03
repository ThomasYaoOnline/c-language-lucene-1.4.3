/*
 * author: Thomas Yao
 */

#ifndef _EXPLANATION_H_
#define _EXPLANATION_H_

struct _explanation
{
  float value;
  char* description;
  struct _explanations* details;
};

struct _explanations_node
{
  struct _explanations_node* next;
  struct _explanations_node* prev;
  struct _explanation* data;
  int index;
};

struct _explanations
{
  struct _explanations_node* curr;
  int size;
};

struct _explanations*
explanations_initial();

void
explanations_add(struct _explanations* _s, struct _explanation* _data);

struct _explanation*
explanations_get_curr(struct _explanations* _s);

struct _explanation*
explanations_get_index(struct _explanations* s, int _index);

void
explanations_rm_curr(struct _explanations* _s);

void
explanations_destory(struct _explanations* _s);

void
explanations_clear(struct _explanations* _s);

void
explanations_print(struct _explanations* _s);

#endif

