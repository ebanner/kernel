CC = gcc
LD = ld
ASM = nasm
INCLUDE = ../include

objects = boot.o main.o screen.o gdt.o idt.o isrs.o common.o

CFLAGS = -Wall -nostdlib -nostdinc -fno-builtin \
	 -fno-stack-protector -I../include -c


kernel.bin : $(objects) link.ld
	$(LD) -T link.ld -o kernel.bin $(objects)

boot.o :
	$(ASM) -f elf -o boot.o boot.asm

main.o : main.c boot.asm gdt.c idt.c isrs.c $(INCLUDE)/system.h
	$(CC) $(CFLAGS) -o main.o main.c

common.o : common.c
	$(CC) $(CFLAGS) -o common.o common.c

screen.o : boot.asm screen.c $(INCLUDE)/system.h 
	$(CC) $(CFLAGS) -o screen.o screen.c

gdt.o : boot.asm gdt.c $(INCLUDE)/system.h 
	$(CC) $(CFLAGS) -o gdt.o gdt.c

idt.o : boot.asm idt.c $(INCLUDE)/system.h 
	$(CC) $(CFLAGS) -o idt.o idt.c

isrs.o : boot.asm isrs.c idt.c $(INCLUDE)/system.h 
	$(CC) $(CFLAGS) -o isrs.o isrs.c

clean :
	rm kernel.bin $(objects)
