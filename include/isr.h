// isr.h -- defines a struct that captures the stack at the time an ISR or IRQ
// interrupt handler function is entered.

#ifndef ISR_H
#define ISR_H

#define MASTER_PIC 0x20
#define SLAVE_PIC  0xA0
#define EOI        0x20

/* allow us to refer to each IRQ by its position in the IDT */
#define IRQ0  32
#define IRQ1  33
#define IRQ2  34
#define IRQ3  35
#define IRQ4  36
#define IRQ5  37
#define IRQ6  38
#define IRQ7  39
#define IRQ8  40
#define IRQ9  41
#define IRQ10 42
#define IRQ11 43
#define IRQ12 44
#define IRQ13 45
#define IRQ14 46
#define IRQ15 47

/* This is a snapshot of the stack when an IRQ or ISR handler function is
 * entered. */
struct registers {
    unsigned int ds;                                      /* Data Segment selector */
    unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;  /* pushed by pusha */
    unsigned int int_no, err_code;                        /* Interrupt number and error code (0 if not error code is returned */
    unsigned int eip, cs, elags, useresp, ss;             /* pushed by processor automatically */
};

/* Define `isr_t' to be a pointer to a function that returns `void' and
 * takes an `int' as a parameter. */
typedef void (*isr_t)(struct registers);
/* a function that calls execute the interrupt handler function of the n'th
 * interrupt in the IDT */
extern void register_interrupt_handler(unsigned char n, isr_t handler);

#endif
