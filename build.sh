#!/bin/bash 

echo Cleaning...
rm *.o

echo Now assembling, compiling, and linking your kernel...
nasm -f elf -o start.o start.asm

# remember this spot here: we will add `gcc' commands here to compile C sources

# create object module for main
gcc -Wall -O -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc \
    -fno-builtin -I./include -c -o main.o main.c

# create object module for screen
gcc -Wall -O -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc \
    -fno-builtin -I./include -c -o screen.o screen.c

# create object module for GDT
gcc -Wall -O -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc \
    -fno-builtin -I./include -c -o gdt.o gdt.c

# create object module for GDT
gcc -Wall -O -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc \
    -fno-builtin -I./include -c -o idt.o idt.c

# this links all your files.  remember that as you add *.o files, you need to
# add them after start.o.  if you don't add them at all, they won't be in your
# kernel!
ld -T link.ld -o kernel.bin start.o main.o screen.o gdt.o idt.o

echo Done!
