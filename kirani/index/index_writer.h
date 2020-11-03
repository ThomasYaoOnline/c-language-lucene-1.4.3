/*
 * author: Thomas Yao
 */

#ifndef _INDEX_WRITER_H_
#define _INDEX_WRITER_H_

#include "documents_writer.h"
#include "segment_info.h"

#include "../store/directory.h"
#include "../store/lock.h"
#include "../analysis/analyzer.h"
#include "../document/document.h"
#include "../search/similarity.h"

#define INDEX_WRITER_WRITE_LOCK_TIMEOUT 1000
#define INDEX_WRITER_WRITE_LOCK_NAME "write.lock"

#define INDEX_WRITER_COMMIT_LOCK_TIMEOUT 10000
#define INDEX_WRITER_COMMIT_LOCK_NAME "commit.lock"

#define INDEX_WRITER_DEFAULT_MERGE_FACTOR 10

#define INDEX_WRITER_DEFAULT_MIN_MERGE_DOCS 10
#define INDEX_WRITER_DEFAULT_MAX_MERGE_DOCS 0x7fffffff

#define INDEX_WRITER_DEFAULT_MAX_FIELD_LENGTH 10000

struct _index_writer
{
  struct _directory* dir;
  struct _analyzer* alz;

  struct _similarity* sml;
  struct _segment_infos* sis;
  struct _lock* write_lock;
  int use_compound_file;
  int close_dir;
};

struct _index_writer*
index_writer_initial(struct _directory* _dir, struct _analyzer* _alz,
    int _create, int _close_dir);


void
index_writer_add_document(struct _index_writer* iw, struct _document* _doc,
    struct _analyzer* _alz);

void
index_writer_close(struct _index_writer* iw);

#endif

