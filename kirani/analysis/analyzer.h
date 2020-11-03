/*
 * author: Thomas Yao
 */

#ifndef _ANALYZER_H_
#define _ANALYZER_H_

#include "token_stream.h"

#include "../utils/char_list.h"

struct _analyzer
{
  struct _char_list* stop_words;
};

struct _analyzer*
analyzer_initial();

struct _token_stream*
analyzer_token_stream(struct _analyzer* alz, char* field, char* query_text);

#endif

