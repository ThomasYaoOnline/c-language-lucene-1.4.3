/*
 * author: Thomas Yao
 */

#include <err.h>
#include <stdlib.h>
#include <stdio.h>

#include "term_infos_reader.h"

#include "../utils/utils.h"
#include "../utils/long_long_list.h"

void
read_index(struct _term_infos_reader* tir)
{
  struct _term_enum* index_enum = term_enum_initial(directory_open_file(
      tir->dir, tir->segment, ".tii", "r"), tir->fis, 1);

  int index_size = (int) index_enum->size;
  tir->index_terms = term_postings_initial();
  tir->index_infos = term_infos_initial();
  tir->index_pointers = long_long_list_initial();
  int i;
  for (i = 0; term_enum_next(index_enum); i++)
    {
      term_postings_add(tir->index_terms, index_enum->term, NULL);
      term_infos_add(tir->index_infos, index_enum->terminfo);
      long_long_list_add(tir->index_pointers, index_enum->index_pointer);
    }
  if (index_size != i)
    err(1, "term count error");
  term_enum_close(index_enum);
}

struct _term_infos_reader*
term_infos_reader_initial(struct _directory* dir, char* segment,
    struct _field_infos* fieldinfos)
{
  struct _term_infos_reader* tisr = (struct _term_infos_reader*) malloc(
      sizeof(struct _term_infos_reader));
  if (tisr == NULL)
    err(1, "tir is null");

  tisr->dir = dir;
  tisr->segment = segment;
  tisr->fis = fieldinfos;
  tisr->orig_enum = term_enum_initial(directory_open_file(dir, tisr->segment,
      ".tis", "r"), tisr->fis, 0);
  tisr->size = tisr->orig_enum->size;
  read_index(tisr);
  return tisr;
}

struct _term_enum*
get_enum(struct _term_infos_reader* tisr)
{
  return tisr->orig_enum;
}

struct _term_info*
scan_enum(struct _term_infos_reader* tisr, struct _term* term)
{
  // Scans within block for matching term.
  struct _term_enum* enumerator = get_enum(tisr);
  while (term_compare_to(term, enumerator->term) != 0 && term_enum_next(
      enumerator))
    {
    }
  if (enumerator->term != NULL && term_compare_to(term, enumerator->term) == 0)
    return enumerator->terminfo;
  else
    return NULL;
}

int
get_index_offset(struct _term_infos_reader* tisr, struct _term* term)
{
  int lo = 0; // binary search indexTerms[]
  int hi = tisr->index_terms->size - 1;

  while (hi >= lo)
    {
      int mid = (lo + hi) >> 1;
      int delta = term_compare_to(term, term_postings_get_term_index(
          tisr->index_terms, mid));
      if (delta < 0)
        hi = mid - 1;
      else if (delta > 0)
        lo = mid + 1;
      else
        return mid;
    }
  return hi;
}

void
seek_enum(struct _term_infos_reader* tisr, int index_offset)
{
  term_enum_seek(get_enum(tisr), long_long_list_get_index(tisr->index_pointers,
      index_offset), (index_offset * get_enum(tisr)->index_interval) - 1,
      term_postings_get_term_index(tisr->index_terms, index_offset),
      term_infos_get_index(tisr->index_infos, index_offset));
}

struct _term_info*
term_infos_reader_get(struct _term_infos_reader* tisr, struct _term* term)
{
  if (tisr->size == 0)
    return NULL;
  // optimize sequential access: first try scanning cached enum w/o seeking
  struct _term_enum* enumerator = get_enum(tisr);
  if (enumerator->term != NULL &&
      ((enumerator->prev != NULL && term_compare_to( term, enumerator->prev) > 0)
      || term_compare_to(term, enumerator->term) >= 0))
    {
      int enum_offset = (int) (enumerator->position
          / enumerator->index_interval) + 1;
      if (tisr->index_terms->size == enum_offset || term_compare_to(term,
          term_postings_get_term_index(tisr->index_terms, enum_offset)) < 0) // but before end of block
        return scan_enum(tisr, term);
    }
  // random-access: must seek
  seek_enum(tisr, get_index_offset(tisr, term));
  return scan_enum(tisr, term);
}

struct _term_enum*
term_infos_reader_terms(struct _term_infos_reader* tisr)
{
  return tisr->orig_enum;
}
