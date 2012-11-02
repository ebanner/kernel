// timer.c -- initializes the PIT and handles clock updates

#include "timer.h"
#include "isr.h"
#include "screen.h"
#include "common.h"

unsigned int tick = 0;

/* handler function for IRQ0 */
static void timer_callback(struct registers regs)
{
    tick++;
    puts("Tick: ");
    putint(tick);
    putch('\n');
}

void init_timer(unsigned int frequency)
{
    /* add our handler function for IRQ0 to our list of handler functions */
    register_interrupt_handler(IRQ0, timer_callback);

    /* We need to give the pit some value at which to divide its 1,193,180 Hz
     * oscillator by.  If we give it the full 1,193,180 Hz, then we will get 1
     * tick per second. */
    unsigned int divisor = 1193180 / frequency;

    /* Send the command byte to the PIT's command register port.
     * 0x36 refers to repeating mode, so that when the divisor gets to 0, it
     * automatically refreshes to its starting value. */
    outb(0x43, 0x36);

    /* divisor has to be sent bit-wise, so split up the upper and lower 8 bits
     * */
    unsigned char low  = (divisor & 0xFF);
    unsigned char high = ((divisor >> 8) & 0xFF);

    /* send the frequency divisor to the PIT's Channel 0 port */
    outb(0x40, low);
    outb(0x40, high);
}
