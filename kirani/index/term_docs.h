/*
 * author: Thomas Yao
 */

#ifndef TERM_DOCS_H_
#define TERM_DOCS_H_

#include "index_reader.h"

#include "../store/file_stream.h"
#include "../utils/int_list.h"

struct _term_docs
{
  struct _index_reader* parent;
  struct _file_stream* freq_stream;
  int count;
  int df;
  struct _int_list *bit_vector, *deleted_docs;
  int doc;
  int freq;

  int skip_interval;
  int num_skips;
  int skip_count;
  struct _file_stream* skip_stream;
  int skip_doc;
  long freq_pointer;
  long prox_pointer;
  long skip_pointer;
  int have_skipped;

  struct _file_stream* prox_stream;
  int prox_count;
  int position;
};

struct _term_docs*
term_docs_initial(struct _index_reader* _ir);

void
term_docs_seek_term(struct _term_docs* term_docs, struct _term* _term);

void
term_docs_seek_term_info(struct _term_docs* term_docs, struct _term_info* ti);

void
term_docs_seek_term_enum(struct _term_docs* term_docs,
    struct _term_enum* term_enum);
void
term_docs_close(struct _term_docs* term_docs);

int
term_docs_next(struct _term_docs* term_docs);

int
term_docs_next_position(struct _term_docs* postings);

int
term_docs_read(struct _term_docs* term_docs, struct _int_list* docs,
    struct _int_list* freqs);

void
term_docs_close(struct _term_docs* term_docs);

#endif /* TERM_DOCS_H_ */
