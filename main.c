#include "system.h"
#include "descriptor_tables.h"
#include "paging.h"
#include "heap.h"
#include "kmalloc.h"

#include "thread.h"
#include "scheduler.h"
#include "timer.h"


int fn(void *arg) {
    for (;;) {
        int i;
        for (i = 0; i < 80; i++) {
            printk("a = %d\n", i);
        }
    }
    return 6;
}

int main(void *mboot_ptr) {
    init_descriptor_tables();
    init_paging();
    init_timer(20);

    asm volatile ("sti");
    init_scheduler(init_threading());

    uint32_t *stack = kmalloc(0x400) + 0x3F0;
    thread_t *t = create_thread(&fn, (void *)0x567, stack);
   

    for (;;) {
        int i;
        for (i = 0; i < 80; i++) {
            printk("b = %d\n", i);
        }
    }

    for (;;);

    return 0xDEADBABA;
}

//changes from here****************************************************************************************************************
// #include  "system.h"
// #include  "descriptor_tables.h"
// #include  "timer.h"
// #include  "paging.h"
// #include  "kmalloc.h"

// int  main(void  *mboot_ptr) {
//     init_descriptor_tables();
//     init_paging();
    
//     asm  volatile ("sti");
    
//     uint32_t  *a  =  kmalloc(8);
//     uint32_t  *b  =  kmalloc(8);
//     uint32_t  *c  =  kmalloc(8);
    
//     *a  =  10;
//     *b  =  20;
//     *c  =  *a  +  *b;
    
//      printk("value = %d", *c);
//      kfree(c);
//      kfree(b);
//      kfree(a);
     
//      for (;;);
//      return  0xDEADBABA;
// }

//changes********************************************************************************************************
// #include  "system.h"
// #include  "descriptor_tables.h"
// #include  "timer.h"
// #include  "paging.h"

// int  main(void  *mboot_ptr) {
//     init_descriptor_tables();
//     init_paging();
    
//     asm  volatile ("sti");
//     uint32_t  *ptr  = (uint32_t*)0xA0000000;
//     uint32_t  do_page_fault  =  *ptr;
//     printk("value = %d", do_page_fault);
    
//     for (;;);
//     return  0xDEADBABA;
// }