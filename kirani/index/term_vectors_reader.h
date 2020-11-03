/*
* author: Thomas Yao
*/

#ifndef _TERM_VECTORS_READER_H_
#define _TERM_VECTORS_READER_H_

#include "field_info.h"

#include "../store/file_stream.h"

struct _term_vectors_reader
{
  struct _field_infos* fis;
  struct _file_stream* tvx;
  struct _file_stream* tvd;
  struct _file_stream* tvf;
  int size;
};

#endif



