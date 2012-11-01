// screen.h -- functions that set up the screen and allow for printing.

#ifndef SCREEN_H
#define SCREEN_H

extern void putch(char c);
extern void puts(char *text);
extern void putint(int n);
extern void puthex(int h);
extern void print_hexdigit(int d);
extern void init_video(void);

#endif
