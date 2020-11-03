/*
 * author: Thomas Yao
 */

#ifndef _NORM_H_
#define _NORM_H_

#include "../utils/int_list.h"

struct _norm
{
  struct _file_stream* in;
  struct _int_list* bytes;
  int dirty;
  int number;
};

struct _norm*
norm_initial(struct _file_stream* _in, int _number);


struct _norms_node
{
  struct _norms_node* next;
  struct _norms_node* prev;
  struct _norm* data;
  int index;
  char* key;
};

struct _norms
{
  struct _norms_node* curr;
  int size;
};

struct _norms*
norms_initial();

void
norms_add(struct _norms* _s, char* _key, struct _norm* _data);

struct _norm*
norms_get_curr(struct _norms* _s);

struct _norm*
norms_get_index(struct _norms* s, int _index);

struct _norm*
norms_get_by_key(struct _norms* _s, char* _key);

void
norms_rm_curr(struct _norms* _s);

void
norms_destory(struct _norms* _s);

void
norms_clear(struct _norms* _s);

void
norms_print(struct _norms* _s);


#endif /* NORM_H_ */
