/*
* author: Thomas Yao
*/

#ifndef _KIRANI_H_
#define _KIRANI_H_

#include <wchar.h>

#include "./analysis/analyzer.h"
#include "./search/query.h"

struct _query* q;
struct _query* qp_parse(char* _line, char* _field_name, struct _analyzer* _alz);
char* qpinput;
char* qpinputptr;
char* qpinputlim;

int ts_run();

#endif
