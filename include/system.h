#ifndef __SYSTEM_H
#define __SYSTEM_H

/* MAIN.C */

// stuff in main.c
extern unsigned char *memcpy(unsigned char *dest, const unsigned char *src, int count);
extern unsigned char *memset(unsigned char *dest, unsigned char val, int count);
extern unsigned short *memcpyw(unsigned short *dest, const unsigned short *src, int count);
extern unsigned short *memsetw(unsigned short *dest, unsigned short val, int count);
extern int strlen(const char *str);
extern unsigned char inb(unsigned short _port);
extern void outb(unsigned short _port, unsigned char _data);

// stuff in screen.c
extern void cls();
extern void putch(unsigned char c);
extern void putint(int c);
extern void puthex(int h);
extern void puts(char *text);
extern void settextcolor(unsigned char forecolor, unsigned char backcolor);
extern void init_video();

// gdt stuff
extern void gdt_install();

// idt stuff
extern void idt_install();
void idt_set_gate(unsigned char num, unsigned int base, unsigned short sel, unsigned char flags);
/* Represents what the stack looks like when `fault_handle' gets called. */
struct registers {
    unsigned int ds;                                      /* Data Segment selector */
    unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;  /* pushed by pusha */
    unsigned int int_no, err_code;                        /* Interrupt number and error code (0 if not error code is returned */
    unsigned int eip, cs, elags, useresp, ss;             /* pushed by processor automatically */
};
extern void idt_init();

#endif
