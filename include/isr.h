// isr.h -- defines a struct that captures the stack at the time an ISR or IRQ
// interrupt handler function is entered

#ifndef ISR_H
#define ISR_H

/* This is a snapshot of the stack when an IRQ or ISR handler function is
 * entered. */
struct registers {
    unsigned int ds;                                      /* Data Segment selector */
    unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;  /* pushed by pusha */
    unsigned int int_no, err_code;                        /* Interrupt number and error code (0 if not error code is returned */
    unsigned int eip, cs, elags, useresp, ss;             /* pushed by processor automatically */
};

#endif
