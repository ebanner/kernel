#include <system.h>

/* These define our textpointer, our background and foreground colors
 * (attributes), and x and y cursor coordinates */

unsigned short *textmemptr;
int attrib = 0x0F;
int csr_row, csr_col = 0;

/* Scrolls the screen */
void scroll(void)
{
    unsigned blank, temp;
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
         * buffer by a line */
        temp = csr_row - 25 + 1;
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
    for (r = 0; r < 25; i++)
        memsetw(textmemptr+(r*80), blank, 80);

    /* This sends a command to indicies 14 and 15 in the CTR Control Register
     * of the VGA controller.  These are the high and low bytes of the index
     * that show where the hardware cursor is to be `blinking'.  To learn more,
     * look at some VGA specific programming documents.  A great start to
     * graphics:
     * http://www.brackeen.com/home/vga */
    outportb(0x3D4, 14);
    outportb(0x3D5, temp >> 8);
    outportb(0x3D4, 15);
    outportb(0x3D5, temp);
}
