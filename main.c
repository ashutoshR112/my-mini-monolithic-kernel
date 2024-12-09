#include "system.h"
#include "descriptor_tables.h"
#include "paging.h"
#include "heap.h"
#include "kmalloc.h"

#include "thread.h"
#include "scheduler.h"
#include "timer.h"

int fn(void *arg)
{
  for(;;) {
    printk("a");
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
    printk("b");
  }

    for(;;);

    return 0xDEADBABA;
}
