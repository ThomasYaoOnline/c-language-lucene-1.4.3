/*
 * author: Thomas Yao
 */

#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "index_reader.h"
#include "term_docs.h"

#include "../utils/utils.h"

struct _index_reader*
index_reader_initial(struct _directory* dir)
{
  struct _index_reader* ir = (struct _index_reader*) malloc(
      sizeof(struct _index_reader));
  ir->dir = dir;
  ir->sis = NULL;
  ir->directory_owner = 0;
  ir->close_directory = 0;
  ir->stale = 0;
  ir->has_change = 0;
  ir->write_lock = NULL;
  ir->nms = norms_initial();
  return ir;
}

struct _index_reader*
index_reader_initial2(struct _segment_infos* sis, struct _segment_info* si,
    int close_dir)
{
  struct _index_reader* ir = index_reader_initial(si->dir);
  if (ir == NULL)
    err(1, "ir is null");

  ir->sis = sis;
  ir->close_directory = close_dir;

  initialize(ir, si);
  return ir;
}

void
open_norms(struct _index_reader* ir)
{
  int i;
  for (i = 0; i < ir->field_infos->size; i++)
    {
      struct _field_info* fi = field_infos_get_by_number(ir->field_infos, i);

      if (fi->is_indexed)
        {
          char* filesuffix = utils_strcat(".f", utils_itoa(fi->number, 16));
          // look first for re-written file, then in compound format
          struct _file_stream* in = directory_open_file(ir->dir, ir->segment,
              filesuffix, "r");
          norms_add(ir->nms, fi->name, norm_initial(in, fi->number));
        }
    }
}

void
initialize(struct _index_reader* ir, struct _segment_info* si)
{
  ir->segment = si->name;
  ir->field_infos = field_infos_initial_read(ir->dir, ir->segment);

  ir->fields_reader = fields_reader_initial(ir->dir, ir->segment,
      ir->field_infos);
  ir->term_infos_reader = term_infos_reader_initial(ir->dir, ir->segment,
      ir->field_infos);

  // make sure that all index files have been read or are kept open
  // so that if an index update removes them we'll still have them
  ir->freq_stream = directory_open_file(ir->dir, ir->segment, ".frq", "r");
  ir->prox_stream = directory_open_file(ir->dir, ir->segment, ".prx", "r");

  open_norms(ir);
}

struct _index_reader*
index_reader_open(char* _path, int _close_directory)
{
  struct _directory* dir = directory_get_directory(NULL, _path, 0);

  struct _with* wth = with_initial(directory_make_lock(dir,
      INDEX_WRITER_COMMIT_LOCK_NAME), INDEX_WRITER_COMMIT_LOCK_TIMEOUT);

  int locked = lock_obtain(wth->lock_wait_timeout);

  struct _segment_infos* sis = segment_infos_initial();
  segment_infos_read(sis, dir);

  struct _index_reader* ir = index_reader_initial2(sis,
      segment_infos_get_index(sis, 0), _close_directory);
  if (locked)
    lock_release();
  return ir;
}

void
index_reader_rewrite(struct _index_reader* _ir)
{
  struct _file_stream* fs = directory_open_file(_ir->dir, _ir->segment, ".tmp",
      "w");
  //  fwrite(fs->stream, _ir->nm->bytes);
  fclose(fs->stream);
  char* file_name = utils_strcat3(_ir->segment, ".f",
      (char*) _ir->nms->curr->data->number);
  //directory_rename_file(utils_strcat(_ir->segment, ".tmp"), file_name);
  _ir->nms->curr->data->dirty = 0;
}

void
index_reader_close(struct _index_reader* _ir)
{
  index_reader_commit(_ir);
  index_reader_doclose(_ir);
  if (_ir->close_directory)
    directory_close(_ir->dir);
}

void
index_reader_commit(struct _index_reader* _ir)
{
  struct _with* wth;
  if (_ir->has_change)
    {
      if (_ir->directory_owner)
        {
          wth = with_initial(directory_make_lock(_ir->dir,
              INDEX_WRITER_COMMIT_LOCK_NAME), INDEX_WRITER_COMMIT_LOCK_TIMEOUT);
          index_reader_docommit(_ir);
          segment_infos_write(_ir->sis, _ir->dir);

          if (_ir->write_lock != NULL)
            {
              lock_release();
              _ir->write_lock = NULL;
            }
        }
      else
        {
          index_reader_docommit(_ir);
        }
    }
  _ir->has_change = 0;
}

void
index_reader_doclose(struct _index_reader* _ir)
{
  return;
}

void
index_reader_docommit(struct _index_reader* _ir)
{
  return;
}

struct _char_list*
index_reader_get_indexed_fieldnames(struct _index_reader* ir,
    int _store_term_vector)
{
  // maintain a unique set of field names
  struct _char_list* fieldset = char_list_initial();
  int i;
  for (i = 0; i < ir->field_infos->size; i++)
    {
      struct _field_info* fi = field_infos_get_index(ir->field_infos, i);
      if (fi->is_indexed == 1 && fi->store_term_vector == _store_term_vector)
        char_list_add(fieldset, fi->name);
    }
  return fieldset;
}

