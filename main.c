#include "system.h"
#include "descriptor_tables.h"
#include "timer.h"

int main(struct multiboot *mboot_ptr)
{
    printk("TinyOS\n");
    printk("Starting the kernel...\n");

    printk("- descriptor tables init...\t");
	init_descriptor_tables();
	printk(" [OK]\n");


    init_timer(50);

    asm volatile("sti");

    for(;;);

    return 0xDEADBABA;
}
