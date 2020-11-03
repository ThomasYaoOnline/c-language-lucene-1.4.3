/*
 * author: Thomas Yao
 */

#include <err.h>
#include <stdlib.h>
#include <wchar.h>

#include "document.h"

struct _document*
document_initial()
{
  struct _document* doc = (struct _document*) malloc(sizeof(struct _document));
  if (doc == NULL)
    err(1, "doc is null");
  doc->fields = fields_initial();
  doc->boost = 1.0f;
  return doc;
}

void
document_add(struct _document* doc, struct _field* _field)
{
  fields_add(doc->fields, _field);
}

void
document_remove_field(struct _document* _document, char* _name)
{
  fields_rmall_by_data(_document->fields, _name);
}

struct _field*
document_get_field(struct _document* _document, char* _name)
{
  return fields_get_by_name(_document->fields, _name);
}

