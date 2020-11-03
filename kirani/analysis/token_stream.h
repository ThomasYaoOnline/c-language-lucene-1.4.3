/*
 * author: Thomas Yao
 */

#ifndef _TOKEN_STREAM_H_
#define _TOKEN_STREAM_H_

#include "../store/file_stream.h"

struct _token_stream
{
  struct _char_list* stop_words;
  char* term_text;
};

char* tslval;
int ts_begin_column;
int ts_end_column;


struct _token_stream* 
ts_initial();

struct _char_list*
ts_make_stop_list(char* _stop_words[33], int _size, int _ignore_case);

struct _token* 
ts_next();

void 
ts_echo(char* name, char* text);

#endif

