#include "type.h"
#include "system.h"
#include "screen.h"
#include "vsprintf.h"

#include <stdarg.h>

// Write a byte out to the specified port.
void outb(uint16_t port, uint8_t value)
{
    asm volatile ("outb %1, %0" : : "dN" (port), "a" (value));
}

uint8_t inb(uint16_t port)
{
    uint8_t ret;
    asm volatile("inb %1, %0" : "=a" (ret) : "dN" (port));
    return ret;
}

uint16_t inw(uint16_t port)
{
    uint16_t ret;
    asm volatile ("inw %1, %0" : "=a" (ret) : "dN" (port));
    return ret;
}

void *
memset(void *b, int c, size_t len)
{
	char *bb;

	for (bb = b; len > 0; len--)
		*bb++ = c;

	return (b);
}

int strlen(char *src)
{
  int i = 0;
  while (*src++)
    i++;
  return i;
}

void printk (const char *fmt, ...)
{
	static char buf [1024];

 	va_list args;
 	int i;
 
 	va_start(args, fmt);
 	i = vsprintf(buf,fmt,args);
 	va_end(args);

 	buf[i] = '\0';

 	screen_write (buf);
}

void _panic(const char *fmt, ...)
{
    static char buf[1024];
    va_list args;

    // Print the panic header
    screen_write("\nKernel panic!\n--------------------------\n");

    // Format the custom panic message
    va_start(args, fmt);
    vsprintf(buf, fmt, args);
    va_end(args);

    // Write the formatted message
    screen_write(buf);

    // Print the footer
    screen_write("\n--------------------------\n");

    // Halt execution indefinitely
    for (;;);
}