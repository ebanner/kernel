#include "isr.h"
#include "screen.h"

// isr.c -- provides a function for handling ISRs

/* Every ISR will call this function after an exception has occurred.  This
 * provides an explicit way of telling which interrupt happened.  For now, we
 * print the exception message and enter an inifinte loop.  Keep in mind that
 * interrupts have been disabled (cli opcode). */
void fault_handler(struct registers regs)
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
