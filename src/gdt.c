// gdt.c -- functions for setting up and inserting descriptors into the GDT

#include "gdt.h"

/* This will be a function in `boot.esm'.  We use this to properly reload the
 * new segment registers */
extern void gdt_flush();

/* Setup a descriptor in the Global Descriptor Table */
void gdt_set_gate(int num, unsigned int base, unsigned int limit, unsigned char access, unsigned char gran)
{
    /* Setup the descriptor base address */
    /* Low 16 bits of limit */
    gdt[num].limit_low = (limit & 0xFFFF);
    /* Low 16 bits of base */
    gdt[num].base_low = (base & 0xFFFF);
    /* Bits 16-23 of base */
    gdt[num].base_middle = (base >> 16) & 0xFF;
    /* All 8 access bits */
    gdt[num].access = access;
    /* Low 4 bytes of granularity are 16-19 bits of limit */
    gdt[num].granularity = ((limit >> 16) & 0x0F);
    /* High 4 bits of granularity are the high 4 bits of granularity passes in */
    gdt[num].granularity |= (gran & 0xF0);
    /* 24-31 bits of base */
    gdt[num].base_high = (base >> 24) & 0xFF;
}

/* Should be called by main.  This will setup the special GDT pointer, set up
 * the first 3 entries in our GDT, and then finally call `gdt_flush()' in our
 * assembler file in order to tell the processor where the new GDT is and
 * update the new segment registers. */
void gdt_install()
{
    /* Setup the GDT pointer and limit */
    gp.limit = ( sizeof(struct gdt_entry) * 5) - 1;  // zero-indexed--hence the need to subtract 1
    gp.base = (int)&gdt;

    /* Our NULL descriptor */
    gdt_set_gate(
            0,          /* NUM:    zeroth entry in the GDT */
            0,          /* BASE:   base address of 0 */
            0,          /* LIMIT:  spans no space in memory */
            0,          /* ACCESS: not present, kernel ring 0, non-executable, can only 
                           be executed from ring 0, READ ONLY, access bit is 0 */
            0           /* FLAGS: `limit' is in 1B blocks, 16 bit protected mode */
            );     

    /* Kernel Code Segment descriptor */
    gdt_set_gate(
            1,          /* NUM:    first entry in the GDT */
            0,          /* BASE:   base address of 0 */
            0xFFFFFFFF, /* LIMIT:  spans all of memory */
            0x9A,       /* ACCESS: present, kernel ring 0, executable, can only 
                            be executed from ring 0, READ ONLY, access bit is 0 */
            0xCF        /* FLAGS: `limit' is in 4K blocks, 32 bit protected mode */
            );

    /* Kernel Data Segment descriptor */
    gdt_set_gate(
            2,          /* NUM:    second entry in the GDT */
            0,          /* BASE:   base address of 0 */
            0xFFFFFFFF, /* LIMIT:  spans all of memory */
            0x92,       /* ACCESS: present, kernel ring 0, NOT executable, can only 
                            be read from ring 0, READ/WRITE, access bit is 0 */
            0xCF        /* FLAGS: `limit' is in 4K blocks, 32 bit protected mode */
            );

    /* User mode Code Segment descriptor */
    gdt_set_gate(
            3,          /* NUM:    first entry in the GDT */
            0,          /* BASE:   base address of 0 */
            0xFFFFFFFF, /* LIMIT:  spans all of memory */
            0xFA,       /* ACCESS: present, kernel ring 3, executable, can only 
                            be executed from ring 0, READ ONLY, access bit is 0 */
            0xCF        /* FLAGS: `limit' is in 4K blocks, 32 bit protected mode */
            );

    /* User mode Data Segment descriptor */
    gdt_set_gate(
            4,          /* NUM:    second entry in the GDT */
            0,          /* BASE:   base address of 0 */
            0xFFFFFFFF, /* LIMIT:  spans all of memory */
            0xF2,       /* ACCESS: present, kernel ring 3, NOT executable, can only 
                            be read from ring 0, READ/WRITE, access bit is 0 */
            0xCF        /* FLAGS: `limit' is in 4K blocks, 32 bit protected mode */
            );

    /* Load the new GDT we just defined, replacing the GDT that GRUB provides */
    gdt_flush();
}
