#include <system.h>

/* These define our textpointer, our background and foreground colors
 * (attributes), and x and y cursor coordinates */

/* We write words to the VGA buffer located at position 0xB8000 in physical
 * memory.  The buffer is of datatype `short'.  Each word in the text memory
 * buffer can be broken up into an `upper' 8 bits and a `lower' 8 bits, like
 * so:
 *
 *  15______12_11________8_7______________________0
 * |Backcolor | Forecolor |       Character       |
 *  ----------------------------------------------
 */
unsigned short *textmemptr;
int attrib = 0x0F;  /* white foreground, black background */
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
         * buffer by a line. `offset' will be how far we are off the screen.
         * e.g. if row == 25, then we are 25-25+1 = 1 row off the screen.
         */
        offset = csr_row - 25 + 1;

        /* (offset*80) is how many column positions you are off the screen.
         *
         * That is, if you are 3 rows off the screen, then (offset*80) = (3*80)
         * = 240 are three rows worth of columns.  textmemptr+(offset*80) will
         * point to the beginning of the offset'th row.  
         *
         * (25-offset)*80 is the count of how many words it will take to write
         * the amount of lines we need to move up */
        memcpyw(textmemptr, textmemptr+(offset*80), (25-offset)*80);
    }
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

    /* This sends a command to indicies 14 and 15 in the CTR Control Register
     * of the VGA controller.  These are the high and low bytes of the index
     * that show where the hardware cursor is to be `blinking'.  To learn more,
     * look at some VGA specific programming documents.  A great start to
     * graphics:
     * http://www.brackeen.com/home/vga */
    outportb(0x3D4, 14);
    outportb(0x3D5, offset >> 8);
    outportb(0x3D4, 15);
    outportb(0x3D5, offset);
}
