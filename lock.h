#ifndef LOCK_H
#define LOCK_H

#include "system.h"
#include "thread.h"
#include "scheduler.h"

#define SPINLOCK_LOCKED 0
#define SPINLOCK_UNLOCKED 1

typedef volatile uint32_t spinlock_t;

typedef struct semaphore
{
  uint32_t counter;
  spinlock_t lock;
  thread_list_t queue;
} semaphore_t;

void spinlock_lock (spinlock_t *lock);

void spinlock_unlock (spinlock_t *lock);

#endif