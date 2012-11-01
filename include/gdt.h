#ifndef GDT_H
#define GDT_H

extern void gdt_install();

/* Defines a GDT entry.  We use the attribute `packed' to tell gcc not to
 * change any of the alignment in the structure. */
struct gdt_entry {
    unsigned short limit_low;
    unsigned short base_low;
    unsigned char base_middle;
    unsigned char access;
    unsigned char granularity;
    unsigned char base_high;
} __attribute__((packed));

/* Special pointer which includes the limit [(max bytes taken up by GDT)-1] */
struct gdt_ptr
{
    unsigned short limit;
    unsigned int base;
} __attribute__((packed));

/* Our GDT, with 3 entries, and finally our special GDT pointer */
struct gdt_entry gdt[5];
struct gdt_ptr gp;

#endif
