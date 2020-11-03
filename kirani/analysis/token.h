/*
 * author: Thomas Yao
 */

#ifndef _TOKEN_H_
#define _TOKEN_H_

struct _token
{
  char* term_text;
  int start_offset;
  int end_offset;
  char* type;
  int position_increment;
};

struct _token*
token_initial();

struct _token*
token_initial1(char* text, int start, int end);

struct _token*
token_initial2(char* text, int start, int end, char* type);

#endif
