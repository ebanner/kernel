#include "common.h"

// idt.c -- structs that define an IDT entry and the IDT itself
//       -- funtions for initializing and inserting an entry into the IDT

/* Defines an entry in the IDT */
struct idt_entry {
    unsigned short base_lo;
    unsigned short sel;         /* Our kernel segment always goes here! */
    unsigned char zero;         /* This will ALWAYS be set to 0! */
    unsigned char flags;        /* Set using the above table! */
    unsigned short base_hi;
} __attribute__((packed));

/* Defines a pointer to an array of interrupt handlers.  We package the limit
 * and base like this so we can hand this struct to `lidt'*/
struct idt_ptr {
    unsigned short limit;
    unsigned int base;
} __attribute__((packed));

/* Declare an IDT of 256 entries.  If any unidentified IDT entry is hit, it
 * normally will cuase an ``Unhandled Interrupt" exception.  Any descriptor
 * for which the `presence' bit is cleard (0) will generate an ``Unhandled
 * Interrupt" exception */
struct idt_entry idt[256];
struct idt_ptr idtp;

/* This exists in `start.asm', and is used to load our IDT */
extern void idt_load();

/* Use this function to set an entry in the IDT. */
void idt_set_gate(unsigned char num, unsigned int base, unsigned short sel, unsigned char flags)
{
    /* address of the ISR */
    idt[num].base_lo = (base & 0xFFFF);
    /* the offset to the kernel's code selector in the GDT */
    idt[num].sel = sel;
    /* always zero */
    idt[num].zero = 0;
    /* FLAGS:
     * Bits 3..0
     *   IDT gate type (0xE for 32-bit interrupt gate)
     * Bits 4
     *   Storage Segment (0 for interrupt gates)
     * Bits 6..5
     *   Descriptor privilege level (0 for kernel ring)
     * Bits 7
     *   Present bit -- can be set to 0 for unused interrupts or for paging
     */
    idt[num].flags = flags;
    /* high bits of the base address */
    idt[num].base_hi = (base >> 16) & 0xFFFF;
}

void idt_install()
{
    /* Sets the special IDT pointer up, just like in `gdt.c' */
    idtp.limit = (sizeof (struct idt_entry)*256 - 1);
    idtp.base = (unsigned int)idt;

    /* Clear out the entire IDT, initializing it to zeros */
    memset((unsigned char *)idt, 0, sizeof (struct idt_entry) * 256);

    /* Points the processor's internal register to the new IDT */
    idt_load();
}
