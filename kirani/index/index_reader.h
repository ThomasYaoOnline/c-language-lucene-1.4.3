/*
 * author: Thomas Yao
 */

#ifndef _INDEX_READER_H_
#define _INDEX_READER_H_

#include "field_info.h"
#include "fields_reader.h"
#include "segment_info.h"
#include "term_infos_reader.h"
#include "term_vectors_reader.h"
#include "index_writer.h"
#include "norm.h"

#include "../document/document.h"
#include "../store/directory.h"
#include "../store/lock.h"
#include "../utils/char_list.h"

struct _index_reader
{
  struct _directory* dir;
  int directory_owner;
  struct _segment_infos* sis;
  struct _lock* write_lock;
  int stale;
  int has_change;

  int close_directory;

  char* segment;
  struct _field_infos* field_infos;
  struct _fields_reader* fields_reader;
  struct _term_infos_reader* term_infos_reader;
  struct _term_vectors_reader* term_vectors_reader;
  struct _int_list* deleted_docs;
  int deleted_docs_dirty;
  int norms_dirty;
  int un_delete_all;
  struct _file_stream* freq_stream;
  struct _file_stream* prox_stream;
  struct _norms* nms;
};

struct _index_reader*
index_reader_initial2(struct _segment_infos* sis, struct _segment_info* si,
    int close_dir);

struct _index_reader*
index_reader_initial(struct _directory* dir);

void
initialize(struct _index_reader* ir, struct _segment_info* si);

struct _index_reader*
index_reader_open(char* _path, int _close_directory);

void
index_reader_rewrite(struct _index_reader* _ir);

void
index_reader_close(struct _index_reader* _ir);

void
index_reader_commit(struct _index_reader* _ir);

void
index_reader_doclose(struct _index_reader* _ir);

void
index_reader_docommit(struct _index_reader* _ir);

struct _char_list*
index_reader_get_indexed_fieldnames(struct _index_reader* ir,
    int _store_term_vector);

struct _char_list*
inde_reader_get_fieldnames(struct _index_reader* ir, int _is_indexed);

int
index_reader_is_deleted(struct _index_reader* ir, int n);

struct _document*
index_reader_document(struct _index_reader* ir, int n);

int
index_reader_doc_freq(struct _index_reader* ir, struct _term* _tm);

int
index_reader_max_doc(struct _index_reader* ir);

int
index_reader_num_docs(struct _index_reader* ir);

struct _int_list*
index_reader_norms(struct _index_reader* ir, char* field);

int
index_reader_is_delete(struct _index_reader* ir, int k);

struct _term_docs*
index_reader_term_docs(struct _index_reader* ir, struct _term* term);

struct _term_enum*
index_reader_terms(struct _index_reader* ir);

struct _index_readers_node
{
  struct _index_readers_node* prev;
  struct _index_readers_node* next;
  struct _index_reader* data;
  int index;
};

struct _index_readers
{
  struct _index_readers_node* curr;
  int size;
};

struct _index_readers*
index_readers_initial();

void
index_readers_add(struct _index_readers* s, struct _index_reader* ir);

struct _index_reader*
index_readers_get_index(struct _index_readers* s, int index);

void
index_readers_rm_curr(struct _index_readers* _s);

void
index_readers_destory(struct _index_readers* _s);

void
index_readers_clear(struct _index_readers* _s);

void
index_readers_print(struct _index_readers* _s);

#endif

