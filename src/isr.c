// isr.c -- provides a function for handling ISRs

#include "isr.h"
#include "screen.h"
#include "common.h"

/* Every ISR will call this function after an exception has occurred.  This
 * provides an explicit way of telling which interrupt happened.  For now, we
 * print the exception message and enter an inifinte loop.  Keep in mind that
 * interrupts have been disabled (cli opcode). */
void isr_handler(struct registers regs)
{
    /* Print the interrupt number */
    puts("Interrupt number: ");
    puthex(regs.int_no);
    putch('\n');
    switch (regs.err_code) {
        case 8: 
        case 10: 
        case 11: 
        case 12: 
        case 13: 
        case 14:
            puts("Error Code: ");
            puthex(regs.err_code);
            putch('\n');
    }
}

/* Handlers an IRQ.  This is a little different from our ISR handler because
 * we have to tell the PIC when we're done handling the interrupt and that it
 * can send us more interrupts. */
void irq_handler(struct registers regs)
{
    /* In addition to sending an EOI (end of interrupt) to the master PIC, we
     * need to send an EOI to the slave PIC if the IRQ is greater than or
     * equal to 40 */
    if (regs.int_no >= 40)
        outb(SLAVE_PIC, EOI);

    outb(MASTER_PIC, EOI); /* send an EOI to the master PIC regardless */

    /* custom handle the interrupt if we've defined a function to do so */
    if (interrupt_handlers[regs.int_no] != 0) {
        /* get the handler function for this interrupt */
        isr_t handler = interrupt_handlers[regs.int_no];
        /* call the function and give it the snapshot of the stack */
        handler(regs);
    }
}

void register_interrupt_handler(unsigned char n, isr_t handler)
{
    /* define the function to handle the n'th entry in the IDT */
    interrupt_handlers[n] = handler;
}
