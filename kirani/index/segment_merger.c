/*
 * author: Thomas Yao
 */

#include <err.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "segment_merger.h"
#include "fields_writer.h"

const char* SEGMENT_MERGER_COMPOUND_EXTENSIONS[7] =
  { "fnm", "frq", "prx", "fdx", "fdt", "tii", "tis" };

const char* SEGMENT_MERGER_VECTOR_EXTENSIONS[3] =
  { "tvx", "tvd", "tvf" };

struct _segment_merger*
segment_merger_initial(struct _directory* _dir, char* _segment_name,
    int _compound_file)
{
  struct _segment_merger* sm = (struct _segment_merger*) malloc(
      sizeof(struct _segment_merger));
  if (sm == NULL)
    err(1, "sm is null");
  sm->dir = _dir;
  sm->segment = _segment_name;
  sm->use_compound_file = _compound_file;
  sm->terminfo = term_info_initial();
  sm->irs = index_readers_initial();
  sm->skip_buffer = fs_initial_mem2();
  return sm;
}

int
merge_fields(struct _segment_merger* sm)
{
  // merge field names
  sm->fis = field_infos_initial();
  int doc_count = 0;
  int i;
  struct _index_reader* ir;
  for (i = 0; i < sm->irs->size; i++)
    {
      ir = index_readers_get_index(sm->irs, i);
      field_infos_add_indexed(sm->fis, index_reader_get_indexed_fieldnames(ir,
          1), 1);
      field_infos_add_indexed(sm->fis, index_reader_get_indexed_fieldnames(ir,
          0), 0);
      field_infos_add_not_storetermvectors(sm->fis, inde_reader_get_fieldnames(
          ir, 0), 0);
    }
  field_infos_write(sm->fis, sm->dir, sm->segment, ".fnm");

  // merge field values
  struct _fields_writer* fieldswriter = fields_writer_initial(sm->dir,
      sm->segment, sm->fis);

  int j;
  for (i = 0; i < sm->irs->size; i++)
    {
      ir = index_readers_get_index(sm->irs, i);
      int max_doc = ir->fields_reader->size;
      for (j = 0; j < max_doc; j++)
        {
          if (!index_reader_is_deleted(ir, j)) // skip deleted docs
            {
              fields_writer_add_document(fieldswriter, index_reader_document(
                  ir, j));
              doc_count++;
            }
        }
    }
  fields_writer_close(fieldswriter);
  return doc_count;
}

void
reset_skip(struct _segment_merger* sm)
{
  fs_seek(sm->skip_buffer, 0);
  sm->last_skip_doc = 0;

  sm->last_skip_freq_pointer = fs_tell(sm->freq_output);
  printf("segment_merger:reset_skip:sm->last_skip_freq_pointer: %lli\n",
      sm->last_skip_freq_pointer);
  sm->last_skip_prox_pointer = fs_tell(sm->prox_output);
  printf("segment_merger:reset_skip:sm->last_skip_prox_pointer: %lli\n",
      sm->last_skip_prox_pointer);
}

