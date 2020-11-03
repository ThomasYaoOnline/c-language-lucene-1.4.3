/*
 * author: Thomas Yao
 */

#include <err.h>
#include <stdlib.h>

#include "fields_reader.h"

#include "../utils/utils.h"
#include "../store/directory.h"
#include "../document/field.h"
#include "../document/document.h"

struct _fields_reader*
fields_reader_initial(struct _directory* dir, char* segment,
    struct _field_infos* fieldinfos)
{
  struct _fields_reader* fieldsreader = (struct _fields_reader*) malloc(
      sizeof(struct _fields_reader));
  fieldsreader->fieldinfos = fieldinfos;
  fieldsreader->fields_stream = directory_open_file(dir, segment, ".fdt", "r");
  fieldsreader->index_stream = directory_open_file(dir, segment, ".fdx", "r");
  int length = 0;
  while (fs_read_byte(fieldsreader->index_stream) != EOF)
    length++;
  fieldsreader->size = (int) (length / 8);
  fs_seek(fieldsreader->index_stream, 0);
  return fieldsreader;
}

struct _document*
fields_reader_doc(struct _fields_reader* fieldsreader, int _n)
{
  fs_seek(fieldsreader->index_stream, _n * 8L);
  long long position = fs_read_long(fieldsreader->index_stream);
  printf("fields_reader_doc:position %lli\n", position);
  fs_seek(fieldsreader->fields_stream, position);

  struct _document* doc = document_initial();

  int num_fields = fs_read_vint(fieldsreader->fields_stream);
  printf("fields_reader_doc:num_fields: %d\n", num_fields);
  int i;
  for (i = 0; i < num_fields; i++)
    {
      int fieldnumber = fs_read_vint(fieldsreader->fields_stream);
      printf("fields_reader_doc:fieldnumber: %d\n", fieldnumber);
      struct _field_info* fi = field_infos_get_by_number(fieldsreader->fieldinfos, fieldnumber);
      int bits = fs_read_byte(fieldsreader->fields_stream);
      printf("fields_reader_doc:bits: %d\n", bits);

      struct _field* fd = field_initial(fi->name, //name
          (char*) fs_read_string(fieldsreader->fields_stream), //stringvalue
          NULL, //readervalue
          1, //stored
          fi->is_indexed, //indexed
          (bits & 1) != 0, fi->store_term_vector); // vector
      printf("fields_reader_doc:fd->name: %s\n", fd->name);
      printf("fields_reader_doc:fd->string_value: %s\n", fd->string_value);
      document_add(doc, fd);
    }
  return doc;
}

void
fields_reader_close(struct _fields_reader* fieldsreader)
{
  fclose(fieldsreader->fields_stream->stream);
  fclose(fieldsreader->index_stream->stream);
}
