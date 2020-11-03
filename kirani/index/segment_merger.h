/*
 * author: Thomas Yao
 */

#ifndef _SEGMENT_MERGER_H_
#define _SEGMENT_MERGER_H_

#include "field_info.h"
#include "index_reader.h"
#include "segment_merge_info.h"
#include "term_infos_writer.h"

#include "../store/directory.h"
#include "../store/file_stream.h"


struct _segment_merger
{
  int use_compound_file;
  struct _directory* dir;
  char* segment;
  struct _index_readers* irs;
  struct _field_infos* fis;

  struct _file_stream* freq_output;
  struct _file_stream* prox_output;
  struct _term_infos_writer* tisw;
  int skip_interval;
  struct _segment_merge_infos* queue;

  struct _term_info* terminfo;

  struct _file_stream* skip_buffer;
  int last_skip_doc;
  long long last_skip_freq_pointer;
  long long last_skip_prox_pointer;
};

struct _segment_merger*
segment_merger_initial(struct _directory* _dir, char* _segment_name,
    int _compound_file);
int
segment_merger_merge(struct _segment_merger* sm);
void
segment_merger_close_readers(struct _segment_merger* sm);

#endif /* SEGMENT_MERGER_H_ */
