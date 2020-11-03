/*
 * author: Thomas Yao
 */

#ifndef _LOCK_H_
#define _LOCK_H_

#define lock_LOCK_POLL_INTERVAL 1000

struct _lock
{
};

struct _lock*
lock_initial();

int lock_obtain();

void lock_release();

struct _with
{
  struct _lock* lock;
  long long lock_wait_timeout;
};

struct _with*
with_initial(struct _lock* _lk, long long _lock_wait_timeout);

#endif

