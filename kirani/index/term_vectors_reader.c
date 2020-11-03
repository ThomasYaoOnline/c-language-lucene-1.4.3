/*
* author: Thomas Yao
*/

#include <err.h>
#include <stdio.h>
#include <stdlib.h>

#include "term_vector.h"
#include "term_vectors_reader.h"
#include "term_vectors_writer.h"


#include "../utils/utils.h"

void check_valid_format(struct _file_stream* in)
{
  int format = fs_read_int(in);
  if (format > TERM_VECTORS_WRITER_FORMAT_VERSION)
    err(1, "Incompatible format version");
}

struct _term_vectors_reader*
term_vectors_reader_initial(struct _directory* _dir,
                            char* _segment,
                            struct _field_infos* _fis)
{
  struct _term_vectors_reader* tvr = (struct _term_vectors_reader*) malloc(
      sizeof(struct _term_vectors_reader));
  //if (_dir.isExists(utils_strcat(_segment, ".tvx")))
    //{
      tvr->tvx = directory_open_file(_dir, _segment,"tvx", "r");
      check_valid_format(tvr->tvx);
      tvr->tvd = directory_open_file(_dir, _segment,"tvd", "r");
      check_valid_format(tvr->tvd);
      tvr->tvf = directory_open_file(_dir, _segment,"tvf", "r");
      check_valid_format(tvr->tvf);
      tvr->size = (int) tvr->tvx->length/8;
    //}
  tvr->fis = _fis;
  return tvr;
}

void term_vectors_reader_close(struct _term_vectors_reader* _tvr)
{
  if (_tvr->tvx != NULL)
    fs_close(_tvr->tvx);
  if (_tvr->tvd != NULL)
    fs_close(_tvr->tvd);
  if (_tvr->tvf != NULL)
    fs_close(_tvr->tvf);
}

struct _term_vector*
read_term_vector(struct _term_vectors_reader* _tvr,
                 long long tvf_pointer,
                 char* _field)
{
  fs_seek(_tvr->tvf, tvf_pointer);
  int num_terms = fs_read_int(_tvr->tvf);

  if (num_terms == 0)
    return term_vector_initial(_field, NULL, NULL);

  //int length = num_terms + fs_read_int(_tvr->tvf);
  char* terms[1024];
  int term_freqs[1024];

  int start = 0;
  int delta_length = 0;
  int total_length = 0;
  //char buffer[1024];
  //char* previous_string = "";
  int i;
  for (i=0; i<num_terms; i++)
    {
      start = fs_read_int(_tvr->tvf);
      delta_length = fs_read_int(_tvr->tvf);
      total_length = start + delta_length;

      //fs_read_chars(buffer, start, delta_length);
      //terms[i] = buffer;
      //previous_string = terms[i];
      term_freqs[i] = fs_read_int(_tvr->tvf);
    }

  struct _term_vector* tv = term_vector_initial(_field, terms, term_freqs);
  return tv;
}

struct _term_vector*
term_vectors_reader_get(struct _term_vectors_reader* _tvr, int _doc_num,
                        char* _field)
{
  int field_number = field_infos_get_number_by_name(_tvr->fis, _field);
  struct _term_vector* result;
  if (_tvr->tvx != NULL)
    {
      fs_seek(_tvr->tvx, (_doc_num*8L+TERM_VECTORS_WRITER_FORMAT_SIZE));
      long long position = fs_read_long(_tvr->tvx);
      fs_seek(_tvr->tvd, position);
      int field_count = fs_read_int(_tvr->tvd);
      int number =0;
      int found = -1;
      int i;
      for (i=0; i<field_count; i++)
        {
          number += fs_read_int(_tvr->tvd);
          if (number == field_number)
            found =i;
        }

      if (found != -1)
        {
          position = 0;
          for (i=0; i<=found; i++)
            {
              position += fs_read_long(_tvr->tvd);
            }
          result = read_term_vector(_tvr, position, _field);
        }
      else
        {
          err(1, "field not found");
        }
    }
  else
    {
      err(1, "no tvx file");
    }
  return result;
}


