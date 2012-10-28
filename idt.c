#include <system.h>

struct idt_entry {
    unsigned short base_lo;
    unsigned short sel;         /* Our kernel segment always goes here! */
    unsigned char zero;         /* This will ALWAYS be set to 0! */
    unsigned char flags;        /* Set using the above table! */
    unsigned short base_hi;
} __attribute__((packed));

struct idt_ptr {
    unsigned short limit;
    unsigned int base;
} __attribute__((packed));

/* Declare an IDT of 256 entries.  Although we will only use the first 32
 * entries in this tutorial, the rest exists as a bit of a trap.  If any
 * unidentified IDT entry is hit, it normally will cuase an ``Unhandled
 * Interrupt" exception.  Any descriptor for which the `presence' bit is cleard
 * (0) will generate an ``Unhandled Interrupt" exception */
struct idt_entry idt[256];
struct idt_ptr idtp;

/* This exists in `start.asm', and is used to load our IDT */
extern void idt_load();

/* Use this function to set an entry in the IDT. */
void idt_set_gate(unsigned char num, unsigned int base, unsigned short sel, unsigned char flags)
{
    /* Take the arguments `base' and split it up into a high and low 16-bits,
     * storing them in idt[num].base_hi and base_lo. */
    idt[num].base_lo = (base & 0xFFFF);
    idt[num].sel = sel;
    idt[num].zero = 0;
    idt[num].flags = flags;
    idt[num].base_hi = (base >> 16) & 0xFFFF;
}

void idt_install()
{
    /* Sets the special IDT pointer up, just like in `gdt.c' */
    idtp.limit = (sizeof (struct idt_entry)*256 - 1);
    idtp.base = (unsigned int)idt;

    /* Clear out the entire IDT, initializing it to zeros */
    memset((unsigned char *)idt, 0, sizeof (struct idt_entry) * 256);

    /* Add any new ISRs to the IDT here using idt_set_gate */

    /* Points the processor's internal register to the new IDT */
    idt_load();
}