void
buffer_skip(struct _segment_merger* sm, int doc)
{
  long long freq_pointer = fs_tell(sm->freq_output);
  printf("segment_merger:buffer_skip:freq_pointer: %lli\n", freq_pointer);
  long long prox_pointer = fs_tell(sm->prox_output);
  printf("segment_merger:buffer_skip:prox_pointer: %lli\n", prox_pointer);
  fs_write_vint(sm->skip_buffer, doc - sm->last_skip_doc);
  printf("segment_merger:buffer_skip:doc - sm->last_skip_doc: %i\n", doc
      - sm->last_skip_doc);
  fs_write_vint(sm->skip_buffer, (int) (freq_pointer
      - sm->last_skip_freq_pointer));
  printf(
      "segment_merger:buffer_skip:(int) (freq_pointer - sm->last_skip_freq_pointer): %i\n",
      (int) (freq_pointer - sm->last_skip_freq_pointer));
  fs_write_vint(sm->skip_buffer, (int) (prox_pointer
      - sm->last_skip_prox_pointer));
  printf(
      "segment_merger:buffer_skip:(int) (prox_pointer - sm->last_skip_prox_pointer): %i\n",
      (int) (prox_pointer - sm->last_skip_prox_pointer));

  sm->last_skip_doc = doc;
  sm->last_skip_freq_pointer = freq_pointer;
  sm->last_skip_prox_pointer = prox_pointer;
}
int
append_postings(struct _segment_merger* sm, struct _segment_merge_infos* smis,
    int size)
{
  int last_doc = 0;
  int df = 0; // number of docs w/ term
  reset_skip(sm);

  int i;
  for (i = 0; i < size; i++)
    {
      struct _segment_merge_info* smi = segment_merge_infos_get_index(smis, i);
      term_docs_seek_term_enum(smi->postings, smi->term_enum);

      while (term_docs_next(smi->postings))
        {
          int doc = smi->postings->doc;
          if (smi->doc_map != NULL)
            doc = int_list_get_index(smi->doc_map, doc);
          doc += smi->base;

          if (doc < last_doc)
            err(1, "doc out of order");

          df++;

          if ((df % sm->skip_interval) == 0)
            buffer_skip(sm, last_doc);

          int doc_code = (doc - last_doc) << 1; // use low bit to flag freq=1
          last_doc = doc;

          int freq = smi->postings->freq;
          printf("segmentmerger:appendpostings:doccode: %i\n", doc_code);
          printf("segmentmerger:appendpostings:freq: %i\n", freq);

          if (freq == 1)
            {
              fs_write_vint(sm->freq_output, (doc_code | 1)); // write doc & freq=1
            }
          else
            {
              fs_write_vint(sm->freq_output, doc_code); // write doc
              fs_write_vint(sm->freq_output, freq); // write frequency in doc
            }

          int lastPosition = 0; // write position deltas
          int j;
          for (j = 0; j < freq; j++)
            {
              int position = term_docs_next_position(smi->postings);
              fs_write_vint(sm->prox_output, (position - lastPosition));
              printf(
                  "SegmentMerger:appendPostings:(position - lastPosition): %d\n"
                      + (position - lastPosition));
              lastPosition = position;
            }

        }
    }
  return df;
}

long long
write_skip(struct _segment_merger* sm)
{
  long long skip_pointer = fs_tell(sm->freq_output);
  fs_write_to(sm->skip_buffer, sm->freq_output);
  return skip_pointer;
}

void
merge_term_info(struct _segment_merger* sm, struct _segment_merge_infos* smis,
    int size)
{
  long long freq_pointer = fs_tell(sm->freq_output);
  printf("segment_merger:merge_term_info:freq_pointer: %lli\n", freq_pointer);

  long long prox_pointer = fs_tell(sm->prox_output);
  printf("segment_merger:merge_term_info:prox_pointer: %lli\n", prox_pointer);

  printf("%s\n", "SegmentMerger:mergeTermInfo:appendPostings:^^^^^^^^^^");
  int df = append_postings(sm, smis, size); // append posting data
  printf("%s\n", "SegmentMerger:mergeTermInfo:appendPostings:$$$$$$$$$$");

  long long skip_pointer = write_skip(sm);

  if (df > 0)
    {
      // add an entry to the dictionary with pointers to prox and freq files
      term_info_set(sm->terminfo, df, freq_pointer, prox_pointer,
          (int) (skip_pointer - prox_pointer));
      term_infos_writer_add(sm->tisw,
          segment_merge_infos_get_index(smis, 0)->term, sm->terminfo);
    }
}

void
merge_term_infos(struct _segment_merger* sm)
{
  int base = 0;
  int i;
  printf("\n");
  printf("%s\n", "SegmentMerger:mergeTermInfos:SegmentMergerInfo:Add:^^^^^^^^^");
  for (i = 0; i < sm->irs->size; i++)
    {
      struct _index_reader* ir = index_readers_get_index(sm->irs, i);
      struct _term_enum* term_enum = index_reader_terms(ir);
      struct _segment_merge_info* smi = segment_merge_info_initial(base,
          term_enum, ir);
      base += index_reader_num_docs(ir);
      if (segment_merger_info_next(smi))
        segment_merge_infos_add(sm->queue, smi);
      else
        segment_merge_info_close(smi);
    }
  printf("%s\n", "SegmentMerger:mergeTermInfos:SegmentMergerInfo:Add:$$$$$$$$$");
  printf("SegmentMerger:mergeTermInfos:SegmentMergerInfo:queue.size: %i\n",
      sm->queue->size);

  struct _segment_merge_infos* match = segment_merge_infos_initial();
  for (i = 0; i < sm->irs->size; i++)
    {
      segment_merge_infos_add(match, NULL);
    }

  while (sm->queue->size > 0)
    {
      int match_size = 0;
      segment_merge_infos_set_index(match, match_size++,
          segment_merge_infos_pop(sm->queue));
      struct _term* term = segment_merge_infos_get_index(match, 0)->term;
      struct _segment_merge_info* top = segment_merge_infos_top(sm->queue);

      while (top != NULL && 0 == term_compare_to(term, top->term))
        {
          segment_merge_infos_set_index(match, match_size++,
              segment_merge_infos_pop(sm->queue));
          top = segment_merge_infos_top(sm->queue);
        }

      printf("%s\n",
          "SegmentMerger:mergeTermInfos:SegmentMergerInfo:merge_term_info:^^^^^^^^^");
      merge_term_info(sm, match, match_size); // add new TermInfo
      printf("%s\n",
          "SegmentMerger:mergeTermInfos:SegmentMergerInfo:merge_term_info:$$$$$$$$$");
      printf("\n");

      while (match_size > 0)
        {
          struct _segment_merge_info* smi = segment_merge_infos_get_index(
              match, --match_size);
          if (segment_merger_info_next(smi))
            segment_merge_infos_add(sm->queue, smi);// restore queue
          else
            segment_merge_info_close(smi); // done with a segment
        }
    }
}

