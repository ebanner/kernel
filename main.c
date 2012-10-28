#include <system.h>

/* Copies `count' bytes of `src' to `dest'. */
unsigned char *memcpy(unsigned char *dest, const unsigned char *src, int count)
{
    int i;
    
    for (i = 0; i < count; i++)
        dest[i] = src[i];

    return dest;
}

/* Sets the first `count' bytes of `dest' to `val'. */
unsigned char *memset(unsigned char *dest, unsigned char val, int count)
{
    int i;

    for (i = 0; i < count; i++)
        dest[i] = val;

    return dest;
}

/* Copies `count' words from `src' to `dest'. */
unsigned short *memcpyw(unsigned short *dest, const unsigned short *src, int count)
{
    int i;
    
    for (i = 0; i < count; i++)
        dest[i] = src[i];

    return dest;
}

/* Sets the first `count' words of `dest' to `val'. */
unsigned short *memsetw(unsigned short *dest, unsigned short val, int count)
{
    int i;

    for (i = 0; i < count; i++)
        dest[i] = val;

    return dest;
}

/* Returns the length of a string. */
int strlen(const char *str)
{
    int i;

    for (i = 0; str[i] != '\0'; i++)
        ;

    return i;
}

/* Receives a byte from an I/O location.  We will use this later on for
 * reading from the I/O ports to get data form devices such as the keyboard.
 * */
unsigned char inportb(unsigned short _port)
{
    unsigned char rv;

    __asm__ __volatile__ ("inb %1, %0" 
            : "=a" (rv) 
            : "Nd" (_port));

    return rv;
}

/* Sends a byte on an I/O location.  This will be used for changing the
 * textmode cursor position.  The `a' modifier enforces `_data' to be placed
 * in the eax register before the asm command is issued and `Nd' allows for
 * one-byte constant values to be assembled as constants, freeing the edx
 * register for other cases. */
void outportb(unsigned short _port, unsigned char _data)
{
    __asm__ __volatile__ ("outb %1, %0" 
            : /* no output registers */
            : "Nd" (_port), "a" (_data)
            ); 
}

/* start.asm hands over control to this function immediately after setting esp
 * to point to our 8K stack. */
int main()
{
    /* set up the Global Descriptor Table */
    gdt_install();

    /* set up the Interrupt Descriptor Table */
    idt_install();
    /* populate the IDT with 32 ISRs */
    idt_init();

    /* clear the screen and set up a pointer to the VGA buffer */
    init_video();

    /* print some stuff to the screen */
    puts((unsigned char*)"The quick brown fox jumped over the lazy brown dog. ");
    putint(-42);
    puts((unsigned char *)" ");
    puthex(0xEDDE);
    putch('\n');
    putint(5/0);

    /* Testing to see if the `Divide by zero' exception is caught correctly by
     * the IDT.  Currently there is no entry in the IDT for dividing by zero,
     * so the machine resets endlessly.  */
    //puts((unsigned char *)"\nDividing by zero... ");
    //putint(5/0);

    /* jump into an endless loop */
    for (;;);
}
