/*
 * author: Thomas Yao
 */

#include <err.h>
#include <stdio.h>
#include <stdlib.h>

#include "fields_writer.h"

#include "../document/field.h"
#include "../store/directory.h"
#include "../utils/utils.h"

struct _fields_writer*
fields_writer_initial(struct _directory* dir, char* segment,
    struct _field_infos* fis)
{
  struct _fields_writer* fw = (struct _fields_writer*) malloc(
      sizeof(struct _fields_writer));
  if (fw == NULL)
    err(1, "fw is null");
  fw->fieldinfos = fis;
  fw->fieldsstream = directory_open_file(dir, segment, ".fdt", "w");
  fw->indexstream = directory_open_file(dir, segment, ".fdx", "w");
  return fw;
}

void
fields_writer_add_document(struct _fields_writer* fw, struct _document* doc)
{
  fs_write_long(fw->indexstream, fs_tell(fw->fieldsstream));
  printf("fields_writer_add_document:fs_tell: %lli\n", fs_tell(fw->fieldsstream));

  int stored_count = 0;
  struct _field* field;

  int i;
  for (i = 0; i < doc->fields->size; i++)
    {
      field = fields_get_index(doc->fields, i);
      if (field->is_stored)
        {
          stored_count++;
        }
    }
  fs_write_vint(fw->fieldsstream, stored_count);
  printf("fields_writer_add_document:stroed_count: %i\n", stored_count);


  for (i = 0; i < doc->fields->size; i++)
    {
      field = fields_get_index(doc->fields, i);
      if (field->is_stored)
        {
          fs_write_vint(fw->fieldsstream, field_infos_get_number_by_name(
              fw->fieldinfos, field->name));
          printf("fields_writer_add_document:field_number: %i\n", field_infos_get_number_by_name(
              fw->fieldinfos, field->name));
          int bits = 0;
          if (field->is_tokenized)
            bits |= 1;
          fs_write_byte(fw->fieldsstream, bits);
          printf("fields_writer_add_document:bits: %i\n", bits);
          fs_write_string(fw->fieldsstream, field->string_value);
          printf("fields_writer_add_document:field->string_value: %s\n", field->string_value);
        }
    }
}

void
fields_writer_close(struct _fields_writer* fw)
{
  fs_close(fw->fieldsstream);
  fs_close(fw->indexstream);
}

