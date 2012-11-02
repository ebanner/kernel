; descriptors.s -- provides functions for loading a new GDT and IDT

; The function loads a new GDT and fixes all segment registers
GLOBAL gdt_flush       ; Allows the C code in `gdt.c' to link to this
EXTERN gp              ; Says that `gp' is in another file
gdt_flush:
    lgdt [gp]          ; Load the GDT with `gp' (defined in `gdt.'), which is
                       ; the pointer to our new GDT
    mov ax, 0x10       ; 0x10 is the offset in the GDT which contains the base
    mov ds, ax         ; address of the array of GDT entries.  Why are we
    mov es, ax         ; setting the Stack Segment (ss), Data Segment (ds),
    mov fs, ax         ; Extra Segment (es), F Segment (fs), and G Segement
    mov gs, ax         ; (gs) to point to the Data Segment in our GDT?  It's bc
    mov ss, ax         ; these registers all need to know where the kernel's
                       ; data segment is.

    jmp 0x08:setCS     ; This ``far jump" appears to be the only way to set
                       ; the Code Segment (cs) register.  So this command loads 
                       ; `IP' with the address of `foo' and loads `CS' with
                       ; 0x08.  Why 0x08?  Because 0x08 is the offset in our GDT
                       ; in which the code segment entry lives.  Remember,
                       ; each of the entries in the GDT are 8 bytes.
setCS:
    ret                ; Returns back to the C code!


; Lets the processor know where the new IDT lives.
GLOBAL idt_load
EXTERN idtp
idt_load:
    lidt [idtp]
    ret
