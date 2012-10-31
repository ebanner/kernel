; boot.s -- Kernel start location.  Also defines multiboot header.

; Multiboot macros to make a few lines later more readable
MULTIBOOT_PAGE_ALIGN	equ 1<<0 ; Load kernel and modules on a page boundary
MULTIBOOT_MEMORY_INFO	equ 1<<1 ; Provide your kernel with memory info
MULTIBOOT_HEADER_MAGIC	equ 0x1BADB002
MULTIBOOT_HEADER_FLAGS	equ MULTIBOOT_PAGE_ALIGN | MULTIBOOT_MEMORY_INFO
MULTIBOOT_CHECKSUM	equ -(MULTIBOOT_HEADER_MAGIC + MULTIBOOT_HEADER_FLAGS)

[BITS 32]

[GLOBAL mboot]
[EXTERN code]  /* defined in link.ld */
[EXTERN bss]   /* defined in link.ld */
[EXTERN end]   /* defined in link.ld */

mboot:
    ; This is the GRUB Multiboot header.  Allocate dword size values.
    dd MULTIBOOT_HEADER_MAGIC
    dd MULTIBOOT_HEADER_FLAGS
    dd MULTIBOOT_CHECKSUM
    
    dd mboot
    dd code
    dd bss
    dd end
    dd start

[GLOBAL start]
[EXTERN main]

start:
    push ebx                ; Load multiboot header location

    cli                     ; disable interrupts
    call main               ; enter into main()
    jmp $                   ; enter into an infinite loop


; The function loads a new GDT and fixes all segment registers.
global gdt_flush       ; Allows the C code in `gdt.c' to link to this
extern gp              ; Says that `gp' is in another file
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
global idt_load
extern idtp
idt_load:
    lidt [idtp]
    ret

; Labels to hangle interrup execeptions.
global isr0
global isr1
global isr2
global isr3
global isr4
global isr5
global isr6
global isr7
global isr8
global isr9
global isr10
global isr11
global isr12
global isr13
global isr14
global isr15
global isr16
global isr17
global isr18
global isr19
global isr20
global isr21
global isr22
global isr23
global isr24
global isr25
global isr26
global isr27
global isr28
global isr29
global isr30
global isr31

;  0: Divide by Zero Exception
isr0:
    cli         ; Disable interrupts and disable IRQs from firing
    push byte 0 ; A normal ISR stub that pops a dummy error code to keep a
                ; uniform stack frame
    push byte 0
    jmp isr_common_stub

;  1: Debug Exception
isr1:
    cli
    push byte 0
    push byte 1
    jmp isr_common_stub

;  2: Non Maskable Interrupt Exception
isr2:
    cli
    push byte 0
    push byte 2
    jmp isr_common_stub

;  3: Breakpoint Exception
isr3:
    cli
    push byte 0
    push byte 3
    jmp isr_common_stub

;  4: Into Detected Overflow Exception
isr4:
    cli
    push byte 0
    push byte 4
    jmp isr_common_stub

;  5: Out of Bounds Exception
isr5:
    cli
    push byte 0
    push byte 5
    jmp isr_common_stub

;  6: Invalid Opcode Exception
isr6:
    cli
    push byte 0
    push byte 6
    jmp isr_common_stub

;  7: No Coprocessor Exception
isr7:
    cli
    push byte 0
    push byte 7
    jmp isr_common_stub

;  8: Double Fault Exception (With Error Code!)
isr8:
    cli
    push byte 8 ; We don't push a dummy value on the stack because the
                ; execption pushes an error code on the stack already
    jmp isr_common_stub

;  9: Coprocessor Segment Overrun Exception
isr9:
    cli
    push byte 0
    push byte 9
    jmp isr_common_stub

; 10: Bad TSS Exception
isr10:
    cli
    push byte 10 ; We don't push a dummy value on the stack because the
                 ; execption pushes an error code on the stack already
    jmp isr_common_stub

; 11: Segment Not Present Exception
isr11:
    cli
    push byte 11 ; We don't push a dummy value on the stack because the
                 ; execption pushes an error code on the stack already
    jmp isr_common_stub

; 12: Stack Fault Exception
isr12:
    cli
    push byte 12 ; We don't push a dummy value on the stack because the
                 ; execption pushes an error code on the stack already
    jmp isr_common_stub

;  13: General Protection Fault Exception
isr13:
    cli
    push byte 13 ; We don't push a dummy value on the stack because the
                 ; execption pushes an error code on the stack already
    jmp isr_common_stub

