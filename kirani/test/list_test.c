/*
 * author: Thomas Yao
 */


#include "../document/field.h"

void list_test_main()
{
  struct _field* fd = field_initial("path", "filename", NULL, 1, 1, 1, 0);
  struct _fields* s = fields_initial();
  fields_add(s, fd);
  fields_add(s, fd);
  fields_add(s, fd);
  fields_add(s, fd);
  fields_add(s, fd);
  fields_add(s, fd);
  fields_print(s);
}

