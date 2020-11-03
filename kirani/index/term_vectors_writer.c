/*
 * author: Thomas Yao
 */

#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <string.h>
#include <wchar.h>

#include "term_vectors_writer.h"

#include "../utils/utils.h"

struct _term_vectors_writer*
term_vectors_writer_initial(struct _directory* _dir, char* _segment,
    struct _field_infos* _fis)
{
  struct _term_vectors_writer* tvw = (struct _term_vectors_writer*) malloc(
      sizeof(struct _term_vectors_writer));
  if (tvw == NULL)
    err(1, "tvw is null");
  tvw->tvx = directory_open_file(_dir, _segment, ".tvx", "w");
  fs_write_int(tvw->tvx, TERM_VECTORS_WRITER_FORMAT_VERSION);
  tvw->tvd = directory_open_file(_dir, _segment, ".tvd", "w");
  fs_write_int(tvw->tvx, TERM_VECTORS_WRITER_FORMAT_VERSION);
  tvw->tvf = directory_open_file(_dir, _segment, ".tvf", "w");
  fs_write_int(tvw->tvx, TERM_VECTORS_WRITER_FORMAT_VERSION);
  tvw->fis = _fis;
  tvw->tvfs = tv_fields_initial();
  tvw->tvts = tv_terms_initial();
  tvw->current_doc_pointer = -1;
  return tvw;
}

int
term_vectors_writer_is_field_open(struct _term_vectors_writer* _tvw)
{
  if (_tvw == NULL)
    err(1, "_tvw is null");
  return _tvw->current_field != NULL;
}

void
write_field(struct _term_vectors_writer* _tvw)
{
  // remember where this field is written
  _tvw->current_field->tvf_pointer = fs_tell(_tvw->tvf);
  int size = _tvw->tvts->size;

  fs_write_int(_tvw->tvf, size);
  fs_write_int(_tvw->tvf, _tvw->current_field->length - size);

  char* last_term_text;
  // write term ids and positions
  struct _tv_term* tvterm;
  int start;
  int length;
  int i;
  for (i = 0; i < size; i++)
    {
      tvterm = tv_terms_get_index(_tvw->tvts, i);
      start = utils_string_difference(last_term_text, tvterm->term_text);
      length = strlen(tvterm->term_text) - start;
      // write shared prefix length
      fs_write_int(_tvw->tvf, start);
      // write delta length
      fs_write_int(_tvw->tvf, length);
      // write delta chars
      fs_write_string_sl(_tvw->tvf, tvterm->term_text, start, length);
      fs_write_int(_tvw->tvf, tvterm->freq);
      last_term_text = tvterm->term_text;
    }
}

void
term_vectors_writer_close_field(struct _term_vectors_writer* _tvw)
{
  if (term_vectors_writer_is_field_open(_tvw))
    {
      write_field(_tvw);
      tv_fields_add(_tvw->tvfs, _tvw->current_field);
      tv_terms_clear(_tvw->tvts);
      _tvw->current_field = NULL;
    }
}

int
is_document_open(struct _term_vectors_writer* _tvw)
{
  return _tvw->current_doc_pointer != -1;
}

void
write_doc(struct _term_vectors_writer* _tvw)
{

  if (term_vectors_writer_is_field_open(_tvw))
    err(1, "Field is still open while writing document");
  /* write document index record */

  fs_write_long(_tvw->tvx, _tvw->current_doc_pointer);

  // write document data record
  int size = _tvw->tvfs->size;

  // write the number of fields
  fs_write_int(_tvw->tvd, size);

  // write field numbers
  int last_field_number = 0;
  int i;
  struct _tv_field* tvf;
  for (i = 0; i < size; i++)
    {
      tvf = tv_fields_get_index(_tvw->tvfs, i);
      fs_write_int(_tvw->tvd, tvf->number - last_field_number);
      last_field_number = tvf->number;
    }

  // write field pointers
  long long last_field_pointer = 0;
  for (i = 0; i < size; i++)
    {
      tvf = tv_fields_get_index(_tvw->tvfs, i);
      fs_write_int(_tvw->tvd, tvf->tvf_pointer - last_field_pointer);
      last_field_pointer = tvf->tvf_pointer;
    }
}

void
term_vectors_writer_close_document(struct _term_vectors_writer* _tvw)
{
  if (is_document_open(_tvw))
    {
      term_vectors_writer_close_field(_tvw);
      write_doc(_tvw);
      tv_fields_clear(_tvw->tvfs);
      _tvw->current_doc_pointer = -1;
    }
}
void
term_vectors_writer_open_document(struct _term_vectors_writer* _tvw)
{
  term_vectors_writer_close_document(_tvw);
  _tvw->current_doc_pointer = fs_tell(_tvw->tvd);
}

void
term_vectors_writer_open_field(struct _term_vectors_writer* _tvw,
    char* tmfield)
{
  if (!is_document_open(_tvw))
    err(1, "cannot open field when no document is open");
  term_vectors_writer_close_field(_tvw);
  _tvw->current_field = tv_field_initial(field_infos_get_number_by_name(
      _tvw->fis, tmfield));
}

void
add_term_internal(struct _term_vectors_writer* _tvw, char* _tm_text,
    int _posting_freq)
{
  _tvw->current_field->length += _posting_freq;
  struct _tv_term* tvt = tv_term_initial();
  tvt->term_text = _tm_text;
  tvt->freq = _posting_freq;
  tv_terms_add(_tvw->tvts, tvt);
}

void
term_vectors_writer_add_term(struct _term_vectors_writer* _tvw, char* _tm_text,
    int _posting_freq)
{
  if (!is_document_open(_tvw))
    err(1, "cannot add terms when document is not open");
  if (!term_vectors_writer_is_field_open(_tvw))
    err(1, "cannot add terms when field is not open");
  add_term_internal(_tvw, _tm_text, _posting_freq);
}

void
term_vectors_writer_close(struct _term_vectors_writer* _tvw)
{
  fs_close(_tvw->tvd);
  fs_close(_tvw->tvf);
  fs_close(_tvw->tvx);
}