struct _char_list*
inde_reader_get_fieldnames(struct _index_reader* ir, int _is_indexed)
{
  struct _char_list* fieldset = char_list_initial();
  int i;
  struct _field_info* fi;
  for (i = 0; i < ir->field_infos->size; i++)
    {
      fi = field_infos_get_index(ir->field_infos, i);
      if (fi->is_indexed == _is_indexed)
        char_list_add(fieldset, fi->name);
    }
  return fieldset;
}

int
index_reader_is_deleted(struct _index_reader* ir, int n)
{
  return (ir->deleted_docs != NULL && int_list_get_index(ir->deleted_docs, n));
}

struct _document*
index_reader_document(struct _index_reader* ir, int n)
{
  if (index_reader_is_deleted(ir, n))
    err(1, "attempt to access a deleted document");
  return fields_reader_doc(ir->fields_reader, n);
}

int
index_reader_doc_freq(struct _index_reader* ir, struct _term* _tm)
{
  struct _term_info* ti = term_infos_reader_get(ir->term_infos_reader, _tm);
  if (ti != NULL)
    return ti->doc_freq;
  else
    return 0;
}

int
index_reader_max_doc(struct _index_reader* ir)
{
  return ir->fields_reader->size;
}

int
index_reader_num_docs(struct _index_reader* ir)
{
  int n = index_reader_max_doc(ir);
  if (ir->deleted_docs != NULL)
    n -= ir->deleted_docs->size;
  return n;
}

struct _int_list*
index_reader_norms(struct _index_reader* ir, char* field)
{
  struct _norm* norm = norms_get_by_key(ir->nms, field);

  if (norm == NULL) // not an indexed field
    return NULL;

  struct _file_stream* norm_stream = fs_initial_clone(norm->in);

  int _bytes[100];

  // read from disk
  fs_seek(norm_stream, 0);
  fs_read_bytes(norm_stream, _bytes, 0, ir->fields_reader->size);
  //fs_close(norm_stream);

  int i;
  for (i = 0; i < ir->fields_reader->size; i++)
    {
      int_list_add(norm->bytes, _bytes[i]);
    }
  return norm->bytes;
}

int
index_reader_is_delete(struct _index_reader* ir, int k)
{
  return (ir->deleted_docs != NULL && int_list_get_index(ir->deleted_docs, k));
}

struct _term_docs*
index_reader_term_docs(struct _index_reader* ir, struct _term* term)
{
  struct _term_docs* term_docs = term_docs_initial(ir);
  term_docs_seek_term(term_docs, term);
  return term_docs;
}

struct _term_enum*
index_reader_terms(struct _index_reader* ir)
{
  return term_infos_reader_terms(ir->term_infos_reader);
}

struct _index_readers*
index_readers_initial()
{
  struct _index_readers* irs = (struct _index_readers*) malloc(
      sizeof(struct _index_readers));
  if (irs == NULL)
    err(1, "irs is null");
  irs->curr = NULL;
  irs->size = 0;
  return irs;
}

void
index_readers_add(struct _index_readers* _s, struct _index_reader* _data)
{
  struct _index_readers_node* node = (struct _index_readers_node*) malloc(
      sizeof(struct _index_readers_node));
  if (node == NULL)
    err(1, "node is null");

  if (_s->size == 0)
    {
      node->prev = node;
      node->next = node;
      _s->curr = node;
    }

  struct _index_readers_node* curr_node = _s->curr;
  struct _index_readers_node* prev_node = _s->curr->prev;

  curr_node->prev = node;
  prev_node->next = node;

  node->prev = prev_node;
  node->next = curr_node;
  node->data = _data;

  _s->curr = node;
  _s->curr->index = _s->size++;
}

struct _index_reader*
index_readers_get_index(struct _index_readers* s, int _index)
{
  int i;
  for (i = 0; i < s->size; i++)
    {
      if (s->curr->index == _index)
        return s->curr->data;
      s->curr = s->curr->next;
    }
  return NULL;
}

void
index_readers_rm_curr(struct _index_readers* _s)
{
  if (_s == NULL)
    err(1, "_s is null");

  struct _index_readers_node* cn = _s->curr;

  cn->next->prev = cn->prev;
  cn->prev->next = cn->next;

  _s->curr = cn->prev;
  _s->size--;

  free(cn);
}

void
index_readers_destory(struct _index_readers* _s)
{
  if (_s == NULL)
    err(1, "_s is null");

  while (_s->size != 0)
    {
      index_readers_rm_curr(_s);
    }

  free(_s);
}

void
index_readers_clear(struct _index_readers* _s)
{
  if (_s == NULL)
    err(1, "_s is null");
  while (_s->size != 0)
    {
      index_readers_rm_curr(_s);
      _s->curr = _s->curr->next;
    }
}

void
index_readers_print(struct _index_readers* _s)
{
  struct _index_readers_node* curr_node = _s->curr;
  int i;

  for (i = 0; i < _s->size; i++)
    {
      curr_node = curr_node->next;
    }
}
