CC = gcc
LD = ld
ASM = nasm
INCLUDE = ../include

objects = boot.o main.o screen.o gdt.o idt.o isr.o common.o

CFLAGS = -Wall -nostdlib -nostdinc -fno-builtin \
	 -fno-stack-protector -I../include -c


kernel : $(objects) link.ld
	$(LD) -T link.ld -o kernel $(objects)

boot.o :
	$(ASM) -f elf -o boot.o boot.s

main.o : main.c boot.s gdt.c idt.c isr.c \
    $(INCLUDE)/gdt.h $(INCLUDE)/gdt.h $(INCLUDE)/screen.h
	$(CC) $(CFLAGS) -o main.o main.c

common.o : common.c 
	$(CC) $(CFLAGS) -o common.o common.c

screen.o : boot.s screen.c \
    $(INCLUDE)/common.h $(INCLUDE)/screen.h
	$(CC) $(CFLAGS) -o screen.o screen.c

gdt.o : boot.s gdt.c \
    $(INCLUDE)/gdt.h
	$(CC) $(CFLAGS) -o gdt.o gdt.c

idt.o : boot.s idt.c \
    $(INCLUDE)/common.h $(INCLUDE)/idt.h
	$(CC) $(CFLAGS) -o idt.o idt.c

isr.o : boot.s isr.c idt.c \
    $(INCLUDE)/isr.h $(INCLUDE)/screen.h
	$(CC) $(CFLAGS) -o isr.o isr.c

clean :
	rm kernel $(objects)
