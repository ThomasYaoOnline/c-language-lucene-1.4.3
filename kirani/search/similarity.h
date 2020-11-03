/*
 * author: Thomas Yao
 */

#ifndef _SIMILARITY_H_
#define _SIMILARITY_H_

#include <math.h>
#include <complex.h>

#include "index_searcher.h"

#include "../index/index_reader.h"
#include "../index/term_posting.h"

struct _similarity
{
};

struct _similarity*
similarity_initial();

float
similarity_decode_norm(int b);

float
similarity_query_norm(struct _similarity* _sml, float _sum_of_squared_weights);

float
similarity_length_norm(struct _similarity* _sml, char* fiend_name, float num_terms);

float
similarity_tf(struct _similarity* _sml, float freq);

float
similarity_coord(int _over_lap, int _max_over_lap);

float
similarity_idf(struct _term* _tm, struct _index_searcher* _is);

#endif
