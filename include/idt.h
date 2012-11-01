#ifndef IDT_H
#define IDT_H

/* functions for initializing the IDT */
extern void idt_install();
extern void init_idt();
extern void idt_set_gate(unsigned char num, unsigned int base, unsigned short sel, unsigned char flags);


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


/* ISRs */
extern void isr0(void);
extern void isr1(void);
extern void isr2(void);
extern void isr3(void);
extern void isr4(void);
extern void isr5(void);
extern void isr6(void);
extern void isr7(void);
extern void isr8(void);
extern void isr9(void);
extern void isr10(void);
extern void isr11(void);
extern void isr12(void);
extern void isr13(void);
extern void isr14(void);
extern void isr15(void);
extern void isr16(void);
extern void isr17(void);
extern void isr18(void);
extern void isr19(void);
extern void isr20(void);
extern void isr21(void);
extern void isr22(void);
extern void isr23(void);
extern void isr24(void);
extern void isr25(void);
extern void isr26(void);
extern void isr27(void);
extern void isr28(void);
extern void isr29(void);
extern void isr30(void);
extern void isr31(void);

/* IRQs */
extern void irq0(void);
extern void irq1(void);
extern void irq2(void);
extern void irq3(void);
extern void irq4(void);
extern void irq5(void);
extern void irq6(void);
extern void irq7(void);
extern void irq8(void);
extern void irq9(void);
extern void irq10(void);
extern void irq11(void);
extern void irq12(void);
extern void irq13(void);
extern void irq14(void);
extern void irq15(void);

#endif
