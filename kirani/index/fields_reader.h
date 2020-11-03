/*
 * author: Thomas Yao
 */

#ifndef _FIELDS_READER_H_
#define _FIELDS_READER_H_

#include "field_info.h"

#include "../store/file_stream.h"

struct _fields_reader
{
  struct _field_infos* fieldinfos;
  struct _file_stream* fields_stream;
  struct _file_stream* index_stream;
  int size;
};

struct _fields_reader*
fields_reader_initial(struct _directory* _dir, char* _segment,
    struct _field_infos* _fis);

struct _document*
fields_reader_doc(struct _fields_reader* fieldsreader, int _n);

void
fields_reader_close(struct _fields_reader* fieldsreader);

#endif

