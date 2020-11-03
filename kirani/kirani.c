/*
 * author: Thomas Yao
 */

#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./document/document.h"
#include "./index/index_writer.h"
#include "./search/index_searcher.h"
#include "./search/hits.h"
#include "./store/directory.h"
#include "./store/file_stream.h"
#include "./utils/char_list.h"

struct _document*
file_document(char* _file_name)
{
  //int _is_stored, int _is_indexed, int _is_tokenized, int _store_term_vector)
  struct _document* doc = document_initial();
  document_add(doc, field_initial("path", _file_name, NULL, 1, 1, 1, 0));
  document_add(doc, field_initial("modified", "date", NULL, 1, 1, 0, 0));
  document_add(doc, field_initial("contents", NULL,
      fs_initial(_file_name, "r"), 0, 1, 1, 0));

  return doc;
}

void
index_docs(char* sdoc, char* index)
{
  struct _directory* dir = directory_initial(sdoc, index, 0);
  directory_pop_files(dir);

  struct _analyzer* alz = analyzer_initial();
  struct _index_writer* iw = index_writer_initial(dir, alz, 1, 1);

  int i;
  for (i = 0; i < dir->file_names->size; i++)
    {
      char* file_name = char_list_get_index(dir->file_names, i);
      printf("adding %s\n", file_name);
      index_writer_add_document(iw, file_document(file_name), alz);
    }
  index_writer_close(iw);

}

void
search_docs(char* _path)
{
  struct _index_searcher* is = index_searcher_initial(_path, 1);
  struct _analyzer* alz = analyzer_initial();

  while (1)
    {
      printf("%s", "Query: ");
      char buffer[20];
      fgets(buffer, 20, stdin);
      struct _query* query = qp_parse(buffer, "contents", alz);

      printf("search_docs:query:tostring:field: %s; text: %s\n",
          query->tm->field, query->tm->text);
      printf("search_docs:searchingfor: %s\n", query->tm->text);

      struct _hits* hits = index_searcher_search_hits(is, query, NULL);
      printf("%d total matching documents\n", hits->length);

      const int HITS_PER_PAGE = 10;
      int start;
      for (start = 0; start < hits->length; start += HITS_PER_PAGE)
        {
          int end = hits->length < (start + HITS_PER_PAGE) ? hits->length
              : (start + HITS_PER_PAGE);
          int i;
          for (i = start; i < end; i++)
            {
              struct _document* doc = hits_doc(hits, i);
              char* path =
                  fields_get_by_name(doc->fields, "path")->string_value;
              if (path != NULL)
                {
                  printf("%d. %s\n", i, path);
                }
              else
                {
                  char* url =
                      fields_get_by_name(doc->fields, "url")->string_value;
                  if (url != NULL)
                    {
                      printf("%d.%s\n", i, url);
                      printf(
                          "%s   - ",
                          fields_get_by_name(doc->fields, "title")->string_value);
                    }
                  else
                    {
                      printf("%d.%s\n", i, "No path nor URL for this document");
                    }
                }
            }
        }
    }
  index_searcher_close(is);
}

int
main(int argc, char* argv[])
{
/*  if (0 == strcmp(argv[1], "s"))
    search_docs("/root/index");

  if (0 == strcmp(argv[1], "i"))
    index_docs("/root/docs", "/root/index");
  printf("%s\n", "Oops, bye....");*/

  ts_run();
//  list_test_main();
  return 0;
}

