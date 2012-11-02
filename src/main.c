// main.c -- initializes and populates the GDT and IDT
//        -- tests for various functionaliy like catching interrupts
//        -- jumps into an endless loop

#include "gdt.h"
#include "idt.h"
#include "screen.h"
#include "timer.h"

/* function called from `boot.asm' */
int main()
{
    /* clear the screen and set up a pointer to the VGA buffer */
    init_video();

    /* set up the Global Descriptor Table */
    gdt_install();

    /* set up the Interrupt Descriptor Table */
    idt_install();

    /* populate the IDT with 32 ISRs */
    init_idt();

    /* print some stuff to the screen */
    puts("The quick brown fox jumped over the lazy brown dog. ");
    putint(-42);
    puts(" ");
    puthex(0xEDDE);
    putch('\n');

    /* will cause interrupts 3 and 4 */
    __asm__ __volatile__ ("int $0x3");
    __asm__ __volatile__ ("int $0x4");

    /* for some reason, we have to enable interrupts */
    __asm__ __volatile__ ("sti");
    /* initialize the timer to give an IRQ every second */
    init_timer(50);

    /* jump into an endless loop */
    for (;;);
}
