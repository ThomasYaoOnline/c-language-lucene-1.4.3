/*
 * author: Thomas Yao
 */

#include <err.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "documents_writer.h"
#include "fields_writer.h"
#include "term_infos_writer.h"
#include "term_vectors_writer.h"

#include "../analysis/token.h"
#include "../analysis/token_stream.h"
#include "../utils/utils.h"

struct _documents_writer*
documents_writer_initial(struct _directory* _dir, struct _analyzer* _alz,
    struct _similarity* _sml, int _max_field_length)
{
  struct _documents_writer* dw = (struct _documents_writer*) malloc(
      sizeof(struct _documents_writer));

  if (dw == NULL)
    err(1, "documents_writer is null");

  dw->dir = _dir;
  dw->analyzer = _alz;
  dw->similarity = _sml;
  dw->max_field_length = _max_field_length;

  dw->postings = term_postings_initial();

  dw->term_buffer = term_initial("", "");
  return dw;
}

void
add_position(struct _documents_writer* dw, char* field, char* text,
    int position)
{
  term_set(dw->term_buffer, field, text);
  struct _posting* posting = term_postings_get_postings_by_term(dw->postings,
      dw->term_buffer);

  if (posting != NULL)
    {
      int freq = posting->freq;
      if (posting->positions->size == freq)
        {
          struct _int_list* new_positions = int_list_initial();
          int i;
          for (i = 0; i < freq * 2; i++)
            {
              int_list_add(new_positions, 0);
            }
          for (i = 0; i < freq; i++)
            {
              int_list_set_index(new_positions, i, int_list_get_index(
                  posting->positions, i));
            }
          posting->positions = new_positions;
        }

      int_list_set_index(posting->positions, freq, position);
      posting->freq = freq + 1;
    }
  else
    {
      struct _term* term = term_initial(field, text);
      term_postings_add(dw->postings, term, posting_initial(term, position));
    }
}

void
invert_document(struct _documents_writer* dw, struct _document* doc)
{
  struct _field* field;
  struct _token_stream* ts;
  struct _token* tk;
  int fieldnumber;

  int length;
  int position;
  float boost;

  int i;
  for (i = 0; i < doc->fields->size; i++)
    {
      field = fields_get_index(doc->fields, i);
      fieldnumber = field_infos_get_number_by_name(dw->fieldinfos, field->name);

      length = int_list_get_index(dw->field_lengths, fieldnumber);
      position = int_list_get_index(dw->field_positions, fieldnumber);

      if (field->is_indexed)
        {
          if (!field->is_tokenized)
            {
              add_position(dw, field->name, field->string_value, position++);
              length++;
            }
          else
            {
              if (field->reader_value == NULL)
                field->reader_value = fs_initial_mem(field->string_value, "r");

              ts = ts_initial(field->reader_value);
              for (tk = ts_next(); tk != NULL; tk = ts_next())
                {
                  position += (tk->position_increment - 1);
                  add_position(dw, field->name, tk->term_text, position++);
                  if (++length > dw->max_field_length)
                    break;
                }
            }

          int_list_set_index(dw->field_lengths, fieldnumber, length);
          int_list_set_index(dw->field_positions, fieldnumber, position);
          boost = float_list_get_index(dw->field_boosts, fieldnumber);
          float_list_set_index(dw->field_boosts, fieldnumber, boost);
        }
    }
}