; 14: Page Fault Excpeption
isr14:
    cli
    push byte 14 ; We don't push a dummy value on the stack because the
                 ; execption pushes an error code on the stack already
    jmp isr_common_stub

; 15: Unknown Interrupt Exception
isr15:
    cli
    push byte 0
    push byte 15
    jmp isr_common_stub

; 16: Coprocessor Fault Exception
isr16:
    cli
    push byte 0
    push byte 16
    jmp isr_common_stub

; 17: Alignment Check Exception (486+)
isr17:
    cli
    push byte 0
    push byte 17
    jmp isr_common_stub

; 18: Machine Check Exception (Pentium 586+)
isr18:
    cli
    push byte 0
    push byte 18
    jmp isr_common_stub

; 19: Reserved Exception
isr19:
    cli
    push byte 0
    push byte 19
    jmp isr_common_stub

; 20: Reserved Exception
isr20:
    cli
    push byte 0
    push byte 20
    jmp isr_common_stub

; 21: Reserved Exception
isr21:
    cli
    push byte 0
    push byte 21
    jmp isr_common_stub

; 22: Reserved Exception
isr22:
    cli
    push byte 0
    push byte 22
    jmp isr_common_stub

; 23: Reserved Exception
isr23:
    cli
    push byte 0
    push byte 23
    jmp isr_common_stub

; 24: Reserved Exception
isr24:
    cli
    push byte 0
    push byte 24
    jmp isr_common_stub

; 25: Reserved Exception
isr25:
    cli
    push byte 0
    push byte 25
    jmp isr_common_stub
    
; 26: Reserved Exception
isr26:
    cli
    push byte 0
    push byte 26
    jmp isr_common_stub

; 27: Reserved Exception
isr27:
    cli
    push byte 0
    push byte 27
    jmp isr_common_stub

; 28: Reserved Exception
isr28:
    cli
    push byte 0
    push byte 28
    jmp isr_common_stub

; 29: Reserved Exception
isr29:
    cli
    push byte 0
    push byte 29
    jmp isr_common_stub

; 30: Reserved Exception
isr30:
    cli
    push byte 0
    push byte 30
    jmp isr_common_stub

; 31: Reserved Exception
isr31:
    cli
    push byte 0
    push byte 31
    jmp isr_common_stub

; General routine that executes every time the an interrupt execption is
; thrown.  This saves the state of the processor on the stack so we can execute
; the ISR.  This function also sets up kernel mode segments, calls
; the C-level fault handler, and finally restores the stack frame.
extern fault_handler
isr_common_stub:
    pusha        ; pushes all general purpose registers onto the stack in the
                 ; following order: eax, ecx, edx, ebx, esp, ebp, esi, edi, 
    mov ax, ds   
    push eax     ; save Data Segment descriptor

    mov ax, 0x10 ; Load the kernel Data Segment descriptor
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    call fault_handler ; Call the C function to handle a fault.

    pop eax      ; reload the original Data Segment descriptor
    mov ds, ax   ; restore data segment and other segment registers
    mov es, ax
    mov fs, ax
    mov gs, ax

    popa         ; pops edi, esi, ebp, ...
    add esp, 8   ; Leap-frog over the exception number and error code we
                 ; pushed onto the stack before we got here.
    sti          ; enable iterrupts agains
    iret         ; Pops 5 things at once: cs, eip, eflags, ss, and esp.

; remap the IRQs that the PIC sends to the processor
; currently, IRQ0 to IRQ7 are mapped to IDT entries 8-15
; further, IRQ8 to IRQ15 are mapped to IDT entries 0x70-0x78
; we wish to map IRQ0 to IRQ15 to 0x20-0x2F
global irq0
global irq1
global irq2
global irq3
global irq4
global irq5
global irq6
global irq7
global irq8
global irq9
global irq10
global irq11
global irq12
global irq13
global irq14
global irq15

; 32: IRQ0
irq0:
    cli          ; disable interrupts
    push byte 0  ; neither the CPU nor the PIC pushes an error value on the
    push byte 32 ; stack, so we push on a dummy value to be consistent
    jmp irq_common_stub

; 33: IRQ1
irq1:
    cli          ; disable interrupts
    push byte 0  ; neither the CPU nor the PIC pushes an error value on the
    push byte 33 ; stack, so we push on a dummy value to be consistent
    jmp irq_common_stub

