/*
 * author: Thomas Yao
 */

#ifndef _TERM_INFOS_READER_H_
#define _TERM_INFOS_READER_H_

#include "field_info.h"
#include "term_enum.h"
#include "term_posting.h"
#include "term_info.h"

#include "../store/directory.h"

struct _term_infos_reader
{
  struct _directory* dir;
  char* segment;
  struct _field_infos* fis;
  struct _term_enums* enumerators;
  struct _term_enum* orig_enum;
  long long size;

  struct _term_postings* index_terms;
  struct _term_infos* index_infos;
  struct _long_long_list* index_pointers;
};

struct _term_infos_reader*
term_infos_reader_initial(struct _directory* dir, char* segment,
    struct _field_infos* fieldinfos);

struct _term_info*
term_infos_reader_get(struct _term_infos_reader* tisr, struct _term* term);

struct _term_enum*
term_infos_reader_terms(struct _term_infos_reader* tisr);

#endif