void
write_postings(struct _documents_writer* dw, char* segment)
{
  struct _file_stream *freq, *prox;
  struct _term_infos_writer* tisw = NULL;
  struct _term_vectors_writer* tvw = NULL;

  /* open files for inverse index storage */
  freq = directory_open_file(dw->dir, segment, ".frq", "w");
  prox = directory_open_file(dw->dir, segment, ".prx", "w");
  tisw = term_infos_writer_initial1(dw->dir, segment, dw->fieldinfos);

  struct _term_info* ti_buffer = term_info_initial();
  char* current_field;

  int i;
  int j;
  for (i = 0; i < dw->postings->size; i++)
    {
      struct _posting* posting = term_postings_get_posting_index(dw->postings,
          i);

      /* add an entry to the dictionary with pointers to prox and freq files */
      term_info_set(ti_buffer, 1, fs_tell(freq), fs_tell(prox), -1);
      term_infos_writer_add(tisw, posting->term, ti_buffer);

      /* add an entry to the freq file */
      int posting_freq = posting->freq;
      if (posting_freq == 1) // optimize freq=1
        fs_write_vint(freq, 1); // set low bit of doc num
      else
        {
          fs_write_vint(freq, 0); //  the document number
          fs_write_vint(freq, posting_freq); // frequency in doc
        }
      printf("document_writer_write_postings:posting_freq: %d\n", posting_freq);

      int last_position = 0; // write positions
      for (j = 0; j < posting_freq; j++)
        {
          int position = int_list_get_index(posting->positions, j);
          printf("document_writer_write_postings:position %d\n", position);
          printf("document_writer_write_postings:last_position %d\n",
              last_position);
          fs_write_vint(prox, position - last_position);
          last_position = position;
        }

      /* check to see if we switched to a new field */
      if (0 != strcmp(current_field, posting->term->field))
        {
          /* change field - see if there is something to save */
          current_field = posting->term->field;
          struct _field_info* fieldinfo = field_infos_get_by_name(
              dw->fieldinfos, current_field);

          if (fieldinfo->store_term_vector)
            {
              if (tvw == NULL)
                {
                  tvw = term_vectors_writer_initial(dw->dir, segment,
                      dw->fieldinfos);
                  term_vectors_writer_open_document(tvw);
                }
              term_vectors_writer_open_field(tvw, current_field);
            }
          else if (tvw != NULL)
            {
              term_vectors_writer_close_field(tvw);
            }
        }

      if (tvw != NULL && term_vectors_writer_is_field_open(tvw))
        {
          term_vectors_writer_add_term(tvw, posting->term->text, posting_freq);
        }
    }

  if (tvw != NULL)
    term_vectors_writer_close_document(tvw);

  if (freq != NULL)
    fs_close(freq);
  if (prox != NULL)
    fs_close(prox);
  if (tisw != NULL)
    term_infos_writer_close(tisw);
  if (tvw != NULL)
    term_vectors_writer_close(tvw);
}

void
write_norms(struct _documents_writer* _dw, struct _document* _doc,
    char* _segment)
{
  int i;
  for (i = 0; i < _dw->fieldinfos->size; i++)
    {
      struct _field_info* fi = field_infos_get_by_number(_dw->fieldinfos, i);
      if (fi->is_indexed)
        {
          float fieldboost = float_list_get_index(_dw->field_boosts, i);
          float norm = fieldboost * similarity_length_norm(_dw->similarity,
              fi->name, fieldboost);
          char* filesuffix = utils_strcat(".f", utils_itoa(i, 16));
          struct _file_stream* norms = directory_open_file(_dw->dir, _segment,
              filesuffix, "w");
          fs_write_byte(norms, 121);
          fs_close(norms);
        }
    }
}

int
compare_postings(struct _posting* p1, struct _posting* p2)
{
  return term_compare_to(p1->term, p2->term) > 0;
}

void
sort_postings_table(struct _documents_writer* dw)
{
  struct _posting psts[100];
  int i;
  int j = dw->postings->size;
  for (i = 0; i < j; i++)
    {
      psts[i] = term_postings_get_posting_index2(dw->postings, i);
    }

  qsort(&psts, dw->postings->size, sizeof(struct _posting), compare_postings);

  term_postings_clear(dw->postings);

  for (i = 0; i < j; i++)
    {
      term_postings_add(dw->postings, psts[i].term, &psts[i]);
    }
}

void
documents_writer_add_document(struct _documents_writer* dw, char* segment,
    struct _document* doc)
{
  /* write field names */
  dw->fieldinfos = field_infos_initial();
  field_infos_add_doc(dw->fieldinfos, doc);

  field_infos_write(dw->fieldinfos, dw->dir, segment, ".fnm");

  /* write field values */
  struct _fields_writer* fieldswriter = fields_writer_initial(dw->dir, segment,
      dw->fieldinfos);
  fields_writer_add_document(fieldswriter, doc);
  fields_writer_close(fieldswriter);

  /* invert doc into posting_table */
  term_postings_clear(dw->postings);
  dw->field_lengths = int_list_initial_fill(dw->fieldinfos->size, 0);
  dw->field_positions = int_list_initial_fill(dw->fieldinfos->size, 0);
  dw->field_boosts = float_list_initial_fill(dw->fieldinfos->size, 0.0f);
  float_list_add(dw->field_boosts, doc->boost);
  invert_document(dw, doc);

  sort_postings_table(dw);

  /* write postings */
  write_postings(dw, segment);

  /* write norms of indexed fields */
  write_norms(dw, doc, segment);
}