; 34: IRQ2
irq2:
    cli          ; disable interrupts
    push byte 0  ; neither the CPU nor the PIC pushes an error value on the
    push byte 34 ; stack, so we push on a dummy value to be consistent
    jmp irq_common_stub

; 35: IRQ3
irq3:
    cli          ; disable interrupts
    push byte 0  ; neither the CPU nor the PIC pushes an error value on the
    push byte 35 ; stack, so we push on a dummy value to be consistent
    jmp irq_common_stub

; 36: IRQ4
irq4:
    cli          ; disable interrupts
    push byte 0  ; neither the CPU nor the PIC pushes an error value on the
    push byte 36 ; stack, so we push on a dummy value to be consistent
    jmp irq_common_stub

; 37: IRQ5
irq5:
    cli          ; disable interrupts
    push byte 0  ; neither the CPU nor the PIC pushes an error value on the
    push byte 37 ; stack, so we push on a dummy value to be consistent
    jmp irq_common_stub

; 38: IRQ6
irq6:
    cli          ; disable interrupts
    push byte 0  ; neither the CPU nor the PIC pushes an error value on the
    push byte 38 ; stack, so we push on a dummy value to be consistent
    jmp irq_common_stub

; 39: IRQ7
irq7:
    cli          ; disable interrupts
    push byte 0  ; neither the CPU nor the PIC pushes an error value on the
    push byte 39 ; stack, so we push on a dummy value to be consistent
    jmp irq_common_stub

; 40: IRQ8
irq8:
    cli          ; disable interrupts
    push byte 0  ; neither the CPU nor the PIC pushes an error value on the
    push byte 40 ; stack, so we push on a dummy value to be consistent
    jmp irq_common_stub

; 41: IRQ9
irq9:
    cli          ; disable interrupts
    push byte 0  ; neither the CPU nor the PIC pushes an error value on the
    push byte 41 ; stack, so we push on a dummy value to be consistent
    jmp irq_common_stub

; 42: IRQ10
irq10:
    cli          ; disable interrupts
    push byte 0  ; neither the CPU nor the PIC pushes an error value on the
    push byte 42 ; stack, so we push on a dummy value to be consistent
    jmp irq_common_stub

; 43: IRQ11
irq11:
    cli          ; disable interrupts
    push byte 0  ; neither the CPU nor the PIC pushes an error value on the
    push byte 43 ; stack, so we push on a dummy value to be consistent
    jmp irq_common_stub

; 44: IRQ12
irq12:
    cli          ; disable interrupts
    push byte 0  ; neither the CPU nor the PIC pushes an error value on the
    push byte 44 ; stack, so we push on a dummy value to be consistent
    jmp irq_common_stub

; 45: IRQ13
irq13:
    cli          ; disable interrupts
    push byte 0  ; neither the CPU nor the PIC pushes an error value on the
    push byte 45 ; stack, so we push on a dummy value to be consistent
    jmp irq_common_stub

; 46: IRQ14
irq14:
    cli          ; disable interrupts
    push byte 0  ; neither the CPU nor the PIC pushes an error value on the
    push byte 46 ; stack, so we push on a dummy value to be consistent
    jmp irq_common_stub

; 47: IRQ15
irq15:
    cli          ; disable interrupts
    push byte 0  ; neither the CPU nor the PIC pushes an error value on the
    push byte 47 ; stack, so we push on a dummy value to be consistent
    jmp irq_common_stub

; Here is a stub for handling IRQ-based ISRs.  Here we save the state of the
; processor and call our general IRQ handler (located in irq.c)
irq_common_stub:
    pusha        ; pushes edi, esi, ebp, esp, ebx, edx, ecx, eax

    mov ax, ds
    push eax     ; save the data segment in eax

    mov ax, 0x10 ; load the kernel data segment descriptor
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    ;call irq_handler  ; C function that handles IRQs

    pop ebx      ; reload original data segment descriptors
    mov ds, bx
    mov es, bx
    mov fs, bx
    mov gs, bx

    popa              ; analog of pusha
    add esp, 8        ; leap frog past two values we pushed on the stack before
    sti               ; enable interrupts again
    iret              ; pops everything that the processor automagically
                      ; pushed when the IRQ was called (cs, eip, eflags, ss, esp)

; Here is the definition of our BSS section. Right now, we'll use
; it just to store the stack. Remember that a stack actually grows
; downwards, so we declare the size of the data before declaring
; the identifier '_sys_stack'
;SECTION .bss
;    resb 8192               ; This reserves 8KBytes of memory here
;_sys_stack:
