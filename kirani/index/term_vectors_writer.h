/*
 * author: Thomas Yao
 */

#ifndef _TERM_VECTORS_WRITER_H_
#define _TERM_VECTORS_WRITER_H_

#include "field_info.h"
#include "tv_field.h"
#include "tv_term.h"

#include "../store/file_stream.h"

#define TERM_VECTORS_WRITER_FORMAT_VERSION 1
#define TERM_VECTORS_WRITER_FORMAT_SIZE 4

struct _term_vectors_writer
{
  struct _file_stream* tvx;
  struct _file_stream* tvd;
  struct _file_stream* tvf;
  struct _tv_fields* tvfs;
  struct _tv_terms* tvts;
  struct _field_infos* fis;
  struct _tv_field* current_field;
  long long current_doc_pointer;
};

struct _term_vectors_writer*
term_vectors_writer_initial(struct _directory* _dir, char* _segment,
    struct _field_infos* _fis);

int
term_vectors_writer_is_field_open(struct _term_vectors_writer* _tvw);

void
term_vectors_writer_open_field(struct _term_vectors_writer* _tvw,
    char* _tm_field);

void
term_vectors_writer_close_field(struct _term_vectors_writer* _tvw);

void
term_vectors_writer_close_document(struct _term_vectors_writer* _tvw);

void
term_vectors_writer_open_document(struct _term_vectors_writer* _tvw);

void
term_vectors_writer_add_term(struct _term_vectors_writer* _tvw,
    char* _tm_text, int _posting_freq);

void
term_vectors_writer_close(struct _term_vectors_writer* _tvw);

#endif