void
merge_terms(struct _segment_merger* sm)
{
  sm->freq_output = directory_open_file(sm->dir, sm->segment, ".frq", "w");
  sm->prox_output = directory_open_file(sm->dir, sm->segment, ".prx", "w");
  sm->tisw = term_infos_writer_initial1(sm->dir, sm->segment, sm->fis);
  sm->skip_interval = sm->tisw->skip_interval;
  sm->queue = segment_merge_infos_initial();

  printf("%s\n", "SegmentMerger:mergeTerms:mergeTermInfos:^^^^^^^^^^^");
  merge_term_infos(sm);
  printf("%s\n", "SegmentMerger:mergeTerms:mergeTermInfos:$$$$$$$$$$$");

  if (sm->freq_output != NULL)
    fs_close(sm->freq_output);
  if (sm->prox_output != NULL)
    fs_close(sm->prox_output);
  if (sm->tisw != NULL)
    term_infos_writer_close(sm->tisw);
  if (sm->queue != NULL)
    segment_merge_infos_close(sm->queue);
}

void
merge_norms(struct _segment_merger* sm)
{
  int i;
  int j;
  int k;
  for (i = 0; i < sm->fis->size; i++)
    {
      struct _field_info* fi = field_infos_get_index(sm->fis, i);
      if (fi->is_indexed)
        {
          char* filesuffix = utils_strcat(".f", utils_itoa(i, 16));

          struct _file_stream* output = directory_open_file(sm->dir,
              sm->segment, filesuffix, "w");

          for (j = 0; j < sm->irs->size; j++)
            {
              struct _index_reader* reader =
                  index_readers_get_index(sm->irs, j);
              struct _int_list* input = index_reader_norms(reader, fi->name);
              int max_doc = index_reader_max_doc(reader);
              for (k = 0; k < max_doc; k++)
                {
                  int norm = input != NULL ? int_list_get_index(input, k)
                      : (int) 0;
                  if (!index_reader_is_delete(reader, k))
                    printf("segment_merger:merge_norms:norm: %d\n", norm);
                  fs_write_byte(output, norm);
                }
            }
          fs_close(output);
        }
    }
}

int
segment_merger_merge(struct _segment_merger* sm)
{
  int value;

  printf("%s\n", "SegmentMerger:merge:mergeFields:^^^^^^^^^^^");
  value = merge_fields(sm);
  printf("%s\n", "SegmentMerger:merge:mergeFields:$$$$$$$$$$$");

  printf("%s\n", "SegmentMerger:merge:mergeTerms:^^^^^^^^^^^^");
  merge_terms(sm);
  printf("%s\n", "SegmentMerger:merge:mergeTerms:$$$$$$$$$$$$");

  printf("%s\n", "SegmentMerger:merge:mergenorms:^^^^^^^^^^^^");
  merge_norms(sm);
  printf("%s\n", "SegmentMerger:merge:mergenorms:$$$$$$$$$$$$");
  return value;
}

void
segment_merger_close_readers(struct _segment_merger* sm)
{
  int i;
  struct _index_reader* ir;
  for (i = 0; i < sm->irs->size; i++)
    {
      ir = index_readers_get_index(sm->irs, i);
      index_reader_close(ir);
    }
}
