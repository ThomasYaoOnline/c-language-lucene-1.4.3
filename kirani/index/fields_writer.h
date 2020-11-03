/*
 * author: Thomas Yao
 */

#ifndef _FIELDS_WRITER_H_
#define _FIELDS_WRITER_H_

#include "field_info.h"

#include "../store/file_stream.h"

struct _fields_writer
{
  struct _field_infos* fieldinfos;
  struct _file_stream* fieldsstream;
  struct _file_stream* indexstream;
};

struct _fields_writer*
fields_writer_initial(struct _directory* dir, char* segment,
    struct _field_infos* fis);

void
fields_writer_add_document(struct _fields_writer* _fw, struct _document* _doc);

void
fields_writer_close();

#endif

