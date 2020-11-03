/*
 * author: Thomas Yao
 */

#ifndef _DOCUMENT_H_
#define _DOCUMENT_H_

#include "field.h"

struct _document
{
  struct _fields* fields;
  float boost;
};

struct _document*
document_initial();

void
document_add(struct _document* doc, struct _field* field);

void
document_remove_field(struct _document* doc, char* _name);

struct _field*
document_get_field(struct _document* doc, char* _name);

#endif

