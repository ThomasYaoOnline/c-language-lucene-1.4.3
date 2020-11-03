/*
 * author: Thomas Yao
 */

#include <stdio.h>
#include <stdlib.h>

struct _field
{
  char* a;
};

struct list
{
  struct _field* _field;
  int size;
};

void
struct_test_main()
{
  struct _field* _field = (struct _field*) malloc(sizeof(struct _field));
  struct list* _list = (struct list*) malloc(sizeof(struct list));

  _list->_field = _field;
  _list->_field->a = "hello";

  printf("%s\n", _list->_field->a);
}

void
struct_test_norm()
{
/*  struct _norms* nms = norms_initial();
  struct _norm* nm = norm_initial();*/
}
