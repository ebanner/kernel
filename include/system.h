#ifndef __SYSTEM_H
#define __SYSTEM_H

/* MAIN.C */

// stuff in main.c
extern unsigned char *memcpy(unsigned char *dest, const unsigned char *src, int count);
extern unsigned char *memset(unsigned char *dest, unsigned char val, int count);
extern unsigned short *memcpyw(unsigned short *dest, const unsigned short *src, int count);
extern unsigned short *memsetw(unsigned short *dest, unsigned short val, int count);
extern int strlen(const char *str);
extern unsigned char inportb(unsigned short _port);
extern void outportb(unsigned short _port, unsigned char _data);

// stuff in screen.c
extern void cls();
extern void putch(unsigned char c);
extern void putint(int c);
extern void puthex(int h);
extern void puts(unsigned char *text);
extern void settextcolor(unsigned char forecolor, unsigned char backcolor);
extern void init_video();

// gdt stuff
extern void gdt_install();

// idt stuff
extern void idt_install();
void idt_set_gate(unsigned char num, unsigned long base, unsigned short sel, unsigned char flags);

#endif
