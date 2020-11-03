/*
 * author: Thomas Yao
 */

#ifndef _FILTER_H_
#define _FILTER_H_

#include "../index/index_reader.h"

struct _filter
{
};

struct _int_list*
filter_bits(struct _index_reader* _is);

#endif
