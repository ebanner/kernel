#include "gdt.h"
#include "idt.h"
#include "screen.h"

// main.c -- initializes and populates the GDT and IDT
//        -- tests for various functionaliy like catching interrupts
//        -- jumps into an endless loop

/* function called from `boot.asm' */
int main()
{
    /* set up the Global Descriptor Table */
    gdt_install();

    /* set up the Interrupt Descriptor Table */
    idt_install();
    /* populate the IDT with 32 ISRs */
    init_idt();

    /* clear the screen and set up a pointer to the VGA buffer */
    init_video();

    /* print some stuff to the screen */
    puts("The quick brown fox jumped over the lazy brown dog. ");
    putint(-42);
    puts(" ");
    puthex(0xEDDE);
    putch('\n');

    /* will cause interrupts 3 and 4 */
    __asm__ __volatile__ ("int $0x3");
    __asm__ __volatile__ ("int $0x4");

    /* Testing to see if the `Divide by zero' exception is caught correctly by
     * the IDT.  Currently there is no entry in the IDT for dividing by zero,
     * so the machine resets endlessly.  */
    //puts((unsigned char *)"\nDividing by zero... ");
    //putint(5/0);

    /* jump into an endless loop */
    for (;;);
}
