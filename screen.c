#include <system.h>

#define WHITE_ON_BLACK 0x0F
#define BS  0x08
#define TAB 0x09
#define CR  0x0D
#define NL  0x0A

void puthex(int h);
void print_hexdigit(int d);

/* These define our textpointer, our background and foreground colors
 * (attributes), and x and y cursor coordinates */

/* We write words to the VGA buffer located at position 0xB8000 in physical
 * memory.  The buffer is of datatype `short'.  Each word in the text memory
 * buffer can be broken up into an `upper' 8 bits and a `lower' 8 bits, like
 * so:
 *
 *  15------12-11--------8-7---------------------0
 * |Backcolor | Forecolor |       Character       |
 *  ----------------------------------------------
 */
unsigned short *textmemptr;
int attrib = 0x0D;  /* white foreground, black background */
int csr_row, csr_col = 0;

/* Scrolls the screen */
void scroll(void)
{
    unsigned blank, offset;
    /* A blank is defined as a space... we need to give it backcolor, too */
    blank = 0x20 | (attrib << 8);
    /* Here is the operation that just happened:
     *
     *   0x20             0000 0000 0010 0000
     * |                |
     *   attrib << 8      0000 1111 0000 0000
     * _____________    _____________________
     *   0x0F20           0000 1111 0010 0000
     *
     */

    /* Row 25 is the end, this means we need to scroll up */
    if (csr_row >= 25)
    {
        /* Move the current text chunk that makes up the screen back in the
         * buffer by a line. 
         * `offset' will be will tell us how many rows we are off the bottom of
         * the screen.  
         * e.g. if row == 25, then we are 25-25+1 = 1 row off the screen.
         */
        offset = csr_row - 25 + 1;

        /* (offset*80) is how many column positions you are off the screen.
         *
         * That is, if you are 3 rows off the screen, then (offset*80) = (3*80)
         * = 240 are three rows worth of columns.  
         * textmemptr+(offset*80) will point to the beginning of the offset'th
         * row.  
         *
         * (25-offset)*80 is the count of how many words it will take to write
         * the amount of lines we need to move up.
         * So, starting at the offset'th line, copy, to the zero'th row, the
         * next (25-offset) lines.
         *
         * Wait, but that doesn't really make sense.  I would think that no
         * matter where you were (off the end of the screen), you should print
         * the next 24 lines no matter what.  Let's try that. */
        //memcpyw(textmemptr, textmemptr+(offset*80), (25-offset)*80);
        memcpyw(textmemptr, textmemptr+(offset*80), 24*80);

        /* Finally, we set the chunk of memory that occupies the last line of
         * text to our `blank' character
         * Again, `offset' refers to how many rows we are off of the screen.
         * Wait, this seems wrong, too.  Let's blank out the 24th line no
         * matter what... we don't want to blank out the 23rd line or even the
         * 22nd line... */
        //memsetw(textmemptr+(25-offset)*80, blank, 80);
        memsetw(textmemptr+(24*80), blank, 80);
    }
}

/* Updates the hardware cursor: the little blinking line on the screen under
 * the last character pressed */
void move_csr(void)
{
    unsigned offset;

    /* The equation for finding the index in a linear chunk of memory can be
     * represented by:
     *  index = [(row*width) + col].
     * That makes sense... to find the 3rd row and the 50th column in that row,
     * you would need to... well let's step back.  How would we get to the 2nd
     * row?  We would start at textmemptr and add 80 to it.  This would
     * bring us to the 2nd row.  Then we add 80 again to reach the next row.
     * So, to get to the n'th row, do [(textmemptr+n)*80].  Then to get to a
     * specific column in that row, just add the column you want. */
    offset = (csr_row*80) + csr_col;

    /* This sends a command to indicies 14 and 15 in the CTR Control Register
     * of the VGA controller.  These are the high and low bytes of the index
     * that show where the hardware cursor is to be `blinking'.  To learn more,
     * look at some VGA specific programming documents.  A great start to
     * graphics: http://www.brackeen.com/home/vga */
    outportb(0x3D4, 14);
    outportb(0x3D5, offset >> 8);
    outportb(0x3D4, 15);
    outportb(0x3D5, offset);
}

