// idt.c -- structs that define an IDT entry and the IDT itself
//       -- funtions for initializing and inserting an entry into the IDT

#include "common.h"
#include "idt.h"

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

void init_idt()
{
    /* Set the first 32 entries in the IDT to the first 32 ISRs.  We set the
     * access flags to 0x8E.  This means the entry bit is present, is running in
     * ring 0 (kernel mode).  The value of 0x08 for the selector refers to the
     * kernel's Code Segment in the GDT. */
    idt_set_gate(0,  (unsigned int)isr0,  0x08, 0x8E);
    idt_set_gate(1,  (unsigned int)isr1,  0x08, 0x8E);
    idt_set_gate(2,  (unsigned int)isr2,  0x08, 0x8E);
    idt_set_gate(3,  (unsigned int)isr3,  0x08, 0x8E);
    idt_set_gate(4,  (unsigned int)isr4,  0x08, 0x8E);
    idt_set_gate(5,  (unsigned int)isr5,  0x08, 0x8E);
    idt_set_gate(6,  (unsigned int)isr6,  0x08, 0x8E);
    idt_set_gate(7,  (unsigned int)isr7,  0x08, 0x8E);
    idt_set_gate(8,  (unsigned int)isr8,  0x08, 0x8E);
    idt_set_gate(9,  (unsigned int)isr9,  0x08, 0x8E);
    idt_set_gate(10, (unsigned int)isr10, 0x08, 0x8E);
    idt_set_gate(11, (unsigned int)isr11, 0x08, 0x8E);
    idt_set_gate(12, (unsigned int)isr12, 0x08, 0x8E);
    idt_set_gate(13, (unsigned int)isr13, 0x08, 0x8E);
    idt_set_gate(14, (unsigned int)isr14, 0x08, 0x8E);
    idt_set_gate(15, (unsigned int)isr15, 0x08, 0x8E);
    idt_set_gate(16, (unsigned int)isr16, 0x08, 0x8E);
    idt_set_gate(17, (unsigned int)isr17, 0x08, 0x8E);
    idt_set_gate(18, (unsigned int)isr18, 0x08, 0x8E);
    idt_set_gate(19, (unsigned int)isr19, 0x08, 0x8E);
    idt_set_gate(20, (unsigned int)isr20, 0x08, 0x8E);
    idt_set_gate(21, (unsigned int)isr21, 0x08, 0x8E);
    idt_set_gate(22, (unsigned int)isr22, 0x08, 0x8E);
    idt_set_gate(23, (unsigned int)isr23, 0x08, 0x8E);
    idt_set_gate(24, (unsigned int)isr24, 0x08, 0x8E);
    idt_set_gate(25, (unsigned int)isr25, 0x08, 0x8E);
    idt_set_gate(26, (unsigned int)isr26, 0x08, 0x8E);
    idt_set_gate(27, (unsigned int)isr27, 0x08, 0x8E);
    idt_set_gate(28, (unsigned int)isr28, 0x08, 0x8E);
    idt_set_gate(29, (unsigned int)isr29, 0x08, 0x8E);
    idt_set_gate(30, (unsigned int)isr30, 0x08, 0x8E);
    idt_set_gate(31, (unsigned int)isr31, 0x08, 0x8E);

    /* Remap the IRQ table */
    //outb(0x20, 0x11);
    //outb(0xA0, 0x11);
    //outb(0x21, 0x20);
    //outb(0xA1, 0x28);
    //outb(0x21, 0x04);
    //outb(0xA1, 0x02);
    //outb(0x21, 0x01);
    //outb(0xA1, 0x01);
    //outb(0x21, 0x0);
    //outb(0xA1, 0x0);
}
