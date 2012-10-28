; This is the kernel's entry point. We could either call `main' here,
; or we can use this to setup the stack or other nice stuff, like
; perhaps setting up the GDT and segments. Please note that interrupts
; are disabled at this point: More on interrupts later!
[BITS 32]
global start
start:
    mov esp, _sys_stack     ; This points the stack to our new stack area
    jmp stublet

; This part MUST be 4byte aligned, so we solve that issue using 'ALIGN 4'
ALIGN 4
mboot:
    ; Multiboot macros to make a few lines later more readable
    MULTIBOOT_PAGE_ALIGN	equ 1<<0
    MULTIBOOT_MEMORY_INFO	equ 1<<1
    MULTIBOOT_AOUT_KLUDGE	equ 1<<16
    MULTIBOOT_HEADER_MAGIC	equ 0x1BADB002
    MULTIBOOT_HEADER_FLAGS	equ MULTIBOOT_PAGE_ALIGN | MULTIBOOT_MEMORY_INFO | MULTIBOOT_AOUT_KLUDGE
    MULTIBOOT_CHECKSUM	equ -(MULTIBOOT_HEADER_MAGIC + MULTIBOOT_HEADER_FLAGS)
    EXTERN code, bss, end

    ; This is the GRUB Multiboot header.  Allocate dword size values.
    dd MULTIBOOT_HEADER_MAGIC
    dd MULTIBOOT_HEADER_FLAGS
    dd MULTIBOOT_CHECKSUM
    
    ; AOUT kludge - must be physical addresses. Make a note of these:
    ; The linker script fills in the data for these ones!
    dd mboot
    dd code
    dd bss
    dd end
    dd start

; This is an endless loop here.  We hand over control to the `main' function
; in `main.c' here.
stublet:
    extern main
    call main
    jmp $


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
                       ; data section is.

    jmp 0x08:foo       ; This ``far jump" appears to be the only way to set
                       ; the Code Segment (cs) register.  So this command loads 
                       ; `IP' with the address of `foo' and loads `CS' with
                       ; 0x08.  Why 0x08?  Because 0x08 is the offset in our GDT
                       ; in which the code segment entry lives.  Remember,
                       ; each of the entries in the GDT are 8 bytes.
foo:
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
    mov ax, ds   ; Lower 16 bits of eax = ds
    push eax     ; save Data Segment descriptor

    mov ax, 0x10 ; Load the kernel Data Segment descriptor
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    ;mov eax, esp
    ;push eax     ; save the old stack pointer
    ;mov eax, fault_handler

    call fault_handler ; Call the C function to handle a fault.

    pop eax      ; reload the original Data Segment descriptor
    ;pop gs
    ;pop fs
    ;pop es
    ;pop ds
    popa         ; reload the original data segment descriptor
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    add esp, 8   ; Leap-frog over the exception number and error code we
                 ; pushed onto the stack before we got here.
    sti          ; enable iterrupts agains
    iret         ; Pops 5 things at once: cs, eip, eflags, ss, and esp.


; Here is the definition of our BSS section. Right now, we'll use
; it just to store the stack. Remember that a stack actually grows
; downwards, so we declare the size of the data before declaring
; the identifier '_sys_stack'
SECTION .bss
    resb 8192               ; This reserves 8KBytes of memory here
_sys_stack:
