/*
 * author: Thomas Yao
 */

#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "term_infos_writer.h"

#include "../store/directory.h"
#include "../utils/utils.h"

struct _term_infos_writer*
term_infos_writer_initial(struct _directory* _dir, char* _segment,
    struct _field_infos* _fis, int _is_index)
{
  struct _term_infos_writer* tisw = (struct _term_infos_writer*) malloc(
      sizeof(struct _term_infos_writer));
  if (tisw == NULL)
    err(1, "tisw is null");

  /* default */
  tisw->last_term = term_initial("", "");
  tisw->last_ti = term_info_initial();
  tisw->size = 0;
  tisw->index_interval = 128;
  tisw->skip_interval = 16;
  tisw->last_index_pointer = 0;
  tisw->is_index = 0;
  /* default */

  tisw->field_infos = _fis;
  tisw->is_index = _is_index;

  if (tisw->is_index)
    tisw->output = directory_open_file(_dir, _segment, ".tii", "w");
  else
    tisw->output = directory_open_file(_dir, _segment, ".tis", "w");

  fs_write_int(tisw->output, TERM_INFOS_WRITER_FORMAT); // -2
  fs_write_long(tisw->output, 0); // leave space for size
  fs_write_int(tisw->output, tisw->index_interval); // 128
  fs_write_int(tisw->output, tisw->skip_interval); // 16
  return tisw;
}

struct _term_infos_writer*
term_infos_writer_initial1(struct _directory* _dir, char* _segment,
    struct _field_infos* _fis)
{
  struct _term_infos_writer* tisw = term_infos_writer_initial(_dir, _segment,
      _fis, 0);
  tisw->other = term_infos_writer_initial(_dir, _segment, _fis, 1);
  tisw->other->other = tisw;
  return tisw;
}

void
write_term(struct _term_infos_writer* tisw, struct _term* term)
{
  int start = utils_string_difference(tisw->last_term->text, term->text);
  printf("term_infos_writer:write_term:start: %d\n", start);

  int length = strlen(term->text) - start;
  printf("term_infos_writer:write_term:length: %d\n", length);

  fs_write_vint(tisw->output, start);
  fs_write_vint(tisw->output, length);
  fs_write_chars(tisw->output, term->text, start, length);
  printf("term_infos_writer:write_term:term->text: %s\n", term->text);
  fs_write_vint(tisw->output, field_infos_get_number_by_name(tisw->field_infos,
      term->field));
  printf("term_infos_writer:write_term:field_number: %d\n",
      field_infos_get_number_by_name(tisw->field_infos, term->field));

  tisw->last_term = term;
}

void
term_infos_writer_add(struct _term_infos_writer* tisw, struct _term* term,
    struct _term_info* ti)
{
  if (!tisw->is_index && term_compare_to(term, tisw->last_term) <= 0)
    err(1, "term out of order");
  if (ti->freq_pointer < tisw->last_ti->freq_pointer)
    err(1, "freq_pointer out of order");
  if (ti->prox_pointer < tisw->last_ti->prox_pointer)
    err(1, "prox_pointer out of order");

  // add a index term
  if (!tisw->is_index && tisw->size % tisw->index_interval == 0)
    {
      printf("%s\n", "term_infos_writer_other_run");
      term_infos_writer_add(tisw->other, tisw->last_term, tisw->last_ti);
    }

  // write_term
  write_term(tisw, term);

  // write doc freq
  fs_write_vint(tisw->output, ti->doc_freq);
  printf("term_infos_writer_add:ti->doc_freq: %d\n", ti->doc_freq);

  // write pointers
  printf("term_infos_writer_add:ti->freq_pointer: %lli", ti->freq_pointer);
  printf(" term_infos_writer_add:tisw->last_ti->freq_pointer: %lli\n",
      tisw->last_ti->freq_pointer);
  printf("term_infos_writer_add:ti->prox_pointer: %lli", ti->prox_pointer);
  printf(" term_infos_writer_add:tisw->last_ti->prox_pointer: %lli\n",
      tisw->last_ti->prox_pointer);

  fs_write_vlong(tisw->output, ti->freq_pointer - tisw->last_ti->freq_pointer);
  fs_write_vlong(tisw->output, ti->prox_pointer - tisw->last_ti->prox_pointer);

  printf("term_infos_writer_add:tisw->skip_interval: %d\n", tisw->skip_interval);
  if (ti->doc_freq > tisw->skip_interval)
    fs_write_vint(tisw->output, ti->skip_offset);

  if (tisw->is_index)
    {
      fs_write_vlong(tisw->output, ftell(tisw->other->output->stream)
          - tisw->last_index_pointer);
      printf(
          "term_infos_writer_add:ftell(tisw->other->output->stream) - tisw->last_index_pointer %lli\n",
          (ftell(tisw->other->output->stream) - tisw->last_index_pointer));
      tisw->last_index_pointer = ftell(tisw->other->output->stream);
    }

  term_info_set_copy(tisw->last_ti, ti);
  tisw->size++;
}

void
term_infos_writer_close(struct _term_infos_writer* tisw)
{
  fs_seek(tisw->output, 4);
  fs_write_long(tisw->output, tisw->size);
  fs_close(tisw->output);

  if (!tisw->is_index)
    term_infos_writer_close(tisw->other);
}