void cls()
{
    unsigned blank;
    int r; /* row */

    /* Again, we need the `short' that will be used to represent a space with
     * color */
    blank = 0x20 | (attrib << 8);

    /* Sets the entire screen to spaces in our current color */
    /* For every row, write a blank to every column in that row.  Since there
     * are 80 columns in a row, we write a blank to each of them */
    for (r = 0; r < 25; r++)
        memsetw(textmemptr+(r*80), blank, 80);

    /* Update our virtual cursor, and then move the hardware cursor */
    csr_row = 0;
    csr_col = 0;
    move_csr();
}

void putch(unsigned char c)
{
    unsigned short *where;
    unsigned att = attrib << 8;  // black BG, white FG

    /* Handle a backspace, by moving the cursor back one space */
    if (c == BS && csr_col != 0) {
        csr_col--;
    } 
    /* Handles a tab by incrementing the cursor's x, but only to a point that
     * will make it divisible by 8 */
    else if (c == TAB) {
        /* This coerces csr_col to be a multiple of 8 by forcing the bits to
         * the right of the 3rd least significant bit (zero-indexed) to be 0.
         * All of the bits to the left of the 3rd least significant bit
         * (including the 3rd LSB itself) will be 1 and will have no effect on
         * the rest of csr_col */
        csr_col = (csr_col+8) & ~(8-1);
    }
    /* Handles a `Carraige return', which simply brings the cursor back to the
     * margin */
    else if (c == CR) {
        csr_col = 0;
    }
    /* We handle our newlines the way DOS and the BIOS do--we treat it as if a
     * `CR' was also there, so we bring the cursor to the margin and we
     * increment the `y' value */
    else if (c == NL) {
        csr_row++;
        csr_col = 0;
    }
    /* Any character greater than and including a space is a printable
     * character.  The equation for finding the index in a linear chunk of
     * memory can be represented by:
     *  index = (csr_row*80) + csr_col */
    else if (' ' <= c) {
        where = textmemptr + (csr_row*80) + csr_col;
        *where = att | c;  /* color AND character */
        csr_col++;
    }

    /* If the cursor has reached the edge of the screen's width, we insert a
     * new line in there */
    if (csr_col >= 80) {
        csr_row++;
        csr_col = 0;
    }

    /* Scroll the screen if needed and finally move the cursor */
    scroll();
    move_csr();
}

/* Uses the above routine to output a string... */
void puts(unsigned char *text)
{
    int i;

    for (i = 0; i < strlen((char *)text); i++) {
        putch(text[i]);
    }
}

/* Print an integer to the screen.  This will fail on the largest negative
 * integer. */
void putint(int n) {
    if (n < 0) {
        putch('-');
        n = -n;
    }
    int ones = n % 10;
    if (n / 10 == 0) {
        /* n%10 will get us the digit in the ONEs place.  Adding 48 to that
         * will get us the ASCII code for that digit.  Integer division by 10
         * will chop off the ONEs place */
        putch(ones+48);
    } else {
        putint(n / 10);
        putch(ones+48);
    }
}

/* Prints a hexidecmal integer to the screen in base 16. */
void puthex(int h)
{
    if (h/16 == 0) {
        putch('0');
        putch('x');
        print_hexdigit(h%16);
    } else {
        puthex(h/16);
        print_hexdigit(h%16);
    }
}

/* Returns the appropriate hex digit for any 0 < digit < 15 */
void print_hexdigit(int d) {
    switch (d) {
        case 10:
            putch('A');
            break;
        case 11:
            putch('B');
            break;
        case 12:
            putch('C');
            break;
        case 13:
            putch('D');
            break;
        case 14:
            putch('E');
            break;
        case 15:
            putch('F');
            break;
        default:
            putch(d+48);
            break;
    }
}

/* Sets the forecolor and backcolor that we will use.
 * Again, remember that the four high bits in a word correspond to the
 * background color and the 4 low bits correspond to the foreground color */
void settextcolor(unsigned char forecolor, unsigned char backcolor)
{
    attrib = (backcolor << 4) | (forecolor & 0x0F);
}

/* Sets our text-mode VGA pointer, then clears the screen for us */
void init_video(void)
{
    textmemptr = (unsigned short *)0xB8000;
    cls();
}
