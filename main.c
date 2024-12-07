#include "system.h"
#include "descriptor_tables.h"
#include "paging.h"
#include "heap.h"
#include "kmalloc.h"

int main(void *mboot_ptr)
{
	init_descriptor_tables();
	init_paging();


    uint32_t* a = kmalloc(8);

uint32_t* b = kmalloc(8);
uint32_t* c = kmalloc(8);

*a = 100;
*b = 200;

*c = *a + *b;

printk("VAL = %d\n", *c);


kfree(c);
kfree(b);
uint32_t* d = kmalloc(12);


    for(;;);

    return 0xDEADBABA;
}
