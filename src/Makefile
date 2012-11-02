CC = gcc
LD = ld
ASM = nasm
INCLUDE = ../include

objects = boot.o interrupts.o descriptors.o \
	  main.o common.o screen.o gdt.o idt.o isr.o timer.o

includes = $(INCLUDE)/gdt.h \
	   $(INCLUDE)/common.h \
	   $(INCLUDE)/idt.h \
	   $(INCLUDE)/isr.h \
	   $(INCLUDE)/screen.h

GRUB = ../grub
bochs_files = $(GRUB)/stage1 $(GRUB)/stage2 $(GRUB)/pad kernel

CFLAGS = -Wall -nostdlib -nostdinc -fno-builtin \
	 -fno-stack-protector -I../include -c

# bochs target
floppy.img : kernel
	cat $(bochs_files) > floppy.img

kernel : $(objects) link.ld
	$(LD) -T link.ld -o kernel $(objects)

boot.o : boot.s
	$(ASM) -f elf -o boot.o boot.s
	
interrupts.o : interrupts.s
	$(ASM) -f elf -o interrupts.o interrupts.s

descriptors.o : descriptors.s
	$(ASM) -f elf -o descriptors.o descriptors.s

main.o : main.c gdt.c idt.c isr.c $(includes)
	$(CC) $(CFLAGS) -o main.o main.c

common.o : common.c 
	$(CC) $(CFLAGS) -o common.o common.c

screen.o : screen.c $(includes)
	$(CC) $(CFLAGS) -o screen.o screen.c

gdt.o : descriptors.s gdt.c $(includes)
	$(CC) $(CFLAGS) -o gdt.o gdt.c

idt.o : descriptors.s idt.c $(includes)
	$(CC) $(CFLAGS) -o idt.o idt.c

isr.o : isr.c idt.c $(includes)
	$(CC) $(CFLAGS) -o isr.o isr.c

timer.o : timer.c common.c isr.c screen.c $(includes)
	$(CC) $(CFLAGS) -o timer.o timer.c

clean :
	rm kernel $(objects)
