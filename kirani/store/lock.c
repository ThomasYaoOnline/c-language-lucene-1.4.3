/*
 * author: Thomas Yao
 */

#include <err.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <wchar.h>

#include "lock.h"

struct _lock*
lock_initial()
{
  struct _lock* lk = (struct _lock*) malloc(sizeof(struct _lock));
  if (lk == NULL)
    err(1, "lk is null");
  return lk;
}

int lock_obtain()
{
  return 1;
}

void lock_release()
{
  return;
}

struct _with*
with_initial(struct _lock* _lk, long long _lock_wait_timeout)
{
  struct _with* wth = (struct _with*) malloc(sizeof(struct _with));
  if (wth == NULL)
    err(1, "wth is null");
  wth->lock = _lk;
  wth->lock_wait_timeout = _lock_wait_timeout;
  return wth;
}
