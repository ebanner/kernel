// idt.c -- structs that define an IDT entry and the IDT itself
//       -- funtions for initializing and inserting an entry into the IDT

#include "common.h"
#include "idt.h"
#include "screen.h"

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

    /* Remap the IRQ table.
     *
     * Currently, the Master and Slave PICs send IRQ values of 0x08-0x0F and
     * 0x80-0x87 to the CPU currently.  We still send IRQs to the PICs (there
     * are master and slave PICs) with these values, but we need the PICs to
     * be able to map these numbers to another range in order to avoid
     * confusing these IRQ values with the Exception values (currently we're
     * using the first 31 (0x00-0x1F) values in the IDT for exceptions.
     *
     * We send the value of INIT (0x11) to the master PIC because it alerts
     * the master PIC that the next 3 values we send to its data port will be:
     *     1. Its new vector offset (instead of passing along the IRQ number
     *        that we send it TO THE PROCESSOR, it will send the sum of the
     *        IRQ number and the offset.
     *     2. How it is wired to masters/slaves (tells the master that there
     *        is a slave at IRQ n, and tells the slave its cascade identity).
     *     3. Additional information about the environment (?)
     */
    outb(MASTER_CMD, INIT);
    outb(SLAVE_CMD,  INIT);

    /* offset each of the vectors by 0x20.
     * so now instead of 0x08, 0x09, 0x10, ..., 0x0F, they'll be
     *                   0x20, 0x21, 0x22, ..., 0x27. 
     * now the ISRs the PIC sends won't conflict with our 31 ISRs already in
     * the IDT. */
    outb(MASTER_DATA, 0x20); 
    outb(SLAVE_DATA,  0x28);  /* offset the slave IRQ values by 0x28 */

    /* tell the master PIC that there is a slave PIC at IRQ2 (0000 0100) */
    outb(MASTER_DATA, 0x04);
    /* tell the slave PIC its cascade identity is 2 (0000 0010) */
    outb(SLAVE_DATA,  0x02);

    /* master's environment is 8086/88 (MCS-80/85) mode */
    outb(MASTER_DATA, 0x01);
    /* slaves's environment is 8086/88 (MCS-80/85) mode */
    outb(SLAVE_DATA,  0x01);

    /* clear out the data ports for both the master and slave */
    outb(MASTER_DATA, 0x00);
    outb(SLAVE_DATA,  0x00);

    puts("Setting the IRQs...\n");
    /* Set the next 16 entries in the IDT to be the IRQs */
    idt_set_gate(32, (unsigned int)irq0,  0x08, 0x8E);
    idt_set_gate(33, (unsigned int)irq1,  0x08, 0x8E);
    idt_set_gate(34, (unsigned int)irq2,  0x08, 0x8E);
    idt_set_gate(35, (unsigned int)irq3,  0x08, 0x8E);
    idt_set_gate(36, (unsigned int)irq4,  0x08, 0x8E);
    idt_set_gate(37, (unsigned int)irq5,  0x08, 0x8E);
    idt_set_gate(38, (unsigned int)irq6,  0x08, 0x8E);
    idt_set_gate(39, (unsigned int)irq7,  0x08, 0x8E);
    idt_set_gate(40, (unsigned int)irq8,  0x08, 0x8E);
    idt_set_gate(41, (unsigned int)irq9,  0x08, 0x8E);
    idt_set_gate(42, (unsigned int)irq10, 0x08, 0x8E);
    idt_set_gate(43, (unsigned int)irq11, 0x08, 0x8E);
    idt_set_gate(44, (unsigned int)irq12, 0x08, 0x8E);
    idt_set_gate(45, (unsigned int)irq13, 0x08, 0x8E);
    idt_set_gate(46, (unsigned int)irq14, 0x08, 0x8E);
    idt_set_gate(47, (unsigned int)irq15, 0x08, 0x8E);
}
