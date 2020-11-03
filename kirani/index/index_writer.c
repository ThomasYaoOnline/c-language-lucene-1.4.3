/*
 * author: Thomas Yao
 */

#include <err.h>
#include <ctype.h>
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "index_writer.h"
#include "segment_merger.h"

struct _index_writer*
index_writer_initial(struct _directory* _dir, struct _analyzer* _alz,
    int _create, int _close_dir)
{
  struct _index_writer* iw = (struct _index_writer*) malloc(
      sizeof(struct _index_writer));

  if (iw == NULL)
    err(1, "iw is null");
  iw->close_dir = _close_dir;
  iw->dir = _dir;
  iw->alz = _alz;
  iw->sis = segment_infos_initial();
  if (_create)
    segment_infos_write(iw->sis, _dir);
  else
    segment_infos_read(iw->sis, _dir);
  iw->use_compound_file = 0;
  return iw;
}

char*
new_segment_name(struct _index_writer* iw)
{
  char* counter_c = utils_itoa(iw->sis->counter++, 16);
  char* segment_name = utils_strcat("_", counter_c);
  free(counter_c);
  return segment_name;
}

void
merge_segments(struct _index_writer* iw, int min_segment)
{
  char* merged_name = new_segment_name(iw);
  struct _segment_merger* sm = segment_merger_initial(iw->dir, merged_name,
      iw->use_compound_file);

  struct _index_readers* segments_to_delete = index_readers_initial();
  struct _segment_info* si;
  struct _index_reader* ir;
  printf("%s\n", "index_writer_merge_segments:addreaders^^^^^^^^");
  printf("IndexWriter:mergeSegments:segmentInfos.size(): %d\n",iw->sis->size);
  int i;
  for (i = min_segment; i < iw->sis->size; i++)
    {
      si = segment_infos_get_index(iw->sis, i);
      ir = index_reader_initial2(NULL, si, 0);
      index_readers_add(sm->irs, ir);
      index_readers_add(segments_to_delete, ir);
    }
  printf("%s\n", "index_writer_merge_segments:addreaders$$$$$$$$$");

  int merged_doc_count = segment_merger_merge(sm);

  segment_infos_set_size(iw->sis, min_segment);
  segment_infos_add(iw->sis, segment_info_initial(merged_name,
      merged_doc_count, iw->dir));
  segment_merger_close_readers(sm);
  segment_infos_write(iw->sis, iw->dir);  // commit before deleting
  //deleteSegments(segmentsToDelete);  // delete now-unused segments

}

void
may_be_merge_segments(struct _index_writer* iw)
{
  long long target_merge_docs = INDEX_WRITER_DEFAULT_MIN_MERGE_DOCS;
  int min_segment;
  int merge_docs;
  struct _segment_info* si;
  while (target_merge_docs <= INDEX_WRITER_DEFAULT_MAX_MERGE_DOCS)
    {
      min_segment = iw->sis->size;
      merge_docs = 0;
      while (--min_segment >= 0)
        {
          si = segment_infos_get_index(iw->sis, min_segment);
          if (si->doc_count >= target_merge_docs)
            {
              break;
            }
          merge_docs += si->doc_count;
        }

      if (merge_docs >= target_merge_docs)
        merge_segments(iw, min_segment + 1);
      else
        break;

      target_merge_docs *= INDEX_WRITER_DEFAULT_MERGE_FACTOR;
    }
}

void
flash_ram_segments(struct _index_writer* iw)
{
  printf("\n");
  int min_segment = iw->sis->size - 1;
  int doc_count = 0;
  while (min_segment >= 0)
    {
      doc_count += segment_infos_get_index(iw->sis, min_segment)->doc_count;
      min_segment--;
      printf("index_writer_flash_ram_segments:min_segment>=0min_segment:%d\n",
          min_segment);
    }
  // add one FS segment?
  if (min_segment < 0 || (doc_count + segment_infos_get_index(iw->sis,
      min_segment)->doc_count) > INDEX_WRITER_DEFAULT_MERGE_FACTOR)
    {
      min_segment++;
      printf("index_writer_flash_ram_segments:min_segment<0:min_segment: %d\n",
          min_segment);
    }
  if (min_segment > iw->sis->size)
    return; // none to merge
  merge_segments(iw, min_segment);
}

void
index_writer_add_document(struct _index_writer* iw, struct _document* doc,
    struct _analyzer* analyzer)
{
  struct _documents_writer* dw = documents_writer_initial(iw->dir, analyzer,
      iw->sml, INDEX_WRITER_DEFAULT_MAX_FIELD_LENGTH);

  char* segment_name = new_segment_name(iw);

  documents_writer_add_document(dw, segment_name, doc);

  segment_infos_add(iw->sis, segment_info_initial(segment_name, 1, iw->dir));
  // free(segment_name);
  // may_be_merge_segments(iw);
}

void
index_writer_close(struct _index_writer* iw)
{
  flash_ram_segments(iw);
}
