; boot.s -- kernel start location.  also defines multiboot header.

; Multiboot macros to make a few lines later more readable
MULTIBOOT_PAGE_ALIGN	equ 1<<0 ; Load kernel and modules on a page boundary
MULTIBOOT_MEMORY_INFO	equ 1<<1 ; Provide your kernel with memory info
MULTIBOOT_HEADER_MAGIC	equ 0x1BADB002 ; Magic number GRUB looks for
MULTIBOOT_HEADER_FLAGS	equ MULTIBOOT_PAGE_ALIGN | MULTIBOOT_MEMORY_INFO
MULTIBOOT_CHECKSUM	equ -(MULTIBOOT_HEADER_MAGIC + MULTIBOOT_HEADER_FLAGS)

[BITS 32]

[GLOBAL mboot]
[EXTERN code]  ; defined in link.ld
[EXTERN bss]   ; defined in link.ld
[EXTERN end]   ; defined in link.ld

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
[EXTERN main]  ; The `main' in `main.c'

; First code executed in the kernel!
start:
    push ebx                ; Load multiboot header location

    cli                     ; disable interrupts
    call main               ; enter into main()
    jmp $                   ; enter into an infinite loop
