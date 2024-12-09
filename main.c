#include "system.h"
#include "descriptor_tables.h"
#include "paging.h"
#include "heap.h"
#include "kmalloc.h"

#include "thread.h"
#include "scheduler.h"
#include "timer.h"
#include "lock.h"

spinlock_t lock = SPINLOCK_UNLOCKED;

int fn(void *arg)
{
for(;;) {
    int i;
    spinlock_lock(&lock);
    for(i = 0; i < 80; i++)
      printk("a = %d\n", i);
    spinlock_unlock(&lock);
  }
  return 6;
}

int main(void *mboot_ptr)
{
    init_descriptor_tables();
	init_paging();

    init_timer (20);

      asm volatile ("sti");
  init_scheduler (init_threading ());

  uint32_t *stack = kmalloc (0x400) + 0x3F0;

  thread_t *t = create_thread(&fn, (void*)0x567, stack);
  for(;;) {
    int i;
    spinlock_lock(&lock);
    for(i = 0; i < 80; i++)
      printk("b = %d\n", i);
    spinlock_unlock(&lock);
  }

    for(;;);

    return 0xDEADBABA;
}
