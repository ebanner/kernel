// common.c -- a bunch of common functions used throughout the kernel

/* Sends a byte on an I/O location.  This will be used for changing the
 * textmode cursor position.  The `a' modifier enforces `_data' to be placed
 * in the eax register before the asm command is issued and `Nd' allows for
 * one-byte constant values to be assembled as constants, freeing the edx
 * register for other cases. */
void outb(unsigned short port, unsigned char data)
{
    __asm__ __volatile__ ("outb %1, %0" 
            : /* no output registers */
            : "Nd" (port), "a" (data)
            ); 
}

/* Receives a byte from an I/O location.  We will use this later on for
 * reading from the I/O ports to get data form devices such as the keyboard.
 * */
unsigned char inb(unsigned short port)
{
    unsigned char rv;

    __asm__ __volatile__ ("inb %1, %0" 
            : "=a" (rv) 
            : "Nd" (port));

    return rv;
}

/* Not sure what this function does at the moment, but I will shortly! */
unsigned short inw(unsigned short port)
{
    unsigned short ret;
    __asm__ __volatile__ ("inw %1, %0" 
            : "=a" (ret) 
            : "dN" (port));
    return ret;
}


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
