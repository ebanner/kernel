// common.h -- declares some functions so generic, we had to put them in a
// header file called `common.c'

#ifndef COMMON_H
#define COMMON_H

// I/O bus port stuff
extern void outb(unsigned short port, unsigned char value);
extern unsigned char inb(unsigned short port);
extern unsigned short inw(unsigned short port);

// necessary stuff
extern unsigned char *memcpy(unsigned char *dest, const unsigned char *src, int count);
extern unsigned char *memset(unsigned char *dest, unsigned char val, int count);
extern unsigned short *memcpyw(unsigned short *dest, const unsigned short *src, int count);
extern unsigned short *memsetw(unsigned short *dest, unsigned short val, int count);
extern int strlen(const char *str);

#endif
