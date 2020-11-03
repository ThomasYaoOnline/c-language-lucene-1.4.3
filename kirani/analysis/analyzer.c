/*
 * author: Thomas Yao
 */

#include <err.h>
#include <stdlib.h>

#include "analyzer.h"
#include "token_stream.h"

const char* ANALYZER_ENGLISH_STOP_WORDS[33] =
  { "a", "an", "and", "are", "as", "at", "be", "but", "by", "for", "if", "in",
      "into", "is", "it", "no", "not", "of", "on", "or", "such", "that", "the",
      "their", "then", "there", "these", "they", "this", "to", "was", "will",
      "with" };

struct _analyzer*
analyzer_initial()
{
  struct _analyzer* alz = (struct _analyzer*) malloc(sizeof(struct _analyzer));
  if (alz == NULL)
    err(1, "alz is null");

  alz->stop_words = ts_make_stop_list(ANALYZER_ENGLISH_STOP_WORDS, 33, 0);
  return alz;
}

struct _token_stream*
analyzer_token_stream(struct _analyzer* alz, char* field, char* query_text)
{
  return ts_initial();
}

