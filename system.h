#ifndef SYSTEM_H
#define SYSTEM_H

#include "type.h"

#define array_size(x)	(sizeof((x)) / sizeof((x)[0]))

#define panic(s, ...)	_panic("%s:%u in %s: " s, __FILE__, __LINE__, __func__, ##__VA_ARGS__)

void outb(uint16_t port, uint8_t value);
uint8_t inb(uint16_t port);
uint16_t inw(uint16_t port);

void	_panic(const char *fmt, ...);

void *memset(void *b, int c, size_t len);   
int strlen(char *src);

void printk (const char *fmt, ...);

#endif /* SYSTEM_H */
