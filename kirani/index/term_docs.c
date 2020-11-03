/*
 * author: Thomas Yao
 */

#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "term_docs.h"

struct _term_docs*
term_docs_initial(struct _index_reader* parent)
{
  struct _term_docs* term_docs = (struct _term_docs*) malloc(
      sizeof(struct _term_docs));
  if (term_docs == NULL)
    err(1, "term_docs is null");
  term_docs->parent = parent;
  term_docs->freq_stream = fs_initial_clone(parent->freq_stream);
  term_docs->deleted_docs = parent->deleted_docs;
  term_docs->skip_interval
      = parent->term_infos_reader->orig_enum->skip_interval;
  term_docs->prox_stream = term_docs->parent->prox_stream;
  return term_docs;
}

void
term_docs_seek_term(struct _term_docs* term_docs, struct _term* _term)
{
  struct _term_info* ti = term_infos_reader_get(
      term_docs->parent->term_infos_reader, _term);
  term_docs_seek_term_info(term_docs, ti);
}

void
term_docs_seek_term_info(struct _term_docs* term_docs, struct _term_info* ti)
{
  term_docs->count = 0;
  if (ti == NULL)
    {
      term_docs->df = 0;
      printf("term_docs_seek_term_info:ti==NULL:term_docs->df: %i\n",
          term_docs->df);
    }
  else
    {
      term_docs->df = ti->doc_freq;
      printf("term_docs_seek_term_info:term_docs->df: %i\n", term_docs->df);
      term_docs->doc = 0;
      term_docs->skip_doc = 0;
      term_docs->skip_count = 0;
      term_docs->num_skips = term_docs->df / term_docs->skip_interval;
      term_docs->freq_pointer = ti->freq_pointer;
      term_docs->prox_pointer = ti->prox_pointer;
      term_docs->skip_pointer = term_docs->freq_pointer + ti->skip_offset;
      fs_seek(term_docs->freq_stream, term_docs->freq_pointer);
      printf("term_docs_seek_term_info:freqpointer: %li\n",
          term_docs->freq_pointer);
      term_docs->have_skipped = 0;
    }

  if (ti != NULL)
    {
      fs_seek(term_docs->prox_stream, term_docs->prox_pointer);
    }
  term_docs->prox_count = 0;
}

void
term_docs_seek_term_enum(struct _term_docs* term_docs,
    struct _term_enum* term_enum)
{
  struct _term_info* ti;
  // use comparison of fieldinfos to verify that termEnum belongs
  // to the same segment as this SegmentTermDocs
  if (term_enum->fieldinfos == term_docs->parent->field_infos)
    {
      ti = term_info_initial_copy(term_enum->terminfo);
    }
  else
    {
      ti = term_infos_reader_get(term_docs->parent->term_infos_reader,
          term_enum->term);
    }
  term_docs_seek_term_info(term_docs, ti);
}

void
term_docs_close(struct _term_docs* term_docs)
{
  //fs_close(term_docs->freq_stream);
  if (term_docs->skip_stream != NULL)
    fs_close(term_docs->skip_stream);
}

void
skipping_doc()
{

}

int
term_docs_next(struct _term_docs* postings)
{
  int f;
  for (f = postings->prox_count; f > 0; f--)
    fs_read_vint(postings->freq_stream); // skip unread positions

  while (1)
    {
      printf("term_docs_next:postings->count: %d\n", postings->count);
      printf("term_docs_next:postings->df: %d\n", postings->df);
      if (postings->count == postings->df)
        {
          printf("term_docs_next: %s\n",
              "postings->count == postings->df break");
          return 0;
          break;
        }
      int doc_code = fs_read_vint(postings->freq_stream);
      printf("term_docs_next:doc_code: %d\n", doc_code);

      postings->doc += doc_code >> 1; // shift off low bit
      if ((doc_code & 1) != 0) // if low bit is set
        postings->freq = 1; // freq is one
      else
        postings->freq = fs_read_vint(postings->freq_stream); // else read freq
      printf("term_docs_next:postings->freq: %d\n", postings->freq);

      postings->count++;

      if (postings->deleted_docs == NULL || !int_list_get_index(
          postings->deleted_docs, postings->doc))
        {
          printf("%s\n", "term_docs:next:deleted:break:");
          break;
        }
      skipping_doc();
    }

  postings->prox_count = postings->freq;
  postings->position = 0;
  return 1;
}

int
term_docs_next_position(struct _term_docs* postings)
{
  postings->prox_count--;
  return postings->position += fs_read_vint(postings->prox_stream);
}

int
term_docs_read(struct _term_docs* term_docs, struct _int_list* docs,
    struct _int_list* freqs)
{
  int length = docs->size;
  int i = 0;
  printf("term_docs_read:length: %i\n", length);
  printf("term_docs_read:term_docs->count: %i\n", term_docs->count);
  printf("term_docs_read:term_docs->df: %i\n", term_docs->df);
  while (i < length && term_docs->count < term_docs->df)
    {
      // manually inlined call to next() for speed
      int doc_code = fs_read_vint(term_docs->freq_stream);
      printf("term_docs_read:doc_code: %i\n", doc_code);
      term_docs->doc += doc_code >> 1; // shift off low bit
      if ((doc_code & 1) != 0) // if low bit is set
        {
          printf("term_docs_read:term_docs->freq: is one\n");
          term_docs->freq = 1; // freq is one
        }
      else
        {
          term_docs->freq = fs_read_vint(term_docs->freq_stream); // else read freq
          printf("term_docs_read:term_docs->freq: %i\n", term_docs->freq);
        }
      term_docs->count++;

      if (term_docs->deleted_docs == NULL || !int_list_get_index(
          term_docs->deleted_docs, term_docs->doc))
        {
          int_list_set_index(docs, i, term_docs->doc);
          int_list_set_index(freqs, i, term_docs->freq);
          ++i;
        }
    }
  return i;
}
