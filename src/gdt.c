// gdt.c -- functions for setting up and inserting descriptors into the GDT

/* Defines a GDT entry.  We say `packed', because it prevents the compiler from
 * doing things that it thinks is best.  `Packing' prevents compiler
 * optimization. */
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
struct gdt_entry gdt[3];
struct gdt_ptr gp;

/* This will be a function in `start.esm'.  We use this to properly reload the
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
    gp.limit = ( sizeof(struct gdt_entry) * 3) - 1;  // zero-indexed--hence the need to subtract 1
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

    /* Code Segment descriptor */
    gdt_set_gate(
            1,          /* NUM:    first entry in the GDT */
            0,          /* BASE:   base address of 0 */
            0xFFFFFFFF, /* LIMIT:  spans all of memory */
            0x9A,       /* ACCESS: present, kernel ring 0, executable, can only 
                            be executed from ring 0, READ ONLY, access bit is 0 */
            0xCF        /* FLAGS: `limit' is in 4K blocks, 32 bit protected mode */
            );

    /* Data Segment descriptor */
    gdt_set_gate(
            2,          /* NUM:    second entry in the GDT */
            0,          /* BASE:   base address of 0 */
            0xFFFFFFFF, /* LIMIT:  spans all of memory */
            0x92,       /* ACCESS: present, kernel ring 0, NOT executable, can only 
                            be read from ring 0, READ/WRITE, access bit is 0 */
            0xCF        /* FLAGS: `limit' is in 4K blocks, 32 bit protected mode */
            );

    /* Load the new GDT we just defined, replacing the GDT that GRUB provides */
    gdt_flush();
}
