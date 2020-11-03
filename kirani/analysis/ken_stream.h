/*
 *      Author: Thomas Yao
 */

#ifndef KEN_STREAM_H_
#define KEN_STREAM_H_

#include <stdio.h>

#include "../store/file_stream.h"

struct _ken_stream
{
  int ks_begin_column;
  int ks_end_column;
  struct _file_stream* f;
};

struct _ken_stream*
ken_stream_initial();

struct _token*
ks_next(struct _ken_stream* ks);

char*
kslex(struct _ken_stream* ks);

#endif /* KEN_STREAM_H_ */
