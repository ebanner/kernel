#ifndef IDT_H
#define IDT_H

extern void idt_install();
extern void idt_init();
extern void idt_set_gate(unsigned char num, unsigned int base, unsigned short sel, unsigned char flags);

// this is a snapshot of the stack when an IRQ or ISR handler function is
// entered
struct registers {
    unsigned int ds;                                      /* Data Segment selector */
    unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;  /* pushed by pusha */
    unsigned int int_no, err_code;                        /* Interrupt number and error code (0 if not error code is returned */
    unsigned int eip, cs, elags, useresp, ss;             /* pushed by processor automatically */
};

#endif
