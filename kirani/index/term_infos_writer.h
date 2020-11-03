/*
 * author: Thomas Yao
 */

#ifndef _TERM_INFOS_WRITER_H_
#define _TERM_INFOS_WRITER_H_

#include "field_info.h"
#include "term_posting.h"
#include "term_info.h"

#include "../store/directory.h"
#include "../store/file_stream.h"

#define TERM_INFOS_WRITER_FORMAT -2

struct _term_infos_writer
{
  struct _field_infos* field_infos;
  struct _file_stream* output;
  struct _term* last_term;
  struct _term_info* last_ti;
  long long size;
  int index_interval;
  int skip_interval;
  long long last_index_pointer;
  int is_index;
  struct _term_infos_writer* other;
};

struct _term_infos_writer*
term_infos_writer_initial(struct _directory* _dir, char* _segment,
    struct _field_infos* _fis, int _is_index);

struct _term_infos_writer*
term_infos_writer_initial1(struct _directory* _dir, char* _segment,
    struct _field_infos* _fis);

void
term_infos_writer_add(struct _term_infos_writer* _tisw, struct _term* _tm,
    struct _term_info* _ti);

void
term_infos_writer_close(struct _term_infos_writer* _tisw);

#endif

