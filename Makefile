CC = gcc
LD = ld
ASM = nasm

objects = start.o main.o screen.o gdt.o idt.o isrs.o

CFLAGS = -Wall -O -fstrength-reduce -fomit-frame-pointer \
	 -finline-functions -nostdinc -fno-builtin -I./include -c


kernel.bin : $(objects)
	$(LD) -T link.ld -o kernel.bin $(objects)

start.o :
	$(ASM) -f elf -o start.o start.asm

main.o : main.c start.asm gdt.c idt.c isrs.c include/system.h
	$(CC) $(CFLAGS) -o main.o main.c

screen.o : start.asm screen.c include/system.h 
	$(CC) $(CFLAGS) -o screen.o screen.c

gdt.o : start.asm gdt.c include/system.h 
	$(CC) $(CFLAGS) -o gdt.o gdt.c

idt.o : start.asm idt.c include/system.h 
	$(CC) $(CFLAGS) -o idt.o idt.c

isrs.o : start.asm isrs.c idt.c include/system.h 
	$(CC) $(CFLAGS) -o isrs.o isrs.c

clean :
	rm kernel.bin $(objects)
