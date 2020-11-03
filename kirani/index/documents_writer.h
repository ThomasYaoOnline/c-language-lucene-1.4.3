/*
 * author: Thomas Yao
 */

#ifndef _DOCUMENTS_WRITER_H_
#define _DOCUMENTS_WRITER_H_

#include <wchar.h>

#include "field_info.h"
#include "term_posting.h"

#include "../analysis/analyzer.h"
#include "../document/document.h"
#include "../search/similarity.h"
#include "../store/file_stream.h"
#include "../store/directory.h"
#include "../utils/int_list.h"
#include "../utils/float_list.h"

struct _documents_writer
{
  struct _analyzer* analyzer;
  struct _directory* dir;
  struct _similarity* similarity;
  struct _field_infos* fieldinfos;
  int max_field_length;

  struct _term_postings* postings;
  struct _int_list* field_lengths;
  struct _int_list* field_positions;
  struct _float_list* field_boosts;

  struct _term* term_buffer;
};

struct _documents_writer*
documents_writer_initial(struct _directory* _dir, struct _analyzer* _alz,
    struct _similarity* _sml, int _max_field_length);

void
documents_writer_add_document(struct _documents_writer* _dw, char* _segment,
    struct _document* _doc);

#endif



